# Logger

## Description

This is a simple singleton-based logger for Qt-based applications. It is
designed to be easy to use by registering itself as a Qt message handler,
meaning that all your qDebug()'s, qWarning()'s, qCritical()'s et cetera will
end up in a log, in addition to being printed out on the console.

It separates the output into several predefined levels. A threshold can be
set which ensures that no messages below this given level will be logged or
printed out. This threshold is by default set to WARNING, meaning that
qWarning(), qCritical() and qFatal() will end up in the log and console.
The threshold can be set programatically or via a configuration file.

It can also be used with embedded devices with limited storage/memory, by
setting a limit to the number of messages that can be logged (after which
the logfile will be truncated).

Additionally, it comes with a couple of useful debug classes. Debug::Scope
allows you to log entry and exit points, as well as the duration. The
generalisation of this class is the macro "LOG_FUNCTION" which, when placed
at the top of any given function, log the entry and exit times of this function,
giving you a rough estimate of where time is spent in your application.
This, coupled with the Debug::Indent class allows the logger to prettify
your output a bit, making it easier to track the flow of your program in the
event that you are having some trouble.
An example of the output follows:

```
[21:04:01] [DEBUG]    Entering void Plugin::System::parseDescriptions().
[21:04:01] [DEBUG]      Entering bool Plugin::XMLSpecReader::parseXML(QIODevice*, QSharedPointer<Plugin::Description>).
[21:04:01] [DEBUG]      Leaving bool Plugin::XMLSpecReader::parseXML(QIODevice*, QSharedPointer<Plugin::Description>). Took 2 ms
[21:04:01] [DEBUG]      Entering bool Plugin::XMLSpecReader::parseXML(QIODevice*, QSharedPointer<Plugin::Description>).
[21:04:01] [DEBUG]      Leaving bool Plugin::XMLSpecReader::parseXML(QIODevice*, QSharedPointer<Plugin::Description>). Took 1 ms
[21:04:01] [DEBUG]    Leaving void Plugin::System::parseDescriptions(). Took 4 ms
```

These are only active when QT_NO_DEBUG_OUPUT has not been defined and the log
threshold is set to DEBUG.

## Usage

The logger uses QSettings to store settings, so please set up organization name
and application name at least before creating an instance of the logger.

Create an instance of the Logger singleton to make it register itself as a
Qt message handler. This can be done early in main().

The configuration settings that can be changed are:

- Log/log_path - Where to place the log file (default is HOME/.ApplicationName/)

- Log/Log_filename - What to name the log file (default is ApplicationName.log)

- Log/log_threshold - What to log (default is WARNING, meaning all warning
                      messages and above. Valid values are DEBUG, INFO, WARNING,
                      CRITICAL and NONE).

To compile and run the tests, please use CMake.
A rough guide follows:

- $ mkdir build

- $ cd build

- $ cmake ..

- $ make -j5

- $ make test

## License

This logger is licensed under the LGPL v2.1.

## Written By

[Bjørn Øivind Bjørnsen](https://github.com/bjorn-oivind)
