QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG += c++17

TEMPLATE = app

SOURCES +=  tst_shortstringbenchmark.cpp

HEADERS += \
    ../../src/ShortString.h

INCLUDEPATH += ../../src/
QMAKE_CXXFLAGS+= -pedantic-errors
