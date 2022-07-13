QT += serialbus widgets
QT += core

requires(qtConfig(combobox))

qtConfig(modbus-serialport): QT += serialport

TARGET = QtDPS3005Rest
TEMPLATE = app
CONFIG += c++11

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
