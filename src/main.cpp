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

/// main
///
/// \file   main.cpp
/// \author Vincent Hamp
/// \date   05/11/2024

#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include "main_window.hpp"

int main(int argc, char* argv[]) {
  QCoreApplication::setApplicationName("OpenRemiseFlasher");
  QCoreApplication::setApplicationVersion(OPENREMISE_FLASHER_VERSION);

  // Create an application instance
  QApplication app{argc, argv};

  // Initialize resources
  Q_INIT_RESOURCE(qtbreeze_stylesheets);

  /// \bug Adding GlacialIndifference does not work on Windows
  if (QFontDatabase::addApplicationFont(
        ":/fonts/GlacialIndifference-Regular.otf") == -1)
    return -1;
  QFont font{"GlacialIndifference"};
  font.setPointSize(10);
  app.setFont(font);

  // Apply breeze stylesheet
  QFile file{":/light/stylesheet.qss"};
  file.open(QFile::ReadOnly | QFile::Text);
  QTextStream stream{&file};
  app.setStyleSheet(stream.readAll());

  MainWindow w{};
  w.show();
  return app.exec();
}
