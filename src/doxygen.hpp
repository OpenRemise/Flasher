// Copyright (C) 2024 Vincent Hamp
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

/// Documentation
///
/// \file   doxygen.hpp
/// \author Vincent Hamp
/// \date   07/11/2024

/// \details
/// Welcome to the OpenRemise [Flasher](https://github.com/OpenRemise/Flasher)
/// documentation, the tool for loading the OpenRemise
/// [Firmware](https://github.com/OpenRemise/Firmware) onto any of the supported
/// boards.
///
/// \note
/// This documentation is intended for developers. General information on usage
/// can be found on the main page at [openremise.at](https://openremise.at).
///
// clang-format off
/// \mainpage Introduction
/// | Getting Started                                                                                                                            | API Reference                                                                                                                                                                                              |
/// | ------------------------------------------------------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------------------------------------------------------- |
/// | [![](stopwatch.svg)](page_getting_started.html)                                                                                            | [![](api.svg)](page_api_reference.html)                                                                                                   |
/// | <div style="max-width:200px">New to the codebase? Check out the \ref page_getting_started guide to set up a development environment.</div> | <div style="max-width:200px">The \ref page_api_reference contains a detailed description of the data flow of each class and widget.</div> |
///
/// <div class="section_buttons">
/// | Next                      |
/// | ------------------------: |
/// | \ref page_getting_started |
/// </div>
// clang-format on

/// \page page_getting_started Getting Started
/// \details \tableofcontents
/// In this chapter, we set up a development environment that allows us to build
/// the Flasher executable and it's documentation.
///
/// We recommend either an [Arch](https://archlinux.org) (e.g.
/// [Garuda](https://garudalinux.org) or [Manjaro](https://manjaro.org)) or
/// [Ubuntu](https://ubuntu.com) based distribution, so all of the following
/// steps refer to those.
///
/// \section section_prerequisites Prerequisites
/// In order to start developing Flasher, we need to meet quite a few
/// prerequisites. Fortunately, all of them can be obtained directly from the
/// package manager. But before we do that, let's bring our system up to date.
/// <div class="tabbed">
/// - <b class="tab-title">Arch</b>
///   ```sh
///   sudo pacman -Syu --noconfirm
///   ```
/// - <b class="tab-title">Ubuntu 24.04</b>
///   ```sh
///   sudo apt update -y
///   sudo apt upgrade -y
///   ```
/// </div>
///
/// Without going into detail about each individual dependency, the most
/// important ones are CMake, a build system, GCC, a compiler, Qt6, a
/// cross-platform GUI framework, Ninja, a build tool, Doxygen, a documentation
/// generator, and Graphviz, a graph visualization software.
///
/// - [CMake](https://cmake.org) ( >= 3.25 )
/// - [GCC](https://gcc.gnu.org) ( >= 13.2.0 )
/// - [Qt6](https://github.com/espressif/esp-idf) ( >= 6.4.3 )
/// - [Ninja](https://ninja-build.org) ( >= 1.10.2 )
/// - Optional
///   - for building documentation
///     - [Doxygen](https://www.doxygen.nl/index.html) ( >= 1.12.0 )
///     - [Graphviz](https://graphviz.org) ( >= 12.1.1 )
///
// clang-format off
/// \page page_getting_started Getting Started
/// \details \tableofcontents
/// <div class="tabbed">
/// - <b class="tab-title">Arch</b>
///   ```sh
///   sudo pacman -S --noconfirm cmake doxygen gcc git graphviz make ninja qt6-5compat qt6-base qt6-serialport qt6-svg
///   ```
/// - <b class="tab-title">Ubuntu 24.04</b>
///   ```sh
///   sudo apt-get install -y build-essential clang-format cmake doxygen git graphviz libgl1-mesa-dev libglx-dev libqt6svg6* ninja-build qt6-5compat-dev qt6-base-dev qt6-serialport-dev zlib1g-dev
///   ```
/// </div>
// clang-format on
/// \page page_getting_started Getting Started
/// \details \tableofcontents
///
/// \section section_clone Clone
/// The Flasher source code is hosted on GitHub. We can use either SSH or HTTP
/// to clone the [repository](https://github.com/OpenRemise/Flasher). Using `git
/// clone` without any additional arguments will clone the latest version of the
/// master branch to the current working directory. After that, we can change
/// into the `Flasher` directory we've just created. <div class="tabbed">
/// - <b class="tab-title">SSH</b>
///   ```sh
///   git clone git@github.com:OpenRemise/Flasher.git
///   cd Flasher
///   ```
/// - <b class="tab-title">HTTPS</b>
///   ```sh
///   git clone https://github.com/OpenRemise/Flasher.git
///   cd Flasher
///   ```
/// </div>
///
/// \section section_build Build
/// The build supports both debug and release configurations. Apart from the
/// obvious optimizations the main difference is that for release
/// configurations, Qt6 gets **built from source**.
///
/// \warning
/// Depending on your internet connection and machine downloading and compiling
/// Qt6 may **take several minutes**.
///
/// <div class="tabbed">
/// - <b class="tab-title">Debug</b>
///   ```sh
///   cmake --preset "Debug"
///   ```
/// - <b class="tab-title">Release</b>
///   ```sh
///   cmake --preset "Release"
///   ```
/// </div>
///
/// After the CMake configure stage has been successful, we just need to run the
/// actual build stage.
/// ```sh
/// cmake --build build --parallel
/// ```
///
/// \section section_doc Doc
/// If Doxygen was found during CMake's configuration phase, the `FlasherDocs`
/// target can be built to create the documentation.
/// ```sh
/// cmake --build build --target FlasherDocs
/// ```
///
/// <div class="section_buttons">
/// | Previous   | Next                    |
/// | :--------- | ----------------------: |
/// | \ref index | \ref page_api_reference |
/// </div>

/// \page page_api_reference API reference
/// \details \tableofcontents
/// The entire GUI is split into just three sections.
/// - Toolbar for downloading/opening a firmware
/// - %Log displaying messages
/// - %ComBox which is a bunch of settings and a start button
///
/// \image html gui.png "GUI"
///
/// \section section_main_window MainWindow
/// \copydetails MainWindow
///
/// \section section_log Log
/// \copydetails Log
///
/// \section section_com_box ComBox
/// \copydetails ComBox
///
///
/// <div class="section_buttons">
/// | Previous                  |
/// | :------------------------ |
/// | \ref page_getting_started |
/// </div>