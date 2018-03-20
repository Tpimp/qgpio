CXX_MODULE = qml
TARGET = softpwmplugin
TARGETPATH = SoftPwm
IMPORT_VERSION = 1.0

QT += core qml quick softpwm

CONFIG += qt plugin c++11

SOURCES += \
    softpwm_plugin.cpp

DISTFILES = qmldir

load(qml_plugin)
