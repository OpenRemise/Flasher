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
/// \file   com_box.cpp
/// \author Vincent Hamp
/// \date   05/11/2024

#include "com_box.hpp"
#include <QHBoxLayout>
#include <QLabel>
#include <QSerialPortInfo>
#include "boards.hpp"
#include "update_ports_event_filter.hpp"

/// Create layout of various dropdown menus and a start/stop button
ComBox::ComBox(QWidget* parent) : QGroupBox{parent} {
  // Start/stop button
  QPixmap pixmap_off{":/light/play.svg"};
  QPixmap pixmap_on{":/light/stop.svg"};
  QIcon ico;
  ico.addPixmap(pixmap_off, QIcon::Normal, QIcon::Off);
  ico.addPixmap(pixmap_on, QIcon::Normal, QIcon::On);
  _start_stop_button->setIcon(ico);
  _start_stop_button->setText("Start");
  _start_stop_button->setCheckable(true);

  // Chips dropdown
  for (auto const& board : boards) _board_combobox->addItem(board);
  // _board_combobox->setCurrentIndex(_board_combobox->findText("auto"));
  _board_combobox->setToolTip("Target chip type");

  // Port dropdown
  _port_combobox->addItem("auto");
  // Install an event filter which updates list of serial ports every time the
  // dropdown is selected
  _port_combobox->installEventFilter(new UpdatePortsEventFilter{this});
  _port_combobox->setToolTip("Serial port device");

  // Baud dropdown
  _baud_combobox->addItem("auto");
  for (auto const& baud_rate : QSerialPortInfo::standardBaudRates())
    if (baud_rate <= 115200)
      _baud_combobox->addItem(QString::number(baud_rate));
  _baud_combobox->setToolTip(
    "Serial port baud rate used when flashing/reading");

  // Layout
  auto layout{new QHBoxLayout};
  // Workaround: top margin must be zero for the layout to be vertically
  // centered
  layout->setContentsMargins(11, 0, 11, 11);
  layout->addWidget(_start_stop_button);
  layout->addWidget(new QLabel{"Board"}, 0, Qt::AlignRight);
  layout->addWidget(_board_combobox);
  layout->addWidget(new QLabel{"Com"}, 0, Qt::AlignRight);
  layout->addWidget(_port_combobox);
  layout->addWidget(new QLabel{"Baud"}, 0, Qt::AlignRight);
  layout->addWidget(_baud_combobox);
  setLayout(layout);

  connect(_start_stop_button,
          &QPushButton::clicked,
          this,
          &ComBox::startStopButtonClicked);
}

/// Set binaries slot
///
/// \param  bins  Binaries
void ComBox::binaries(QVector<Bin> bins) { _bins = bins; }

/// Start/stop button slot
///
/// \param  start
void ComBox::startStopButtonClicked(bool start) {
  // Start thread
  if (start) {
    _start_stop_button->setText("Stop");

    QString const port{_port_combobox->currentText()};
    QString const baud{_baud_combobox->currentText()};

    _thread = new QThread;
    _esp_flasher = new EspFlasher{
      "esp32s3", port, baud, "no_reset", "no_reset", "", "", _bins};
    _esp_flasher->moveToThread(_thread);

    // Thread starts loader
    connect(_thread,
            &QThread::started,
            _esp_flasher,
            qOverload<>(&EspFlasher::flash));

    // When loader finishes, quit thread and delete loader
    connect(_esp_flasher, &EspFlasher::finished, _thread, &QThread::quit);
    connect(_esp_flasher,
            &EspFlasher::finished,
            _esp_flasher,
            &EspFlasher::deleteLater);

    // When thread finished, delete thread
    connect(_thread, &QThread::finished, _thread, &QThread::deleteLater);

    // Once thread is destroyed, reset button
    connect(_thread, &QThread::destroyed, [this] {
      _start_stop_button->setChecked(false);
      _start_stop_button->setText("Start");
    });

    _thread->start();
  }
  // Stop running thread
  else if (_thread->isRunning())
    _thread->requestInterruption();
}
