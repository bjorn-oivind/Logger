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

  A common file for the Debug namespace and all of the classes inside this namespace.

  Has some classes and statements useful for debugging code.
  */

#ifndef DEBUG_H
#define DEBUG_H

#include <QtGlobal>

// Use the QElapsedTimer if we have a recent enough Qt version.
#if QT_VERSION >= 0x040700
#include <QElapsedTimer>
typedef QElapsedTimer DebugTimer;
#else
#include <QTime>
typedef QTime DebugTimer;
#endif // QT_VERSION 0x040700

#include <QtDebug>
#include "export.h"

/**
  Handy macro which can be placed at the top of any given function which
  desires to have entry, exit and time taken logged using the LogSingleton.
  */
#if defined QT_NO_DEBUG_OUTPUT || defined NO_LOG_FUNCTION
#define LOG_FUNCTION
#else
#define LOG_FUNCTION Debug::Scope __debuggingInstance__(Q_FUNC_INFO);
#endif

/**
  A namespace for assorted debug classes.
  */
namespace Debug
{
    /**
      A very simple class to store the indent level of a line of log.
      This simply contains the number of spaces the LogSingleton
      should indent with, and has a simple interface to push() and
      pop() indent levels.
      */
    class LOGGER_EXPORT Indent {
    private:
        /// the number of spaces to indent with
        static unsigned short numSpaces;
        /// the number of spaces for each level of indentation
        static const unsigned short SPACES_PER_LEVEL = 2;
    public:
        /**
          Adds a new level of indentation.
          */
        static void push();
        /**
          Removes a level of indentation.
          */
        static void pop();

        /**
          Returns the number of spaces to indent with.
          @return the number of spaces which should be written as indentation.
          */
        static unsigned short getIndent();

        /**
          Resets the number of spaces to indent with.
          */
        static void reset();

    private:
        /**
          Default constructor.
          */
        Indent();
    };

    /**
      A simple class responsible for logging its own creation and destruction, as well
      as the time taken between the two. This can be used to give a nice and easy to use
      log sequence of any given function or scope, where an instance can be declared at
      the top of the given function or scope on the stack, and automatically be destructed
      once control reaches the end of that scope.
      */
    class LOGGER_EXPORT Scope {
    private:
        /// a small string identifier meant to give contextual information.
        const char *identifier;

        /// How long the object has been alive.
        DebugTimer timer;
    public:
        /**
          Constructor.
          Sets the identifier to the given string, gets the time when the object was instantiated,
          and logs a simple message of the form "Entering " + str + "." to the LogSingleton with
          a LogLevel of DEBUG.
          */
        explicit Scope(const char *str);
        /**
          Destructor.
          Calculates the time between creation and destruction of this object, and logs this
          information in a simple message of the form "Leaving " + str + ". Took XXXms" to the
          LogSingleton with a LogLevel of DEBUG.
          */
        ~Scope();
    };
}

#endif // DEBUG_H
