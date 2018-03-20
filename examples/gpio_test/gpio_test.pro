TEMPLATE = app

QT += qml quick gpio

# Grab libs and includes so examples can be executed without installing
LIBS += -L../../lib
INCLUDEPATH += $$PWD/../../include

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

RESOURCES += qml.qrc

SOURCES += main.cpp

CONFIG += c++11

target.path = $$[QT_INSTALL_EXAMPLES]/gpio_test

sources.files = $$SOURCES $$HEADERS $$RESOURCES gpio_test.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/gpio_test

INSTALLS += target sources
