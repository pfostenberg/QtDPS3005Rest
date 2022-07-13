QT += serialbus widgets
requires(qtConfig(combobox))

qtConfig(modbus-serialport): QT += serialport

QT += core

TARGET = QtDPS3005Rest
TEMPLATE = app
CONFIG += c++11

#include($$PWD/qthttpserver/src/3rdparty/http-parser.pri)
# http://localhost:5053/
#include($$PWD/qthttpserver\src\httpserver.pri)

include($$PWD/QSimpleRestServer\REST.pri)

SOURCES += \
    dpsdata.cpp \
    main.cpp\
    mainwindow.cpp \
    restdpslistener.cpp \
    settingsdialog.cpp

HEADERS += \
    dpsdata.h \
    mainwindow.h \
    restdpslistener.h \
    settingsdialog.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    master.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/serialbus/modbus/master
INSTALLS += target
