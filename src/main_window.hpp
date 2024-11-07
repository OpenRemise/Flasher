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

/// GUI main window
///
/// \file   main_window.hpp
/// \author Vincent Hamp
/// \date   05/11/2024

#pragma once

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QToolBar>
#include "com_box.hpp"
#include "log.hpp"

/// Main window
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();

private slots:
  void about();

signals:
  void binaries(QVector<Bin> bins);

private:
  void addArchiveFromHardDrive();
  void addArchiveFromHardDrive(QString ar_path);
  void addArchiveFromNetworkDrive();
  void addArchiveFromNetworkDrive(QString browser_download_url);

  QToolBar* _toolbar{addToolBar("")};
  QNetworkAccessManager* _network_manager{new QNetworkAccessManager};
  ComBox* _com_box{new ComBox};
  Log* _log{new Log};
};
