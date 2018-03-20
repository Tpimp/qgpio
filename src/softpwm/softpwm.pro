TARGET = QtSoftPwm
QT = core
VERSION = 1.0.0

CMAKE_MODULE_TESTS = '-'

INCLUDEPATH *= .

HEADERS += \
    qsoftpwm.h \
    qtsoftpwm_global.h

SOURCES += \
    qsoftpwm.cpp

load(qt_module)
