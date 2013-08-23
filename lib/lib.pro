TARGET = qraiden
TEMPLATE = lib
win32: DESTDIR = ./
DEFINES += QRAIDEN_LIBRARY
include($$PWD/../src/raiden.pri)
