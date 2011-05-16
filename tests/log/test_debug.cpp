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

#include "test_debug.h"
#include "common/setup.h"

void TestDebug::initTestCase()
{
    setupTests();
}

void TestDebug::cleanupTestCase()
{
    teardownTests();
}

void TestDebug::init()
{
    Debug::Indent::reset();
}

void TestDebug::testIndentPush()
{
    // indent shall be 0 by default
    QCOMPARE((int)Debug::Indent::getIndent(), 0);
    Debug::Indent::push();
    QCOMPARE((int)Debug::Indent::getIndent(), 2);
}

void TestDebug::testIndentPop()
{
    testIndentPush();
    Debug::Indent::pop();
    QCOMPARE((int)Debug::Indent::getIndent(), 0);
}

void TestDebug::testScope()
{
    // Not much to test here beyond that it does not crash horribly.
    // You could install a different message handler, and check that it
    // prints the correct messages, but that seems like a lot of work for
    // testing something which is essentially six lines of code, two of which
    // being printouts.
    {
        Debug::Scope s("foo");
    }
}

QTEST_MAIN(TestDebug)
#include "test_debug.moc"
