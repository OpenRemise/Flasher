// Copyright (C) 2025 Vincent Hamp
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
/// \file   main_window.cpp
/// \author Vincent Hamp
/// \date   05/11/2024

#include "main_window.hpp"
#include <JlCompress.h>
#include <QApplication>
#include <QDirIterator>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMessageBox>
#include <QNetworkReply>
#include <QTemporaryDir>
#include <QVBoxLayout>

/// Add menu and toolbar
MainWindow::MainWindow() {
  // Initial size
  resize(480, 640);

  // Toolbar should be fixed
  _toolbar->setContextMenuPolicy(Qt::PreventContextMenu);
  _toolbar->setFloatable(false);
  _toolbar->setMovable(false);

  // Add network drive action
  QIcon const network_drive_icon{":/light/network_drive.svg"};
  auto network_drive_act{
    new QAction{network_drive_icon, "Download latest firmware", this}};
  connect(network_drive_act,
          &QAction::triggered,
          this,
          qOverload<>(&MainWindow::addArchiveFromNetworkDrive));
  _toolbar->addAction(network_drive_act);

  // Add hard drive action
  QIcon const hard_drive_icon{":/light/hard_drive.svg"};
  auto hard_drive_act{new QAction{hard_drive_icon, "Open firmware", this}};
  connect(hard_drive_act,
          &QAction::triggered,
          this,
          qOverload<>(&MainWindow::addArchiveFromHardDrive));
  _toolbar->addAction(hard_drive_act);

  // Help action
  QIcon const about_icon{":/light/dialog_help.svg"};
  auto about_act{new QAction{about_icon, "&About", this}};
  connect(about_act, &QAction::triggered, this, &MainWindow::about);
  _toolbar->addAction(about_act);

  // Layout
  auto central_widget{new QWidget};
  auto layout{new QVBoxLayout};
  layout->addWidget(_log);
  layout->addWidget(_com_box);
  layout->setStretchFactor(_log, 1);
  layout->setStretchFactor(_com_box, 0);
  central_widget->setLayout(layout);
  setCentralWidget(central_widget);

  connect(this, &MainWindow::binaries, _com_box, &ComBox::binaries);

  // Workaround for WIN32, otherwise it throws self-signed and untrusted at us
  connect(_network_manager,
          &QNetworkAccessManager::sslErrors,
          [](QNetworkReply* reply, QList<QSslError> const&) {
            reply->ignoreSslErrors();
          });
}

/// Open file dialog, get .zip archive path
void MainWindow::addArchiveFromHardDrive() {
  if (auto const ar_path{
        QFileDialog::getOpenFileName(this,
                                     "Open firmware",
                                     "",
                                     "Zip archive (*.zip)",
                                     nullptr,
                                     QFileDialog::DontUseCustomDirectoryIcons)};
      QFileInfo{ar_path}.exists()) {
    qInfo().noquote() << "Opening" << QFileInfo{ar_path}.fileName();
    addArchiveFromHardDrive(ar_path);
  }
}

/// Read archive and gather binaries
///
/// \param  ar_path Zip archive path
void MainWindow::addArchiveFromHardDrive(QString ar_path) {
  // Create temporary directory
  QTemporaryDir temp_dir;
  if (!temp_dir.isValid()) {
    qCritical().noquote() << temp_dir.errorString();
    return;
  }

  // Extract files to temporary directory, then add it
  JlCompress::extractDir(ar_path, temp_dir.path());

  QDirIterator it{temp_dir.path(),
                  QStringList() << "flasher_args.json",
                  QDir::Files,
                  QDirIterator::Subdirectories};
  if (!it.hasNext()) {
    qCritical() << "No OpenRemise firmware found";
    return;
  }
  auto const json_path{it.next()};

  // Gather binaries
  QVector<Bin> bins{};
  QFile json{json_path};
  json.open(QIODevice::ReadOnly | QIODevice::Text);
  QJsonDocument const doc{QJsonDocument::fromJson(json.readAll())};
  QJsonObject const flash_files{doc["flash_files"].toObject()};
  for (auto const& offset : flash_files.keys()) {
    auto const bin_path{QFileInfo{json_path}.dir().filePath(
      flash_files.value(offset).toString())};
    QFile bytes{bin_path};
    bytes.open(QIODevice::ReadOnly);
    bins.push_back(
      {.offset = offset.toUInt(nullptr, 0), .bytes = bytes.readAll()});
  }

  emit binaries(bins);
}

/// Query GitHub REST API for latest release of firmware
void MainWindow::addArchiveFromNetworkDrive() {
  auto const reply{
    _network_manager->get(QNetworkRequest{QUrl{OPENREMISE_FIRMWARE_URL}})};

  connect(reply, &QNetworkReply::finished, this, [this, reply] {
    if (reply->error()) {
      qCritical().noquote() << reply->errorString();
      return;
    }

    // Qt's JSON interface is beyond my comprehension. Just don't touch this.
    // Just... don't. Don't change a const, don't change the assign... just keep
    // it the way it is.
    QJsonDocument doc{QJsonDocument::fromJson(reply->readAll())};
    QJsonArray tmp{doc["assets"].toArray()};
    auto assets = tmp.takeAt(0).toArray();
    for (auto asset : assets)
      if (auto const browser_download_url{
            asset.toObject()["browser_download_url"].toString()};
          browser_download_url.endsWith(".zip", Qt::CaseInsensitive)) {
        qInfo().noquote() << "Downloading"
                          << QFileInfo{browser_download_url}.fileName();
        return addArchiveFromNetworkDrive(browser_download_url);
      }
  });
}

/// Download latest firmware release and gather binaries
///
/// \param  browser_download_url  URL of latest firmware release
void MainWindow::addArchiveFromNetworkDrive(QString browser_download_url) {
  auto const reply{
    _network_manager->get(QNetworkRequest{QUrl{browser_download_url}})};

  // Show download progress
  connect(reply, &QNetworkReply::downloadProgress, [](qint64 ist, qint64 max) {
    auto const pct{100.0 * static_cast<double>(ist) / static_cast<double>(max)};
    qDebug().nospace() << "Downloading " << ist / 1024 << "kB... ("
                       << static_cast<int>(pct) << "%)";
  });

  connect(reply, &QNetworkReply::finished, this, [=, this] {
    qInfo().noquote() << "Done";

    // Create temporary directory
    QTemporaryDir temp_dir;
    if (!temp_dir.isValid()) {
      qCritical().noquote() << temp_dir.errorString();
      return;
    }

    // Download .zip archive
    QFile file(temp_dir.filePath(QFileInfo{browser_download_url}.fileName()));
    file.open(QIODevice::WriteOnly);
    file.write(reply->readAll());
    file.close();
    addArchiveFromHardDrive(QFileInfo{file}.absoluteFilePath());
  });
}

/// About message box
void MainWindow::about() {
  QMessageBox about;
  about.setWindowTitle("About " + QCoreApplication::applicationName());
  about.setTextFormat(Qt::RichText);
  about.setDetailedText("@Version\n" + QCoreApplication::applicationVersion() +
                        "\n\n" +      //
                        "@GitHub\n" + //
                        "github.com/openremise/Flasher");
  about.setStandardButtons(QMessageBox::Ok);
  about.setIconPixmap(QPixmap{":/images/logo.svg"}.scaledToWidth(200));
  about.show();
  about.exec();
}
