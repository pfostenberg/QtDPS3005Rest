#include "mainwindow.h"

#include <QApplication>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QLoggingCategory>
#include <QTime>

#include "restserver.h"
#include "restrequestlistener.h"
#include "restrequest.h"
#include "restdpslistener.h"


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString ds = QTime::currentTime().toString();
    std::string sds = ds.toStdString();
    const char *pds = sds.c_str();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "%s DBG: %s\n", pds, localMsg.constData()); // (%s:%u, %s)  , context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "%s INF: %s\n", pds, localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "%s WRN: %s\n", pds, localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "%s CRI: %s\n", pds, localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "%s FAT: %s (%s:%u, %s)\n", pds, localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}


int main(int argc, char *argv[])
{
    // Uncomment the following line to enable logging
    // QLoggingCategory::setFilterRules(QStringLiteral("qt.modbus* = true"));
    QApplication a(argc, argv);
    qInstallMessageHandler(myMessageOutput);

    MainWindow w;

    // Rest GET examples: call // http://127.0.0.1:5053/ or see void RestDpsListener::http_get
    RESTServer server;
    server.addRequestListener( new RestDpsListener(&w) );
    server.listen( 5053 );  // HEX of PS

    w.show();

    return a.exec();
}
