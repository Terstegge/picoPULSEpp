# picoPULSE++

**picoPULSE++** is a high-speed logic analyzer firmware for the Raspberry Pi **RP2040** and **RP2350** microcontrollers, featuring native support in **sigrok / PulseView**.

---

## Key Features

* **Supported Hardware:** Raspberry Pi Pico / RP2040 and Raspberry Pi Pico 2 / RP2350 boards
* **Capture Capabilities:** *[e.g. Up to 16 / 24 digital channels, RLE compression, configurable trigger conditions]*
* **Plug & Play USB Interface:** Uses a standard USB CDC serial port (`/dev/ttyACMx` on Linux, `COMx` on Windows) — no custom USB drivers or Zadig setup required
* **Full PulseView Integration:** Dedicated hardware driver in `libsigrok` with support for all sigrok protocol decoders

---

## Quick Start

### 1. Flash the Firmware (RP2040 / RP2350)
1. Download the pre-built `.uf2` firmware file for your board from the [Releases](https://github.com) page *(or build it from source — see below)*.
2. Hold the **BOOTSEL** button on your Pico / Pico 2 while connecting it via USB.
3. Copy the `.uf2` file onto the `RPI-RP2` / `RP2350` mass storage drive. The board will reboot automatically.

### 2. Install & Run PulseView
Pre-built binaries with native **picoPULSE++** support are available on the [Releases](https://github.com) page:

* **Windows (64-bit):** Download and run `PulseView-0.5.0-picopulse-x86_64-installer.exe`.
* **Linux (x86_64):** Download `PulseView-0.5.0-picopulse-x86_64.AppImage`, make it executable (`chmod +x`), and run it.
  *(Ensure your user is a member of the `dialout` group for serial port access without `root`.)*

### 3. Connect in PulseView
1. Open **PulseView** and click **Connect to Device** (next to the device dropdown).
2. Select **`picoPULSE++`** as the driver and choose the serial port (`COMx` or `/dev/ttyACMx`).
3. Click **Scan for devices**, select **picoPULSE++**, and start capturing.

---

## Pinout / Wiring

| Channel | RP2040 / RP2350 GPIO | Notes |
| :--- | :--- | :--- |
| **D0 – D7** | *[e.g. GPIO 0 – GPIO 7]* | *3.3 V logic levels* |
| **D8 – D15** | *[e.g. GPIO 8 – GPIO 15]* | *3.3 V logic levels* |
| **GND** | **GND** | Common ground with target circuit |

> **Warning:** RP2040 and RP2350 GPIO pins are **3.3 V** logic (not 5 V tolerant). Use a level shifter when probing 5 V signals.

---

## Building from Source

### Firmware (Pico SDK)
```bash
git clone https://github.com
cd picoPULSEpp
mkdir build && cd build
cmake ..
make -j\$(nproc)
```
*(Pass `-DPICO_BOARD=pico2` to CMake when building for the RP2350.)*

### Host Software (`libsigrok` & `PulseView`)
The modified host software sources are maintained in the following forks:
* **libsigrok:** [Terstegge/libsigrok](https://github.com)
* **PulseView:** [Terstegge/pulseview](https://github.com)

---

## License

* The **picoPULSE++** firmware in this repository is licensed under the *[MIT / GPLv3 — insert your chosen license]* license.
* **libsigrok** and **PulseView** are licensed under the **GNU General Public License v3.0 or later (GPLv3+)**.
