cmake_minimum_required(VERSION 2.8)

find_package(Qt4 4.6 COMPONENTS QtCore QtTest REQUIRED)

# we don't need GUI
set(QT_DONT_USE_QTGUI true)

include(${QT_USE_FILE})

# Logger test
# we want to make sure that QT_NO_DEBUG is not defined, as this will
# break the test. The test is dependant on having a working qDebug(), etc.
remove_definitions(-DQT_NO_DEBUG_OUTPUT)
set(TEST_NAME test_logger)
set(TEST_SOURCES test_logger.h test_logger.cpp)
#
qt4_automoc(${TEST_SOURCES})
add_executable(${TEST_NAME} ${TEST_SOURCES})
add_test(${TEST_NAME} ${TEST_NAME})
add_dependencies(${TEST_NAME} logger)
target_link_libraries(${TEST_NAME} logger ${QT_LIBRARIES})

# Debug test
set(TEST_NAME test_debug)
set(TEST_SOURCES test_debug.h test_debug.cpp)
#
qt4_automoc(${TEST_SOURCES})
add_executable(${TEST_NAME} ${TEST_SOURCES})
add_test(${TEST_NAME} ${TEST_NAME})
add_dependencies(${TEST_NAME} logger)
target_link_libraries(${TEST_NAME} logger ${QT_LIBRARIES})

