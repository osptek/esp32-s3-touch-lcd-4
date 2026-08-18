<p align="left"><img alt="OSPTEK" src="./images/logo.png" width="200" /></p>

<h1 align="center">OSPTEK ESP32-S3-Touch-4-Pro Touch Panel</h1>

<p align="center"><b>86-Type Panel · Touch UI · RS485 · Onboard Audio</b></p>

<p align="center">English | <a href="./README.md">简体中文</a> · <a href="../../README_EN.md">Family index</a></p>

<p align="center">
  <img alt="MCU: ESP32-S3" src="https://img.shields.io/badge/MCU-ESP32--S3-E7352C?style=flat-square" />
  <img alt="Display: 4 inch 480x480" src="https://img.shields.io/badge/Display-4%22_480%C3%97480-3498DB?style=flat-square" />
  <img alt="Audio: ES8311" src="https://img.shields.io/badge/Audio-ES8311_%2B_NS4150B-9B59B6?style=flat-square" />
  <img alt="RS485" src="https://img.shields.io/badge/Interface-RS485-F39C12?style=flat-square" />
</p>

<p align="center"><img alt="OSPTEK ESP32-S3-Touch-4-Pro product image" src="./images/product.png" width="420" /></p>

## Contents

- [Overview](#overview)
- [Other SKUs in this series](#other-skus-in-this-series)
- [Features](#features)
- [Specifications](#specifications)
- [Display](#display)
- [Hardware Resources](#hardware-resources)
- [Repository Structure](#repository-structure)
- [Documentation](#documentation)
- [Where to Buy](#where-to-buy)
- [Support](#support)

---

## Overview

> 📌 This page is **ESP32-S3-Touch-4-Pro** (schematic: `ESP32-S3-Touch-Pro_V1.1`).

OSPTEK **ESP32-S3-Touch-4-Pro** adds an onboard audio path (**ES8311**, amp **NS4150B**, MIC / SPEAKER) on top of the Classic feature set.

Module: **ESP32-S3-WROOM-1-N16R8**, with **RS485**, USB Type-C, I²C, and more.

## Features

- **4" square touch panel**: YDP395B003-V4, 480×480, ST7701S + capacitive touch
- **ESP32-S3**: Wi-Fi + BLE 5, 16 MB Flash + 8 MB PSRAM
- **Pro audio**: ES8311 + NS4150B, microphone / speaker
- **RS485** onboard (THVD1406DR)
- **Debug**: CH340K USB-UART
- **Isolated I/O**: optocoupler digital I/O; PCA9557 I²C expander

## Specifications

### MCU & Memory

| Item | Specification |
| ---- | ------------- |
| Product | ESP32-S3-Touch-4-Pro |
| Board / schematic | ESP32-S3-Touch-Pro V1.1 |
| Module | ESP32-S3-WROOM-1-N16R8 |
| Flash | 16 MB |
| PSRAM | 8 MB |
| Wireless | 2.4 GHz Wi-Fi, Bluetooth LE 5 |

### Display & Touch

| Item | Specification |
| ---- | ------------- |
| Panel module | YDP395B003-V4 (~3.95") |
| Resolution | 480×480 |
| Driver IC | ST7701S |
| Display interface | RGB 18-bit |
| Touch | Capacitive (FT5x06-compatible, I²C) |
| Brightness | 350 cd/m² typ. |

Full module details are in [Display](#display) below.

### Audio (Pro)

| Item | Specification |
| ---- | ------------- |
| Codec | ES8311 |
| Amplifier | NS4150B |
| I/O | MIC, SPEAKER |

### Interfaces

| Item | Specification |
| ---- | ------------- |
| USB | Type-C |
| USB-UART | CH340K |
| RS485 | THVD1406DR |
| Sensor / expand | I²C (incl. PCA9557) |
| Isolated I/O | Optocoupler digital I/O |

Pinout and power tree: see the [schematic](./docs/ESP32-S3-Touch-Pro_V1.1.pdf).

## Display

This panel uses OSPTEK TFT module **YDP395B003-V4**, driver IC **ST7701S**, connected via the LCD FPC; touch is capacitive (I²C, FT5x06-compatible).

### Module Specs (YDP395B003-V4)

| Item | Specification |
| ---- | ------------- |
| Module | YDP395B003-V4 |
| Size | 3.95" diagonal |
| Display mode | Normally Black |
| Resolution | 480 (H) RGB × 480 (V) |
| Pixel pitch | 153 μm × 153 μm |
| Active area | 71.86 × 70.18 mm |
| Module outline | 74.66 × 76.54 × 2.06 mm |
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

- [Panel datasheet YDP395B003-V4 (PDF)](./docs/YDP_395_B003_V4_d3e49044f9.pdf)
- [ST7701S datasheet (PDF)](./docs/ST_7701_S_SPEC_V1_3_f82b940377.pdf)

## Hardware Resources

### Appearance

<p align="center">
  <img alt="ESP32-S3-Touch-4-Pro front" src="./images/product-front.png" width="320" />
  &nbsp;&nbsp;
  <img alt="ESP32-S3-Touch-4-Pro back" src="./images/product-back.png" width="320" />
</p>

### Onboard Resources

| Resource | Notes |
| -------- | ----- |
| ESP32-S3-WROOM-1-N16R8 | Wi-Fi + BLE MCU module |
| CH340K | USB-UART |
| THVD1406DR | RS485 transceiver |
| PCA9557 | I²C I/O expander |
| ES8311 + NS4150 | Audio codec / amp |
| Optocouplers | Isolated digital I/O |
| USB Type-C | Power / flash |
| Terminal block | External power and bus |

### Board Layout

<p align="center"><img alt="ESP32-S3-Touch-4-Pro board layout" src="./images/board-layout.png" width="720" /></p>

For a fuller write-up, see the [product manual](./docs/ESP32-S3-Touch-Pro%204%E5%AF%B8WiFi%E4%B8%B2%E5%8F%A3%E5%B1%8F20260604.pdf).

### Schematic

<p align="center"><img alt="ESP32-S3-Touch-4-Pro schematic overview" src="./images/schematic-overview.jpg" width="720" /></p>

Full PDF:

- [ESP32-S3-Touch-Pro V1.1 schematic (PDF)](./docs/ESP32-S3-Touch-Pro_V1.1.pdf)

## Repository layout

```text
esp32-s3-touch-lcd-4/                                # repo root (nav: ../../README_EN.md)
└── versions/
    └── ESP32-S3-Touch-4-Pro/                        # full materials for this SKU
        ├── README.md
        ├── README_EN.md
        ├── images/
        └── docs/
```

## Documentation

- [Product manual (PDF)](./docs/ESP32-S3-Touch-Pro%204%E5%AF%B8WiFi%E4%B8%B2%E5%8F%A3%E5%B1%8F20260604.pdf)
- [ESP32-S3-Touch-Pro V1.1 schematic (PDF)](./docs/ESP32-S3-Touch-Pro_V1.1.pdf)
- [Panel datasheet YDP395B003-V4 (PDF)](./docs/YDP_395_B003_V4_d3e49044f9.pdf)
- [ST7701S datasheet (PDF)](./docs/ST_7701_S_SPEC_V1_3_f82b940377.pdf)
- [ESP32-S3-Touch-4-Classic](../ESP32-S3-Touch-4-Classic/)

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
