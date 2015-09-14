#include "osmpoint.h"

OSMPoint::OSMPoint(long &id, double &x, double &y, QObject *parent) : QObject(parent)
{
    _id = id;
    _x = x;
    _y = y;
}


double OSMPoint::x() const
{
    return _x;
}

double OSMPoint::y() const
{
    return _y;
}


long OSMPoint::id() const
{
    return _id;
}
