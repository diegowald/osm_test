#ifndef OSMPOINT_H
#define OSMPOINT_H

#include <QObject>

class OSMPoint : public QObject
{
    Q_OBJECT
public:
    explicit OSMPoint(long &id, double &x, double &y, QObject *parent = 0);

signals:

public slots:

private:
    long _id;
    double _x;
    double _y;
};

typedef QSharedPointer<OSMPoint> OSMPointPtr;
#endif // OSMPOINT_H
