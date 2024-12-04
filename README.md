# Flasher

[![build](https://github.com/OpenRemise/Flasher/actions/workflows/build.yml/badge.svg)](https://github.com/OpenRemise/Flasher/actions/workflows/build.yml) [![license](https://img.shields.io/github/license/OpenRemise/Flasher)](https://github.com/OpenRemise/Flasher/raw/master/LICENSE)

<a href="https://openremise.at">
<picture>
  <source media="(prefers-color-scheme: dark)" srcset="https://github.com/OpenRemise/.github/raw/master/data/icons/icon_dark.svg">
  <img src="https://github.com/OpenRemise/.github/raw/master/data/icons/icon_light.svg" width="20%" align="right">
</picture>
</a>

Flasher is a GUI wrapper around [esp-serial-flasher](https://github.com/espressif/esp-serial-flasher), a portable C library for flashing of [Espressif](https://www.espressif.com/) SoCs. It is used to load the [OpenRemise](https://openremise.at/) [Firmware](https://github.com/OpenRemise/Firmware) onto any of the following supported boards:
- [S3Main](https://github.com/OpenRemise/S3Main)

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#features">Features</a></li>
    <li><a href="#options">Options</a></li>
      <ul>
        <li><a href="#board">Board</a></li>
        <li><a href="#serial-port">Serial Port</a></li>
        <li><a href="#baud-rate">Baud Rate</a></li>
      </ul>
    <li><a href="#usage">Usage</a></li>
  </ol>
</details>

## Features
- **No dependencies**
- Flash [OpenRemise](https://openremise.at/) boards from either
  - [Latest release](https://github.com/OpenRemise/Firmware/releases/latest)
  - or local .zip archive
- Pre-built Windows and Linux executables

## Options
At the bottom of the GUI there are a few settings available via drop down menus.  
![options](data/images/options.png)

### Board
The board type onto which the firmware should be flashed. Currently only `S3Main` is supported.

### Serial Port
The serial port used for flashing. Normally the port should be detected automatically, so it is recommended to leave the setting on `auto`.

### Baud Rate
The default Flasher baud rate is `115200`. Slower rates may be set using the drop down. It is **recommend** to only set the baud rate if you're experiencing transmission errors during flashing. If left at default Flasher tries to change the baud rate to `460800` when running to considerably reduce flash times.

## Usage
At this point we refer you to the [Getting Started](https://openremise.at/page_getting_started.html#section_getting_started_install) section on [openremise.at](https://openremise.at). There you will find extensive information on how to get a board up and running using Flasher.