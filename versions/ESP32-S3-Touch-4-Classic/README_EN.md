<p align="left"><img alt="OSPTEK" src="./images/logo.png" width="200" /></p>

<h1 align="center">OSPTEK ESP32-S3-Touch-4-Classic Touch Panel</h1>

<p align="center"><b>86-Type Panel · Touch UI · One RS485 Port</b></p>

<p align="center">English | <a href="./README.md">简体中文</a> · <a href="../../README_EN.md">Family index</a></p>

<p align="center">
  <img alt="MCU: ESP32-S3" src="https://img.shields.io/badge/MCU-ESP32--S3-E7352C?style=flat-square" />
  <img alt="Display: 4 inch 480x480" src="https://img.shields.io/badge/Display-4%22_480%C3%97480-3498DB?style=flat-square" />
  <img alt="Wireless: Wi-Fi Bluetooth" src="https://img.shields.io/badge/Wireless-Wi--Fi_%2B_BLE_5-0A7BBB?style=flat-square" />
  <img alt="RS485" src="https://img.shields.io/badge/Interface-RS485-F39C12?style=flat-square" />
</p>

## Contents

- [Overview](#overview)
- [Other SKUs in this series](#other-skus-in-this-series)
- [Features](#features)
- [Applications](#applications)
- [Specifications](#specifications)
- [Display](#display)
- [Hardware Resources](#hardware-resources)
- [Quick Start](#quick-start)
- [Examples](#examples)
- [Prebuilt Firmware](#prebuilt-firmware)
- [Repository Structure](#repository-structure)
- [Documentation](#documentation)
- [Where to Buy](#where-to-buy)
- [Support](#support)

---

## Overview

> 📌 This page is **ESP32-S3-Touch-4-Classic** (schematic: `ESP32-S3-Touch基础板V1.0`). The bring-up example / prebuilt firmware are **verified on Classic**.

OSPTEK **ESP32-S3-Touch-4-Classic** is a **4-inch touch panel** based on an ESP32-S3 module (2.4 GHz Wi-Fi + Bluetooth LE 5), with **16 MB Flash** and **8 MB PSRAM**, and an onboard **YDP395B003-V4** (~3.95") **480×480** RGB capacitive touch display (driver **ST7701S**).

The module is **ESP32-S3-WROOM-1-N16R8**. It includes one **RS485** port, USB Type-C (power / flash), I²C, and a buzzer—suited as a finished product for smart control panels and interactive HMI.

## Features

- **4" square touch display**: YDP395B003-V4, 480×480, ST7701S + capacitive touch, ideal for 86-type panel UIs
- **ESP32-S3 MCU**: Wi-Fi + BLE 5, 16 MB Flash + 8 MB PSRAM
- **Industrial bus**: onboard RS485
- **Power**: USB Type-C; wide-range terminal input (see schematic)
- **Debug**: CH340K USB-UART
- **Expansion**: I²C, LCD FPC, buzzer, and more (see schematic)

## Applications

- Smart home central control / 86-type interactive panels
- Home gateway and local HMI
- Industrial control and device status displays
- Smart lighting and building panels

## Specifications

### MCU & Memory

| Item | Specification |
| ---- | ------------- |
| Product model | ESP32-S3-Touch-4-Classic |
| Board / schematic | ESP32-S3-Touch Standard board V1.0 |
| Module | ESP32-S3-WROOM-1-N16R8 |
| Flash | 16 MB |
| PSRAM | 8 MB |
| Wireless | 2.4 GHz Wi-Fi, Bluetooth LE 5 |

### Display & Touch

Summary below; full module parameters, driver, and touch details are in [Display](#display).

| Item | Specification |
| ---- | ------------- |
| Panel module | YDP395B003-V4 (~3.95") |
| Resolution | 480×480 |
| Driver IC | ST7701S |
| Display interface | RGB 18-bit |
| Touch | Capacitive (FT6336U, I²C) |
| Brightness | 350 cd/m² typ. |

### Interfaces & Power

| Item | Specification |
| ---- | ------------- |
| USB | Type-C (power, flash) |
| USB-UART | CH340K |
| RS485 | Onboard |
| Sensor | I²C |
| Audio cue | Onboard buzzer |

Power tree and exact electrical ratings: see the [schematic](./docs/ESP32-S3-Touch基础板V1.0.pdf).

## Display

This panel uses OSPTEK TFT module **YDP395B003-V4** with driver IC **ST7701S**, connected via the board LCD FPC. Touch is capacitive (I²C, FT6336U).

### Module Specifications (YDP395B003-V4)

| Item | Specification |
| ---- | ------------- |
| Module model | YDP395B003-V4 |
| Size | 3.95" (diagonal) |
| Display mode | Normally black |
| Resolution | 480 (H) RGB × 480 (V) |
| Dot pitch | 153 μm × 153 μm |
| Active area | 71.86 × 70.18 mm |
| Module outline | 74.66 × 76.54 × 2.06 mm |
| Color arrangement | RGB vertical stripe |
| Interface | RGB 18-bit (HSYNC / VSYNC / DE / DCLK); driver init via 3-wire SPI (SDA / SCL / CS) |
| Driver IC | ST7701S |
| Brightness | 350 cd/m² typ. (min. ~300) |
| Viewing angle | All view (typ. ~80°) |
| Contrast ratio | ~1000:1 typ. |
| Backlight | 8 white LEDs (typ. ~12 V / 40 mA) |
| Operating temp. | −20 ℃ ~ +70 ℃ |
| Storage temp. | −30 ℃ ~ +80 ℃ |

Touch signals such as `TP_SCL` / `TP_SDA` / `TP_INT` / `TP_RESET` are brought out on the same FPC; typical touch supply is 2.8–3.3 V.

### Display Documents

- [Panel datasheet YDP395B003-V4 (PDF)](./docs/YDP_395_B003_V4_d3e49044f9.pdf)
- [Driver IC ST7701S datasheet (PDF)](./docs/ST_7701_S_SPEC_V1_3_f82b940377.pdf)

## Hardware Resources

### Onboard Overview

| Resource | Description |
| -------- | ----------- |
| ESP32-S3-WROOM-1-N16R8 | Wi-Fi + BLE MCU module |
| RS485 | Onboard transceiver |
| USB bridge | CH340K |
| LCD connector | FPC to the LCD |
| Buzzer | Onboard |

### Schematic

- [ESP32-S3-Touch Standard board V1.0 schematic (PDF)](./docs/ESP32-S3-Touch基础板V1.0.pdf)

## Quick Start

1. Prepare: ESP32-S3-Touch-LCD-4, a data-capable USB cable (not charge-only), and a PC (Windows / Linux / macOS).
2. Connect USB between the PC and the panel USB port.
3. **Verify the display (recommended)**: flash the prebuilt firmware in this repo—see [Prebuilt Firmware](#prebuilt-firmware).
   - On Windows, use Espressif’s graphical **[Flash Download Tools](https://www.espressif.com/en/support/download/other-tools)**.
   - Guide: [Flash Download Tool User Guide · ESP32-S3](https://docs.espressif.com/projects/esp-test-tools/en/latest/esp32s3/production_stage/tools/flash_download_tool.html)
4. **Application development**: build the [example](#examples) with ESP-IDF (auto-download via UART DTR/RTS; see schematic).

Hardware details:

- [ESP32-S3-Touch Standard board V1.0 schematic (PDF)](./docs/ESP32-S3-Touch基础板V1.0.pdf)

Espressif tools & getting started:

- [Flash Download Tools (Windows)](https://www.espressif.com/en/support/download/other-tools)
- [ESP-IDF Get Started · ESP32-S3](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/)
- [ESP-IDF 快速入门 · ESP32-S3（中文）](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32s3/get-started/)

## Examples

| Description | Path |
| ----------- | ---- |
| Display bring-up / LVGL demo (RGB ST7701, 480×480) | [`examples/esp32s3-3.95-tft-480x480-rgb-st7701-bringup/`](./examples/esp32s3-3.95-tft-480x480-rgb-st7701-bringup/) |

> Verified on the **Classic** board (same panel as v1).

With ESP-IDF installed:

```bash
cd examples/esp32s3-3.95-tft-480x480-rgb-st7701-bringup
idf.py set-target esp32s3
idf.py build
idf.py -p <PORT> flash monitor
```

Component dependencies are managed by `main/idf_component.yml` and are fetched on the first build.

## Prebuilt Firmware

| File | Flash address | Description |
| ---- | ------------- | ----------- |
| [`firmware/esp32-s3-touch-lcd-4.bin`](./firmware/esp32-s3-touch-lcd-4.bin) | **`0x0`** | Merged image (bootloader + partition table + app) for the display example above |

Flash settings match the project: chip **ESP32-S3**, Flash **16 MB**, **DIO**, **80 MHz**. Write the merged package from address **`0x0`**.

### Option 1: Flash Download Tools (Windows, easiest)

1. Download and open Espressif **[Flash Download Tools](https://www.espressif.com/en/support/download/other-tools)**.
2. Set `ChipType` to **ESP32-S3**, `WorkMode` to Develop, `LoadMode` to UART.
3. Enable one row, select [`firmware/esp32-s3-touch-lcd-4.bin`](./firmware/esp32-s3-touch-lcd-4.bin), address **`0x0`** (do not use app-partition `0x10000`).
4. Set SPI SPEED / SPI MODE / FLASH SIZE as above (80 MHz, DIO, 16 MB).
5. Select the correct COM port and click **START**.

UI details: [Flash Download Tool User Guide · ESP32-S3](https://docs.espressif.com/projects/esp-test-tools/en/latest/esp32s3/production_stage/tools/flash_download_tool.html)

### Option 2: esptool (CLI)

Replace `<PORT>` with your serial port (e.g. `/dev/ttyUSB0`, `COM3`); address is **`0x0`**:

```bash
esptool.py --chip esp32s3 -p <PORT> write_flash 0x0 firmware/esp32-s3-touch-lcd-4.bin
```

## Repository layout

```text
esp32-s3-touch-lcd-4/                                # repo root (nav: ../../README_EN.md)
└── versions/
    └── ESP32-S3-Touch-4-Classic/                    # full materials for this SKU
        ├── README.md
        ├── README_EN.md
        ├── images/
        ├── docs/
        ├── firmware/
        └── examples/
```

## Documentation

### Product Documents

- [ESP32-S3-Touch Standard board V1.0 schematic (PDF)](./docs/ESP32-S3-Touch基础板V1.0.pdf)
- [Panel datasheet YDP395B003-V4 (PDF)](./docs/YDP_395_B003_V4_d3e49044f9.pdf)
- [Driver IC ST7701S datasheet (PDF)](./docs/ST_7701_S_SPEC_V1_3_f82b940377.pdf)
- [Display example](./examples/esp32s3-3.95-tft-480x480-rgb-st7701-bringup/)
- [Prebuilt firmware esp32-s3-touch-lcd-4.bin](./firmware/esp32-s3-touch-lcd-4.bin)

### Chip Documents (Espressif)

- [ESP32-S3 product page](https://www.espressif.com/en/products/socs/esp32-s3)
- [ESP32-S3 系列产品页（中文）](https://www.espressif.com/zh-hans/products/socs/esp32-s3)
- [ESP-IDF Programming Guide · ESP32-S3](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/index.html)
- [ESP-IDF 编程指南 · ESP32-S3（中文）](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32s3/index.html)

## Where to Buy

<p align="center">
  <a href="https://www.aliexpress.com/store/1105701619"><img alt="AliExpress Official Store" src="https://img.shields.io/badge/AliExpress-Official_Store-E62E04?style=for-the-badge&logo=aliexpress&logoColor=white" /></a>
  &nbsp;&nbsp;
  <a href="https://shop110742373.taobao.com/"><img alt="Taobao Official Store" src="https://img.shields.io/badge/Taobao-Official_Store-FF6A00?style=for-the-badge" /></a>
</p>

**International (AliExpress)**

- Store: [OSPTEK Official Store](https://www.aliexpress.com/store/1105701619)

**China (Taobao)**

- Store: [鱼鹰光电工厂店](https://shop110742373.taobao.com/)

---

## Support

For technical questions or business inquiries:

- Technical support / product inquiry: <luyu@osptek.com>
- QQ technical group: **985881096**
- Website: <https://osptek.com/>
- Feel free to open an Issue in this repository if you have any questions

---

<p align="center"><sub>© 2026 OSPTEK · Materials in this repository are licensed under CC BY 4.0</sub></p>
