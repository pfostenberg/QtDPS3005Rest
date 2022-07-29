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
    // default handler... all unknonw stuff -> show manual ( not html!!)
  qDebug() << "http_get_" << request->params();
  QString msg = "QtDPS3005Rest Interface: Version 0.3.0";  // VERSION-TAG also in the ui headline!!
  msg+="\nhttp://127.0.0.1:5053/v1/set?V=12.0   to set Voltaget o: 12.0";
  msg+="\nhttp://127.0.0.1:5053/v1/set?I=1.0";
  msg+="\nhttp://127.0.0.1:5053/v1/get";

  request->result()->setData( msg);
  request->result()->setStatusCode( 200 );
}

QString  RestDpsListener::getValesAsJson(void)
{
    return m_ModBus->getValuesAsJson();

}

void RestDpsListener::http_get_v1STAR(RESTRequest * request)
{
    QString path = request->path();
    qDebug() << "http_get_v1" << path << request->params();

    QString msg = "unknown path: " + path;

    QMap<QString, QString> params = request->params();

    if (!path.startsWith("/v1"))
    {
        msg = "unknown path: " + path;
        request->result()->setData( msg);
        request->result()->setStatusCode( 400 );
        return;
    }
    if (path.startsWith("/v1/get"))
    {
        msg = getValesAsJson();
        request->result()->setData( msg);
        request->result()->setStatusCode( 200 );
        return;
    }

    if (path.startsWith("/v1/set"))
    {
        if( params.contains( "V" ) )
        {
            QString v=params["V"];

            float fv = v.toFloat();
            qDebug() << "http_get_v2: " << v << " as float: " << fv;
            int vMalHundert = fv*100.0;
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

               msg = getValesAsJson();
               request->result()->setData( msg);
               request->result()->setStatusCode( 200 );
               return;
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
    }

  request->result()->setData( msg);
  request->result()->setStatusCode( 400 );
}

