# serialbus was the first working attempt.
# but quite now does not work on a standard PI
# so integrate QModbus directly
QT += widgets
QT += core

requires(qtConfig(combobox))

#qtConfig(modbus-serialport):
QT += serialport

TARGET = QtDPS3005Rest
TEMPLATE = app
CONFIG += c++11

include($$PWD/QSimpleRestServer/REST.pri)

INCLUDEPATH += 3rdparty/libmodbus \
               3rdparty/libmodbus/src

win32:{
    DEFINES +=  _CRT_SECURE_NO_WARNINGS
}
SOURCES += \
    dpsdata.cpp \
    main.cpp\
    mainwindow.cpp \
    restdpslistener.cpp \
    3rdparty/libmodbus/src/modbus.c \
    3rdparty/libmodbus/src/modbus-data.c \
    3rdparty/libmodbus/src/modbus-rtu.c \
    3rdparty/libmodbus/src/modbus-ascii.c


HEADERS += \
    dpsdata.h \
    mainwindow.h \
    restdpslistener.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    master.qrc

target.path = build
INSTALLS += target
