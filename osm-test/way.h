#ifndef WAY_H
#define WAY_H

#include <QObject>
#include "osmhandler.h"
#include "osmpoint.h"


class Way : public QObject
{
    Q_OBJECT
public:
    explicit Way(OSMHandlerPtr osmHandler, long way_id, QObject *parent = 0);

    bool pointInWay(double &x, double &y, double &direction);
    bool pointInWay(double &x, double &y);
signals:

public slots:

private:
    OSMHandlerPtr _handler;
    QList<OSMPointPtr> _points;
    long _id;
};

#endif // WAY_H
