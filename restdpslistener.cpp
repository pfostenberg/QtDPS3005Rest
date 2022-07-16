#include "restdpslistener.h"

#include <QtDebug>
#include <QThread>
#include <QCoreApplication>


RestDpsListener::RestDpsListener(MainWindow *mw, QObject *parent) : RESTRequestListener("/", parent)
{
    m_ModBus = mw;
}

void RestDpsListener::http_get_(RESTRequest * request)
{
    qDebug() << "http_get_" << request->params();
    QString msg = "hellp workd";
  request->result()->setData( msg);
  request->result()->setStatusCode( 200 );
}

void RestDpsListener::http_get_v1STAR(RESTRequest * request)
{
    QString path = request->path();
    qDebug() << "http_get_v1" << path; // request->params();

    QString msg = "";

    QMap<QString, QString> params = request->params();

    if( params.contains( "V" ) )
    {
        QString v=params["V"];
        msg = "got Voltage: ";
        msg += v;

        float fv = v.toFloat();
        int vMalHundert = fv*100;
        m_ModBus->onSetUint(0,vMalHundert);

        bool done=false;
        int cnt = 0;
        while(!done)
        {
            cnt++;
            for (int t=0;t<1000;t++)
            {
            QCoreApplication::processEvents();
            QThread::msleep(1);
            }
           m_ModBus->onSecond();

           float av = m_ModBus->getActVoltage();
           float delta = av-fv;
           qDebug() << "Delta: " << delta;
           if(cnt > 10)
           {
               qDebug() << "timeout after sec: " << cnt;
               done = true;
               msg = "timeout after 10 sec voltate not reached! " + v;
           }
           if (abs(delta) < 0.2)
           {
               qDebug() << "delta OK after sec: " << cnt;
               done = true;
           }
        }
        // todo readback...
    }
    else
    {
        if( params.contains( "I" ) )
        {
            QString v=params["I"];
            msg = "got Current: ";
            msg += v;

            float fv = v.toFloat();
            int vMalHundert = fv*100;
            m_ModBus->onSetUint(1,vMalHundert);

            // todo readback...
        }
        else
        {
          msg="Error no parater V";

        }

    }

  request->result()->setData( msg);
  request->result()->setStatusCode( 200 );
}

