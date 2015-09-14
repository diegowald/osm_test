#include "osmpoint.h"

OSMPoint::OSMPoint(long &id, double &x, double &y, QObject *parent) : QObject(parent)
{
    _id = id;
    _x = x;
    _y = y;
}

