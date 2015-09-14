#ifndef WAY_H
#define WAY_H

#include <QObject>
#include "osmhandler.h"
#include "osmpoint.h"


class Way : public QObject
{
    Q_OBJECT
public:
    explicit Way(OSMHandler *osmHandler, long way_id, QObject *parent = 0);

    bool pointInWay(double &x, double &y, double &direction, double &threshold);
    bool pointInWay(double &x, double &y);

private:
    bool pointInSegment(double &x, double& y, int lastIndex);
    bool isDirectionAlignedToSegment(int lastIndex, double &direction, double &threshold);

signals:

public slots:

private:
    OSMHandler *_handler;
    QList<OSMPointPtr> _points;
    long _id;
};

#endif // WAY_H
