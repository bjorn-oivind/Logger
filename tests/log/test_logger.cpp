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

#include "test_logger.h"
#include "common/setup.h"

#include <QTextStream>

void TestLogger::initTestCase()
{
    setupTests();
    // make sure we take control of the default message handler.
    Logger::instance()->close();
}

void TestLogger::cleanupTestCase()
{
    teardownTests();
}

void TestLogger::init()
{
    Logger *log = Logger::instance();
    // make sure the log threshold is set to DEBUG
    log->setLogThreshold(DEBUG);
    // set a temporary path for the test logfile
    log->setLogPath(QDir::tempPath(), "test_logger.log");
    // open the log file
    QString filename = "test_logger.log";
    QString logDirPath = QDir::tempPath();
    m_logFile.setFileName(logDirPath + QDir::separator() + filename);
    m_logFile.open(QIODevice::Text | QIODevice::ReadOnly);
}

void TestLogger::cleanup()
{
    Logger::instance()->close();
    m_logFile.close();
}

void TestLogger::testInstance()
{
    Logger *l1 = Logger::instance();
    Logger *l2 = Logger::instance();

    QCOMPARE(l1, l2);
}

void TestLogger::testClose()
{
    try {
        Logger *l1 = Logger::instance();
        l1->close();
    }
    catch(...) {
        QFAIL("Exception caught while closing instance");
    }
}

void TestLogger::testLog()
{
    // check that the messages wind up in the log file.
    // log messages are prefixed with a timestamp, we don't want to compare these
    // as there may be subtle differences.
    Logger *log = Logger::instance();
    QTextStream s(&m_logFile);
    QString line;

    // Debug
    log->log(DEBUG, "testmessage");
    line = s.readLine();
    QCOMPARE(line.endsWith("[DEBUG]    testmessage"), true);

    // this should also happen with qDebug().
    // However, note that qDebug() has a tendency to add spaces (it uses maybespace).
    qDebug() << "testmessage2";
    line = s.readLine();
    QCOMPARE(line.endsWith("[DEBUG]    testmessage2 "), true);

    // Info
    log->log(INFO, "info message");
    line = s.readLine();
    QCOMPARE(line.endsWith("[INFO]     info message"), true);
    // there is no qInfo() :(

    // Warning
    log->log(WARNING, "warning message");
    line = s.readLine();
    QCOMPARE(line.endsWith("[WARNING]  warning message"), true);

    qWarning() << "warning message2";
    line = s.readLine();
    QCOMPARE(line.endsWith("[WARNING]  warning message2 "), true);

    // critical
    log->log(CRITICAL, "critical message");
    line = s.readLine();
    QCOMPARE(line.endsWith("[CRITICAL] critical message"), true);

    qCritical() << "critical message 2";
    line = s.readLine();
    QCOMPARE(line.endsWith("[CRITICAL] critical message 2 "), true);

    // you could test qFatal too, but since that only calls log->log(CRITICAL, ...) and then
    // exits the application with an error message, it seems a bit troublesome for no gain.
}

void TestLogger::benchmarkLog()
{
    Logger *log = Logger::instance();
    log->setLogToConsole(false);
    QBENCHMARK {
        log->log(DEBUG, "This is a benchmark");
    }
}

void TestLogger::testLogThreshold()
{
    // test checking log threshold, check that the contents of the log file
    // matches our expectations.
    Logger *log = Logger::instance();
    log->setLogThreshold(DEBUG);
    QCOMPARE(log->logThreshold(), DEBUG);

    // check that the correct amount is being logged to file.
    QTextStream s(&m_logFile);
    QString line;

    qDebug() << "testmessage";
    line = s.readLine();
    QCOMPARE(line.endsWith("[DEBUG]    testmessage "), true);

    qint64 size = m_logFile.size();

    // now up the threshold and make sure that we do not log debug.
    log->setLogThreshold(INFO);
    qDebug() << "more testmessages";
    // the log file should not have increased despite our qDebug()'ing.
    QCOMPARE(m_logFile.size(), size);

    // also check that NONE logs nothing at all
    log->setLogThreshold(NONE);

    qCritical() << "CRITICAL!";
    // the log file should not have increased, despite the horrible situation.
    QCOMPARE(m_logFile.size(), size);

    // reset log threshold
    log->setLogThreshold(DEBUG);
}

void TestLogger::testLogToConsole()
{
    Logger *log = Logger::instance();
    // log to console shall be enabled by default
    QCOMPARE(log->logToConsole(), true);
    log->setLogToConsole(false);
    QCOMPARE(log->logToConsole(), false);
    log->setLogToConsole(true);
    QCOMPARE(log->logToConsole(), true);
}

void TestLogger::testLogLimit()
{
    Logger *log = Logger::instance();
    // there shall be no limit set by default
    QCOMPARE(log->logLimit(), 0);
    log->setLogLimit(5);
    QCOMPARE(log->logLimit(), 5);

    // check that the correct amount is being logged to file.
    QTextStream s(&m_logFile);
    QString line;

    // log stuff and check that the output file is truncated
    for(int i = 0; i < 5; i++) {
        qDebug() << "testmessage";
        // check that the message arrived
        line = s.readLine();
        QCOMPARE(line.endsWith("[DEBUG]    testmessage "), true);
    }
    // we have logged five lines, the next line shall truncate the file.
    qint64 size = m_logFile.size();

    qDebug() << "post truncate";
    // check that the message arrived
    // we need to seek to the beginning of the file, since it has been truncated.
    s.seek(0);
    line = s.readLine();
    QCOMPARE(line.endsWith("[DEBUG]    post truncate "), true);

    // since the logfile has been truncated, the file shall be smaller than before.
    QVERIFY(m_logFile.size() < size);
}

QTEST_MAIN(TestLogger)
#include "test_logger.moc"
