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

    // Rest GET examples: call // http://127.0.0.1:5053/ or see void RestDpsListener::http_get
    RESTServer server;
    server.addRequestListener( new RestDpsListener(&w) );
    server.listen( 5053 );  // HEX of PS

    w.show();

    return a.exec();
}
