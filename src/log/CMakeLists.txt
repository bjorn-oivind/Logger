# We need C-support to enable some of the configure time checks
project(logger C CXX)

cmake_minimum_required(VERSION 2.8)

# set versions
set(LOGGER_VERSION_MAJOR 1)
set(LOGGER_VERSION_MINOR 0)
set(LOGGER_VERSION_PATCH 0)
set(LOGGER_VERSION "${LOGGER_VERSION_MAJOR}.${LOGGER_VERSION_MINOR}.${LOGGER_VERSION_PATCH}")
set(LOGGER_SOVERSION 1)

find_package(Qt4 4.6 COMPONENTS QtCore REQUIRED)

# sources
set(LOG_SOURCES logger.cpp debug.cpp export.h)

# we don't need GUI
set(QT_DONT_USE_QTGUI true)

include(${QT_USE_FILE})
include_directories(${CMAKE_SOURCE_DIR} ${LOGGER_SOURCE_DIR} ${LOGGER_BINARY_DIR})

# Use fast string concatenation
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

add_definitions(-DEXPORT_LOGGER)
add_library(logger SHARED ${LOG_SOURCES})
target_link_libraries(logger ${QT_LIBRARIES})
set_target_properties(logger PROPERTIES VERSION ${LOGGER_VERSION} SOVERSION ${LOGGER_SOVERSION})

# install rules
install(TARGETS logger
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
