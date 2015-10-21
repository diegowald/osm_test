#include "waysignaldetector.h"


WaySignalDetector::WaySignalDetector(OSMHandlerPtr osmHandler, QObject *parent) : QObject(parent)
{
    _handler = osmHandler;
    maxDistanceMeters = 750.;
    maxDistance = meters2Degrees(maxDistanceMeters);
    //qDebug() << maxDistanceMeters;
    //qDebug() << maxDistance;
    //qDebug() << degrees2meters(maxDistance);
}


WaySignalDetector::~WaySignalDetector()
{
}

double WaySignalDetector::getMaxDistance()
{
    return maxDistance;
}

QList<NodeAssociatedToWayPtr> WaySignalDetector::getUpcommingSignals(double &x, double &y, double &direction, double &speed)
{
    QList<NodeAssociatedToWayPtr> nodes;

    WayPtr nearestWay = _handler->nearestWay(x, y, direction, maxDistance, speed);
    //WayPtr nearestWay = _handler->nearestWay(x, y, maxDistance);

    nodes = _handler->getInfoNodes(nearestWay->id(), x, y, maxDistance);
    return nodes;
}

QList<NodeAssociatedToWayPtr> WaySignalDetector::getUpcommingIntersections(double &x, double &y, double &direction)
{
    //int nearestWay = _handler->nearestWay(x, y, direction, maxDistance);
    WayPtr nearestWay = _handler->nearestWay(x, y, maxDistance);

    QList<NodeAssociatedToWayPtr> intersections = _handler->getIntersections(nearestWay->id(), x, y, maxDistance);
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

WayPtr WaySignalDetector::getCurrentWay(double &x, double &y, double &direction, double &speed)
{
    //return _handler->nearestWay(x, y, maxDistance);
    return _handler->nearestWay(x, y, direction, maxDistance, speed);
}

QList<WayPtr> WaySignalDetector::getIntersectionWays(QList<NodeAssociatedToWayPtr> intersections)
{
    QList<WayPtr> result;

    foreach (NodeAssociatedToWayPtr node, intersections)
    {
        result.append(WayPtr::create(_handler.data(), node->id()));
    }

    return result;
}
