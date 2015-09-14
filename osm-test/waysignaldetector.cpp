#include "waysignaldetector.h"

WaySignalDetector::WaySignalDetector(OSMHandlerPtr osmHandler, QObject *parent) : QObject(parent)
{
    _handler = osmHandler;
}

WaySignalDetector::~WaySignalDetector()
{
}

QList<NodeAssociatedToWayPtr> WaySignalDetector::getUpcommingSignals(double &x, double &y, double &direction)
{
    int nearestNode = _handler->nearestWay(x, y, direction);
    int nearestWay = _handler->wayByNodeId(nearestNode);

    QList<NodeAssociatedToWayPtr> nodes = _handler->getInfoNodes(nearestWay);
    return nodes;
}

QList<NodeAssociatedToWayPtr> WaySignalDetector::getUpcommingIntersections(double &x, double &y, double &direction)
{
    int nearestNode = _handler->nearestWay(x, y, direction);
    int nearestWay = _handler->wayByNodeId(nearestNode);

    QList<NodeAssociatedToWayPtr> intersections = _handler->getIntersections(nearestWay);
    return intersections;
}
