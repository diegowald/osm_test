#include "waysignaldetector.h"
#include <QDebug>


WaySignalDetector::WaySignalDetector(OSMHandlerPtr osmHandler, QObject *parent) : QObject(parent)
{
    _handler = osmHandler;
    maxDistanceMeters = 500.;
    maxDistance = meters2Degrees(maxDistanceMeters);
    qDebug() << maxDistanceMeters;
    qDebug() << maxDistance;
    qDebug() << degrees2meters(maxDistance);
}

WaySignalDetector::~WaySignalDetector()
{
}

QList<NodeAssociatedToWayPtr> WaySignalDetector::getUpcommingSignals(double &x, double &y, double &direction)
{
    QList<NodeAssociatedToWayPtr> nodes;

    int nearestWay = _handler->nearestWay(x, y, direction, maxDistance);

    //int nearestWay = _handler->wayByNodeId(nearestNode);

    nodes = _handler->getInfoNodes(nearestWay, x, y, maxDistance);
    return nodes;
}

QList<NodeAssociatedToWayPtr> WaySignalDetector::getUpcommingIntersections(double &x, double &y, double &direction)
{
    int nearestWay = _handler->nearestWay(x, y, direction, maxDistance);
    //int nearestWay = _handler->wayByNodeId(nearestNode);

    QList<NodeAssociatedToWayPtr> intersections = _handler->getIntersections(nearestWay, x, y, maxDistance);
    return intersections;
}

double WaySignalDetector::meters2Degrees(double &meters)
{
    // 1 minute = 1 nautical mile (in Equator)
    // 1 degree = 1 mile / 60;
    // 1 nautical mile = 1853.2 m
    // degrees = meters / 1853.2 / 60;
    return meters / 1853.2 / 60.;
}

double WaySignalDetector::degrees2meters(double &degrees)
{
    return degrees * 1853.2 * 60;
}
