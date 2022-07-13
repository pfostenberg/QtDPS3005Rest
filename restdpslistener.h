#ifndef RESTDPSLISTENER_H
#define RESTDPSLISTENER_H

#include <QObject>
#include "restrequest.h"
#include "mainwindow.h"

class RestDpsListener : public  RESTRequestListener
{
    Q_OBJECT

public:
   explicit RestDpsListener(MainWindow * mw, QObject *parent = 0);

signals:

public slots:
  void http_get_( RESTRequest * request );
  void http_get_v1STAR(RESTRequest * request);

private:
  MainWindow *m_ModBus;
};

#endif // RESTDPSLISTENER_H
