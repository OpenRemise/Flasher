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

/// GUI options
///
/// \file   com_box.hpp
/// \author Vincent Hamp
/// \date   05/11/2024

#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QThread>
#include <esp_flasher/esp_flasher.hpp>

/// Bottom part GUI widget which displays serial port options
///
/// ComBox inherits a [QGroupBox](https://doc.qt.io/qt-6/qgroupbox.html) widget
/// which displays a couple of dropdown menus to choose various serial port
/// options. Apart from that, there is a start/stop button to start the writing
/// process.
class ComBox : public QGroupBox {
  Q_OBJECT

public:
  ComBox(QWidget* parent = nullptr);

public slots:
  void binaries(QVector<Bin> bins);

private slots:
  void startStopButtonClicked(bool start);

private:
  QComboBox* _board_combobox{new QComboBox};
  QComboBox* _port_combobox{new QComboBox};
  QComboBox* _baud_combobox{new QComboBox};
  QPushButton* _start_stop_button{new QPushButton};
  QVector<Bin> _bins{};
  QThread* _thread{};
  EspFlasher* _esp_flasher{};
};