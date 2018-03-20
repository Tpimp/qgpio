CXX_MODULE = qml
TARGET = gpioplugin
TARGETPATH = Gpio
IMPORT_VERSION = 1.0

QT += core qml quick gpio

CONFIG += qt plugin c++11

SOURCES += \
    gpio_plugin.cpp

DISTFILES = qmldir

load(qml_plugin)
