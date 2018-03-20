TARGET = QtGpio
QT = core
VERSION = 1.0.0

CMAKE_MODULE_TESTS = '-'

INCLUDEPATH *= .

HEADERS += \
    qgpio.h \
    qgpiowatcher.h \
    qtgpio_global.h

SOURCES += \
    qgpio.cpp

load(qt_module)
