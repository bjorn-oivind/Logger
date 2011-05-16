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

  Declaration of LogSingleton and the LogLevel enum.
  */

#ifndef LOGGER_H
#define LOGGER_H

#include <QMutex>
#include <QString>
#include <QFile>
#include <QtMsgHandler>
#include <QtDebug>

#include "export.h"

/**
  A simple hierarchy of levels used when logging.
  */
enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    CRITICAL,
    NONE,
};

/**
  A simple logging singleton. For an explanation on the singleton pattern, see wikipedia
  or "Design Patterns - Elements of Reusable Object-Oriented Software" by Eirch Gamma et al.
  This class is responsible for logging to the console (unless QT_NO_DEBUG is defined)
  and a file on the harddrive.
  It uses a set of different levels of importance for each log message via the LogLevel
  enum. By default it only logs WARNING and CRITICAL messages. This can be overridden
  through a setting. This is read via QSettings, so it is in the registry in windows,
  in a file on Linux or in the Mac-equivalent of the registry on OS X.
  */
class LOGGER_EXPORT Logger
{
public:
    /**
      Returns an instance of the LogSingleton.
      This uses lazy initialisation, so the specific instance is not created
      until the first time someone calls instance().
      @return a pointer to the LogSingleton instance.
      */
    static Logger* instance() throw();
    /**
      Deletes the instance created by instance().
      @note that if instance() is called after close(), a new instance
      will be created.
      */
    void close();
    /**
      Prints a log message to the logfile.
      The LogLevel given must be greater or equal to the log threshold
      for the message to be logged.
      The log message is written to the logfile, provided that the file could be
      opened in the constructor. Console output is enabled unless QT_NO_DEBUG is
      defined or logging to console has been disabled via setLogToConsole().
      @param level the priority of the log message.
      @param message the message to log.
      @see setConsoleLogging()
      */
    void log(LogLevel level, QString message) throw();

    /**
      Sets and stores the path and filename of the log file.
      By default, the log file will be named the same as the
      application (application-name.log) and will be placed in
      a folder under the users home directory named after the
      application prefixed with a dot (.). This is in accordance
      to the usual unix-style of doings things.
      This function will re-open the logfile under the given
      path and filename, and log to this file instead of any
      previous file set. If no log file could be opened under
      the path and filename specified, logging to file will
      be disabled.
      @param dir the path to the directory to log file will be
      created in.
      @param filename the filename the log file will be created
      under.
      @note this function will store the path and filename using
      QSettings, so the settings will be saved for later runs of
      the program.
      */
    void setLogPath(QString dir, QString filename);
    /**
      Returns the path used for the log file.
      @see setLogPath()
      @returns the path the log file is placed under.
      */
    QString logPath() const;
    /**
      Returns the filename used by the log file.
      @see setLogPath()
      @returns the filename the log file is using.
      */
    QString logFilename() const;

    /**
      Sets and stores the log threshold the logger will use to determine
      if it should log a given message.
      A log threshold of DEBUG will log everything, while a threshold of
      NONE will (surprisingly enough) log nothing.
      @see LogLevel
      @param level the log threshold to use when logging.
      @note this function will store the log threshold using QSettings, so
      the setting will be saved for later runs of the program.
      */
    void setLogThreshold(LogLevel level);
    /**
      Returns the log threshold the logger currently uses.
      The log threshold is the filter that defines how much the logger
      class will log to file. A log threshold of DEBUG will log everything,
      while a threshold of NONE will log nothing.
      @see LogLevel
      @see setLogThreshold()
      @returns the log threshold currently active.
      */
    LogLevel logThreshold() const;

    /**
      Shall we print log messages to the console as well as the file?
      This allows you to disable console logging which is useful in the case of background
      services/daemons.
      This is enabled by default.
      @param enable set to true if logging to console is desired, false if not.
      */
    void setLogToConsole(bool enabled);
    /**
      Will the logging singleton log to console?
      @returns true if the logging singleton will log to console, false if it will not.
      @see setLogToConsole()
      */
    bool logToConsole() const;

    /**
      Shall we limit the logfile to a certain number of lines?
      This is useful if we are running on embedded platforms with extended runtimes.
      Setting this to any non-zero number will cause the the Logger to truncate the
      file, discarding all previous output and starting fresh when the limit is hit.
      Setting this to zero will remove any limitations on the number of lines logged.
      The default value is zero (no limitation).
      @param numLines the maximum number of lines to log to the logfile.
      @see logLimit()
      */
    void setLogLimit(int numLines);
    /**
      Returns the maximum number of lines the Logger will log to the logfile.
      If this value is non-zero, the logfile will be truncated when the given
      limit is hit. If this value is zero, no limit is imposed.
      @see setLogLimit()
      */
    int logLimit() const;

protected:
    /**
      Default constructor.
      Attempts to open the logfile and determines which logging level to set
      as the threshold. The log file is opened in a folder in the running users
      home directory. The threshold is found using QSettings.
      */
    Logger() throw();
    /**
      Default destructor.
      Closes the logfile if it is open.
      */
    virtual ~Logger() throw();
    /**
     * Static message handler for Qt messages
     */
    static void logMessageHandler(QtMsgType type, const char *msg);

private:
    /// for thread safety
    static QMutex m_creationalMutex;
    static QMutex m_operationalMutex;
    /// the file to log to.
    QFile logFile;
    /// the single instance kept of this class.
    static Logger *_instance;
    /// the minimum log threshold read using QSettings.
    LogLevel m_logThreshold;
    /// The previous message handler. Restore this upon destruction.
    QtMsgHandler oldHandler;
    /// Shall we log to the console as well as the file?
    bool m_logToConsole;
    /// How many lines should be logged before we truncate the file?
    int m_logLimit;
    /// How many lines have we logged so far?
    int m_linesLogged;

#ifdef Q_OS_LINUX
    /// Support colours in the terminal.
    static QString col[4];
#endif
};

#endif // LOGSINGLETON_H
