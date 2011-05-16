/*
  Logger - a simple logger for Qt-based applications.
  Copyright (C) 2011 Bjørn Øivind Bjørnsen

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
  */

#ifndef SETUP_H
#define SETUP_H

#include <QDir>
#include <QFileInfoList>
#include <QCoreApplication>
#include <QtMsgHandler>
#include "log/logger.h"

void setupTests()
{
    // set up application metadata for the different systems that depend on it
    QCoreApplication::setOrganizationName("Logger");
    QCoreApplication::setOrganizationDomain("https://github.com/bjorn-oivind");
    QCoreApplication::setApplicationName("Logger tests");
    QCoreApplication::setApplicationVersion("1.0");

    // this is hack to preserve the QTest message handler
    QtMsgHandler handler = qInstallMsgHandler(0);
    Logger::instance();
    qInstallMsgHandler(handler);
}

void teardownTests()
{
    Logger::instance()->close();
}


#endif // SETUP_H
