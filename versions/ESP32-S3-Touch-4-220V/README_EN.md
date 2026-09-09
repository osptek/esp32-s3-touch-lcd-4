<p align="left"><img alt="OSPTEK" src="./images/logo.png" width="200" /></p>

<h1 align="center">OSPTEK ESP32-S3-Touch-4-220V Touch Panel</h1>

<p align="center"><b>4" Touch · 220V Power · RS485 · Wi-Fi / BLE</b></p>

<p align="center">English | <a href="./README.md">简体中文</a> · <a href="../../README_EN.md">Family index</a></p>

<p align="center">
  <img alt="MCU: ESP32-S3" src="https://img.shields.io/badge/MCU-ESP32--S3-E7352C?style=flat-square" />
  <img alt="Display: 4 inch 480x480" src="https://img.shields.io/badge/Display-4%22_480%C3%97480-3498DB?style=flat-square" />
  <img alt="Power: 220V AC" src="https://img.shields.io/badge/Power-AC_85--265_V-27AE60?style=flat-square" />
  <img alt="RS485" src="https://img.shields.io/badge/Interface-RS485-F39C12?style=flat-square" />
</p>

<p align="center"><img alt="OSPTEK ESP32-S3-Touch-4-220V product image" src="./images/product.png" width="360" /></p>

## Contents

- [Overview](#overview)
- [Other SKUs in this series](#other-skus-in-this-series)
- [Features](#features)
- [Applications](#applications)
- [Specifications](#specifications)
- [Display](#display)
- [Hardware Resources](#hardware-resources)
- [Development Notes](#development-notes)
- [Examples](#examples)
- [Prebuilt Firmware](#prebuilt-firmware)
- [Repository Structure](#repository-structure)
- [Documentation](#documentation)
- [Where to Buy](#where-to-buy)
- [Support](#support)

---

## Overview

> 📌 This SKU is **ESP32-S3-Touch-4-220V**. Specs follow the user guides in `docs/`.

OSPTEK **ESP32-S3-Touch-4-220V** integrates a **4-inch RGB capacitive touch display**, an **ESP32-S3** (Wi-Fi / BLE) module, a **220V AC input power stage**, and an **RS485** interface—suited for smart-home control, industrial panels, networked devices, and HMI terminals.

## Features

- **4" square touch panel**: YDP395BT003-V4, 480×480, ST7701S + capacitive touch
- **ESP32-S3**: ESP32-S3-WROOM-1-N16R8, **16 MB Flash + 8 MB PSRAM**, Wi-Fi and BLE 5.0
- **220V power**: AC **85~265 V** in, DC **24 V** out
- **RS485** industrial communication
- **USB Type-C** for flash, debug, and power
- **Speaker** for audio alerts

## Applications

- Smart-home control terminals
- Industrial equipment control panels
- Smart power systems
- IoT gateways
- Automation HMI

## Specifications

### MCU & Memory

| Item | Specification |
| ---- | ------------- |
| Product | ESP32-S3-Touch-4-220V |
| Module | ESP32-S3-WROOM-1-N16R8 |
| Flash | 16 MB |
| PSRAM | 8 MB |
| Wireless | Wi-Fi, Bluetooth LE 5.0 |

### Display & Touch

| Item | Specification |
| ---- | ------------- |
| Panel module | YDP395BT003-V4 (~3.95") |
| Resolution | 480×480 |
| Driver IC | ST7701S |
| Display interface | RGB 18-bit |
| Touch | Capacitive (FT6336U, I²C) |
| Brightness | 350 cd/m² typ. |

Full module details are in [Display](#display) below.

### Power & Interfaces

| Item | Specification |
| ---- | ------------- |
| AC input | AC 85~265 V (terminal: 220V power in) |
| DC output | 24 V DC (terminal: DO / 24V out; module typ. 24 V / 410 mA) |
| RS485 | RS485A / RS485B |
| USB | Type-C (flash, debug, power) |
| Audio | Onboard speaker |

> ⚠ Mains AC is hazardous—do not work live. Confirm wiring before first power-up. Installation by qualified personnel is recommended. See the [User Guide](./docs/ESP32-S3-Touch-4-220V_User%20Guide260731.pdf).

## Display

This panel uses OSPTEK TFT module **YDP395BT003-V4**, driver IC **ST7701S**, connected via the LCD FPC; touch is capacitive (I²C, FT6336U).

### Module Specs (YDP395BT003-V4)

| Item | Specification |
| ---- | ------------- |
| Module | YDP395BT003-V4 |
| Size | 3.95" diagonal |
| Display mode | Normally Black |
| Resolution | 480 (H) RGB × 480 (V) |
| Pixel pitch | 153 μm × 153 μm |
| Active area | 71.86 × 70.18 mm |
| Module outline | 83.85 × 83.85 × 3.27 mm |
| Arrangement | RGB vertical stripe |
| Interface | RGB 18-bit (HSYNC / VSYNC / DE / DCLK); init via 3-wire SPI (SDA / SCL / CS) |
| Driver IC | ST7701S |
| Brightness | 350 cd/m² typ. (min. ~300) |
| Viewing angle | All view (~80° typ.) |
| Contrast | ~1000:1 typ. |
| Backlight | 8 white LEDs (~12 V / 40 mA typ.) |
| Operating temp. | −20 ℃ ~ +70 ℃ |
| Storage temp. | −30 ℃ ~ +80 ℃ |

Touch signals `TP_SCL` / `TP_SDA` / `TP_INT` / `TP_RESET` share the same FPC; supply typically 2.8–3.3 V.

### Display Documents

- [Panel datasheet YDP395BT003-V4 (PDF)](./docs/YDP395BT003-V4.pdf)
- [Assembly CAD (YDP395BT003-V4)](./docs/YDP395BT003-V4.dwg)
- [Driver IC ST7701S datasheet (PDF)](./docs/ST7701S_SPEC_V1.3.pdf)
- [Touch IC FT6336U datasheet (PDF)](./docs/FT6336U_DataSheet_V1.1.pdf)
- [Init sequence (text)](./docs/BOE3.95_480x480_ST7701S_init.txt)

## Hardware Resources

### Product & Power Board

<p align="center">
  <img alt="ESP32-S3-Touch-4-220V front" src="./images/product.png" width="280" />
  &nbsp;&nbsp;
  <img alt="Rear 220V power module and terminals" src="./images/product-back.png" width="360" />
</p>

### Board Layout

<p align="center"><img alt="YuYue LCD board layout" src="./images/board-layout.png" width="720" /></p>

Labeled resources include ESP32-S3-WROOM-1-N16R8, RS485, CH340K, DCDC, backlight driver, RTC, audio amp / speaker, USB Type-C, and more.

### Schematic Overview

<p align="center"><img alt="Schematic overview" src="./images/schematic-overview.jpg" width="720" /></p>

Full schematic PDFs:

- [YuYue LCD V1.0 schematic (PDF)](./docs/YuYue%20LCD_V1.0.pdf)
- [YuYue Power V0.8 schematic (PDF)](./docs/YuYue%20Power%20V0.8.pdf)

Full wiring and safety notes are in the PDF guides.

## Development Notes

1. Use **USB Type-C** to flash and debug.
2. **Verify the display (recommended)**: flash the prebuilt firmware—see [Prebuilt Firmware](#prebuilt-firmware).
3. **Application development**: build the [example](#examples) with ESP-IDF (same sources as Classic).

⚠ AC 220V input is hazardous; do not work on live wiring. Confirm wiring before first power-up.

Guides:

- [使用指南（中文 PDF）](./docs/ESP32-S3-TOUCH-4-220V_%E4%BD%BF%E7%94%A8%E6%8C%87%E5%8D%97260731.pdf)
- [User Guide (English PDF)](./docs/ESP32-S3-Touch-4-220V_User%20Guide260731.pdf)

Espressif:

- [Flash Download Tools (Windows)](https://www.espressif.com/en/support/download/other-tools)
- [ESP-IDF Get Started · ESP32-S3](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/)

## Examples

| Description | Path |
| ---- | ---- |
| Display bring-up / LVGL demo (RGB ST7701, 480×480, touch FT6336U) | [`examples/esp32s3-3.95-tft-480x480-rgb-st7701-bringup/`](./examples/esp32s3-3.95-tft-480x480-rgb-st7701-bringup/) |

> Same example sources as the Classic SKU (same panel and touch).

With ESP-IDF installed:

```bash
cd examples/esp32s3-3.95-tft-480x480-rgb-st7701-bringup
idf.py set-target esp32s3
idf.py build
idf.py -p <PORT> flash monitor
```

Dependencies are managed by `main/idf_component.yml` and fetched on first build.

## Prebuilt Firmware

| File | Address | Notes |
| ---- | -------- | ---- |
| [`firmware/esp32-s3-touch-lcd-4.bin`](./firmware/esp32-s3-touch-lcd-4.bin) | **`0x0`** | Merged image (bootloader + partition table + app) for the display example above |

Flash settings: chip **ESP32-S3**, Flash **16 MB**, **DIO**, **80 MHz**. Write the merged image from **`0x0`**.

### Option A: Flash Download Tools (Windows)

1. Open Espressif **[Flash Download Tools](https://www.espressif.com/en/support/download/other-tools)**.
2. `ChipType` **ESP32-S3**, `WorkMode` Develop, `LoadMode` UART.
3. Select [`firmware/esp32-s3-touch-lcd-4.bin`](./firmware/esp32-s3-touch-lcd-4.bin), address **`0x0`**.
4. SPI SPEED / MODE / FLASH SIZE: 80 MHz, DIO, 16 MB.
5. Choose the COM port and click **START**.

### Option B: esptool

```bash
esptool.py --chip esp32s3 -p <PORT> write_flash 0x0 firmware/esp32-s3-touch-lcd-4.bin
```

## Repository layout

```text
esp32-s3-touch-lcd-4/                                # repo root (nav: ../../README_EN.md)
└── versions/
    └── ESP32-S3-Touch-4-220V/                       # full materials for this SKU
        ├── README.md
        ├── README_EN.md
        ├── images/
        ├── docs/
        ├── firmware/
        └── examples/
```

## Documentation

- [使用指南（中文 PDF）](./docs/ESP32-S3-TOUCH-4-220V_%E4%BD%BF%E7%94%A8%E6%8C%87%E5%8D%97260731.pdf)
- [User Guide (English PDF)](./docs/ESP32-S3-Touch-4-220V_User%20Guide260731.pdf)
- [YuYue LCD V1.0 schematic (PDF)](./docs/YuYue%20LCD_V1.0.pdf)
- [YuYue Power V0.8 schematic (PDF)](./docs/YuYue%20Power%20V0.8.pdf)
- [Panel datasheet YDP395BT003-V4 (PDF)](./docs/YDP395BT003-V4.pdf)
- [Assembly CAD (YDP395BT003-V4)](./docs/YDP395BT003-V4.dwg)
- [Driver IC ST7701S datasheet (PDF)](./docs/ST7701S_SPEC_V1.3.pdf)
- [Touch IC FT6336U datasheet (PDF)](./docs/FT6336U_DataSheet_V1.1.pdf)
- [Init sequence (text)](./docs/BOE3.95_480x480_ST7701S_init.txt)

### Shared enclosure CAD

The 86-type enclosure (2-layer / 4-layer) is shared by every SKU. Files live in the repo-root `docs/` folder.

- [4-layer: `0302(1).dwg`](../../docs/86-enclosure-4-layer/0302(1).dwg)
- [4-layer: `WSD4寸-2d-1116.dwg`](../../docs/86-enclosure-4-layer/WSD4%E5%AF%B8-2d-1116.dwg)
- [2-layer: `3.95-01 PCB.dwg`](../../docs/86-enclosure-2-layer/3.95-01%20PCB.dwg)
- [2-layer: `增加开口结构外壳20250102.dwg`](../../docs/86-enclosure-2-layer/%E5%A2%9E%E5%8A%A0%E5%BC%80%E5%8F%A3%E7%BB%93%E6%9E%84%E5%A4%96%E5%A3%B320250102.dwg)
- [2-layer: `装配0718(1).DWG`](../../docs/86-enclosure-2-layer/%E8%A3%85%E9%85%8D0718(1).DWG)

### Espressif

- [ESP32-S3 product page](https://www.espressif.com/en/products/socs/esp32-s3)
- [ESP-IDF Programming Guide · ESP32-S3](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/index.html)

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

- Technical support / product inquiry: <luyu@osptek.com>
- QQ technical group: **985881096**
- Website: <https://osptek.com/>
- Feel free to open an Issue in this repository if you have any questions

---

<p align="center"><sub>© 2026 OSPTEK · Materials in this repository are licensed under CC BY 4.0</sub></p>
