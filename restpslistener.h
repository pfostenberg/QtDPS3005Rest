#ifndef RESTPSLISTENER_H
#define RESTPSLISTENER_H

#include <qobject.h>

#include "restrequest.h"

class RestPsListener
{
    Q_OBJECT
    public:
      explicit RestPsListener(QObject * parent = 0);

    signals:

    public slots:
      void http_get_( RESTRequest * request );
};

#endif // RESTPSLISTENER_H
