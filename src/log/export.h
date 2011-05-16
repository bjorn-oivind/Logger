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

#ifndef LOG_EXPORT_H
#define LOG_EXPORT_H

#if !defined LOGGER_STATIC
#   if defined EXPORT_LOGGER
#       define LOGGER_EXPORT Q_DECL_EXPORT
#   else
#       define LOGGER_EXPORT Q_DECL_IMPORT
#   endif
#else
#   define LOGGER_EXPORT
#endif

#endif // LOG_EXPORT_H
