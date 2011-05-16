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

  Implementation of the classes defined in debug.h.
  */
#include "debug.h"

namespace Debug {
    unsigned short Indent::numSpaces = 0;
    Indent::Indent()
    {
    }

    void Indent::push()
    {
        numSpaces += SPACES_PER_LEVEL;
    }

    void Indent::pop()
    {
        numSpaces -= SPACES_PER_LEVEL;
    }

    unsigned short Indent::getIndent()
    {
        return numSpaces;
    }

    void Indent::reset()
    {
        numSpaces = 0;
    }

    Scope::Scope(const char *str)
            : identifier(str)
    {
        qDebug("Entering %s.", identifier);
        timer.start();
        Indent::push();
    }

    Scope::~Scope()
    {
        Indent::pop();
        int ms = timer.elapsed();
        qDebug("Leaving %s. Took %d ms", identifier, ms);
    }
}
