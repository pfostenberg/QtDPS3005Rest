#include "mainwindow.h"

#include <QApplication>
#include <QLoggingCategory>

#include "restserver.h"
#include "restrequestlistener.h"
#include "restrequest.h"
#include "restdpslistener.h"

int main(int argc, char *argv[])
{
    // Uncomment the following line to enable logging
    // QLoggingCategory::setFilterRules(QStringLiteral("qt.modbus* = true"));
    QApplication a(argc, argv);

    MainWindow w;

    // Rest GET examples:
    // http://127.0.0.1:5053/
    // http://127.0.0.1:5053/v1/set?V=12.0   -> got Voltage: 12.0
    // http://127.0.0.1:5053/v1/set?I=1.0
    //
    RESTServer server;
    server.addRequestListener( new RestDpsListener(&w) );
    server.listen( 5053 );  // HEX of PS

    w.show();

    return a.exec();
}
