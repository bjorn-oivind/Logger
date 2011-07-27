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

/**
  @file

  Implementation of Logger.
  */
#include "logger.h"

// for Indent
#include "debug.h"

#include <iostream>

#include <QMutexLocker>
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QTime>

Logger* Logger::_instance = 0;
QMutex Logger::m_creationalMutex;
QMutex Logger::m_operationalMutex;
#ifdef Q_OS_LINUX
//                          Grey   White  Brown    Red
QString Logger::col[4] = { "01;30", "1", "00;33", "01;31" };
#endif

Logger* Logger::instance() throw()
{
    QMutexLocker locker(&m_creationalMutex);
    if(_instance == 0) {
        _instance = new Logger();
    }
    return _instance;
}

void Logger::close()
{
    delete _instance;
    _instance = 0;
}

void Logger::log(LogLevel level, QString message) throw()
{
    if(!logFile.isOpen())
        return;

    QMutexLocker locker(&m_operationalMutex);

    // do we bother with opening the stream and logging?
    if(level < m_logThreshold)
        return;

    // shall we limit the logfile?
    if(m_logLimit) {
        if(m_linesLogged >= m_logLimit) {
            // truncate the log file
            if(!logFile.resize(0))
                return;
            m_linesLogged = 0;
        }
    }

    ++m_linesLogged;

    QTextStream stream(&logFile);

    // print timestamp
    QString output = "[" + QTime::currentTime().toString() + "] ";
    switch(level) {
    case DEBUG:
        output += "[DEBUG]    " + QString(Debug::Indent::getIndent(), ' ');
        break;
    case INFO:
        output += "[INFO]     ";
        break;
    case WARNING:
        output += "[WARNING]  ";
        break;
    case CRITICAL:
        output += "[CRITICAL] ";
        break;
    case NONE:
        // don't log, fall through
    default:
        break;
    }

    output += message + "\n";
    stream << output;
#ifdef Q_OS_LINUX
    if(!qgetenv("LOG_COLOR").isEmpty()) {
        output.prepend("\x1b[" + col[level] + "m");
        output.append("\x1b[00;39m");
    }
#endif
    if(m_logToConsole)
        std::cerr << qPrintable(output);
}

void Logger::setLogPath(QString dir, QString filename)
{
    QMutexLocker locker(&m_operationalMutex);

    QDir logDir;
    logDir.setPath(dir);
    if(!logDir.exists())
        logDir.mkpath(dir);

    if(logFile.isOpen())
        logFile.close();

    logFile.setFileName(dir + QDir::separator() + filename);
    logFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);

    // store the settings.
    QSettings settings;

    settings.setValue("Log/log_path", dir);
    settings.setValue("Log/log_filename", filename);
}

QString Logger::logPath() const
{
    QSettings settings;
    return settings.value("Log/log_path").toString();
}

QString Logger::logFilename() const
{
    QSettings settings;
    return settings.value("Log/log_filename").toString();
}

void Logger::setLogThreshold(LogLevel level)
{
    m_logThreshold = level;

    // store the setting
    QSettings s;
    QVariant value;

    switch(m_logThreshold) {
    case DEBUG:
        value = "DEBUG";
        break;
    case INFO:
        value = "INFO";
        break;
    case WARNING:
        value = "WARNING";
        break;
    case CRITICAL:
        value = "CRITICAL";
        break;
    case NONE:
        value = "NONE";
        break;
    }

    s.setValue("Log/log_threshold", value);
}

LogLevel Logger::logThreshold() const
{
    return m_logThreshold;
}

void Logger::setLogToConsole(bool enabled)
{
    m_logToConsole = enabled;
}

bool Logger::logToConsole() const
{
    return m_logToConsole;
}

void Logger::setLogLimit(int numLines)
{
    m_logLimit = numLines;
}

int Logger::logLimit() const
{
    return m_logLimit;
}

void Logger::logMessageHandler(QtMsgType type, const char *msg)
{
    switch(type)
    {
    case QtDebugMsg:
        instance()->log(DEBUG, QString(msg));
        break;

    case QtWarningMsg:
        instance()->log(WARNING, QString(msg));
        break;

    case QtCriticalMsg:
        instance()->log(CRITICAL, QString(msg));
        break;

    case QtFatalMsg:
        instance()->log(CRITICAL, QString(msg));
        // Fatal error, kill the application
        QCoreApplication::quit();
        break;
    }
}

Logger::Logger() throw()
{
    // open file(s) for logging, set logging threshold
    QSettings settings;
    QString path;
    QString filename;
    QString threshold;
    // log to console by default
    m_logToConsole = true;
    // do not limit the logfile by default
    m_logLimit = 0;
    m_linesLogged = 0;
    m_logThreshold = NONE;
    // by default the path is a dot-folder under the users home directory.
    path = settings.value("Log/log_path", QVariant(QString("%1%2.%3").arg(QDir::homePath(), QDir::separator(), QCoreApplication::applicationName()))).toString();
    // default filename is <application name>.log
    filename = settings.value("Log/log_filename", QVariant(QString("%1.log").arg(QCoreApplication::applicationName()))).toString();
    // default threshold is WARNING
    threshold = settings.value("Log/log_threshold", "WARNING").toString();

    setLogPath(path, filename);

    // set log threshold
    QString comp[5] = { "DEBUG", "INFO", "WARNING", "CRITICAL", "NONE" };
    for(int i = 0; i < 5; i++) {
        if(threshold == comp[i]) {
            m_logThreshold = (LogLevel)i;
            break;
        }
    }

    // Setup the qMsgHandler
    oldHandler = qInstallMsgHandler(Logger::logMessageHandler);
}

Logger::~Logger() throw()
{
    if(logFile.isOpen())
        logFile.close();

    qInstallMsgHandler(oldHandler);
}

