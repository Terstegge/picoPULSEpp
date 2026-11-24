///////////////////////////////////////////////////////////////
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2026 Andreas Terstegge
//
// This file is part of picoprobe++
// A CMSIS-DAP v2 firmware for RP2040/RP2350 based debug probes
///////////////////////////////////////////////////////////////
//
// picoprobe++ main code.
// Create the objects  for the various functionalities
// (DAP interface, CDC ACM UART, Logic Analyzer). Sign
// up all tasks and start the multitasking kernel.
// The USB configuration is mainly controlled by the
// configuration file, depending on the MCU board.
//
#include "config.h"

#include "usb_bos.h"
#include "usb_dcd.h"
#include "usb_device.h"
#include "usb_device_controller.h"

#include "usb_ms_OS_20_capability.h"
#include "usb_ms_func_subset.h"
#include "usb_ms_compatible_ID.h"
#include "usb_ms_registry_property.h"
using enum wPropertyDataType_t;

#include "usb_cdc_acm_adapter.h"
#include "usb_log.h"

#include "usb_la_device.h"
#include "LA_protocol.h"

#include "posix_io.h"
#include "task.h"
#include "task_monitor.h"

int main() {
    // Get the unique ID of this MCU/board
    auto id = unique_id_rp2xxx::read_unique_id_string();

    // Log levels of various components
    usb_log::inst.setLevel(usb_log::DEBUG_LEVEL_USB);

    // Optional: Firmware Debug UART
    #ifdef DEBUG_UART_ENABLE
    uart_rp2xxx uart(DEBUG_UART_TX_GPIO, DEBUG_UART_RX_GPIO);
    posix_io::inst.register_stdio(uart);
    #endif

    // Set up USB Device driver, USB device and put
    // generic device controller on top
    usb_dcd & driver = usb_dcd::inst();
    usb_device device;
    usb_device_controller controller(driver, device);

    // USB device descriptor
    device.set_bcdUSB          (USB_DEV_bcdUSB);
    device.set_bMaxPacketSize0 (USB_DEV_bMaxPacketSize0);
    device.set_idVendor        (USB_DEV_VID);
    device.set_idProduct       (USB_DEV_PID);
    device.set_Manufacturer    (USB_DEV_Manufacturer);
    device.set_Product         (USB_DEV_Product);
    device.set_SerialNumber    (id.data());
    device.set_bcdDevice       (USB_DEV_bcdDevice);

    // USB configuration descriptor
    usb_configuration config(device);
    config.set_bConfigurationValue( 1 );
    config.set_bmAttributes({ .remote_wakeup = 0,
                              .self_powered  = 0,
                              .bus_powered   = 1});
    config.set_bMaxPower_mA(100);

    // Set up Logic Analyzer (USB part and protocol processing)
    usb_la_device la_device(controller, config);
    la_device.sign_up();
    la_device.setPriority(80);
    LA_protocol la(la_device);
    la.set_serial(id.data());
    la.sign_up();
    la.setPriority(80);

    // Optional: Set up another CDC ACM device for debugging the debugger
    // firmware (connected to stdio of the debugger SW).
    usb_cdc_acm_adapter usb_uart_adapter(controller, config);
    usb_uart_adapter.set_FunctionName("Firmware debug UART");
    posix_io::inst.register_stdio(usb_uart_adapter);


    // Add BOS and MS OS 2.0 capability descriptor
    usb_bos bos(controller, device); // Add a Binary Object Store
    usb_ms_OS_20_capability ms_os20(bos);

    // Activate the USB device
    driver.pullup_enable(true);

    // Wait until USB enumeration has finished
    TUPP_LOG(LOG_INFO, "Waiting for USB connection ...");
    while (!controller.active_configuration) {
        task::sleep_ms(20);
    }

    TUPP_LOG(LOG_INFO, "Starting the scheduler ...");
    task::start_scheduler();
}
