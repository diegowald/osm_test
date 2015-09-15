#include "way.h"
#include "vector2d.h"


Way::Way(OSMHandler *osmHandler, long way_id, QObject *parent) : QObject(parent)
{
    _handler = osmHandler;
    _id = way_id;
    _points = _handler->getWayNodes(way_id);
}


bool Way::pointInWay(double &x, double &y, double &direction, double &threshold)
{
    for (int i = 1; i < _points.count(); ++i)
    {
        if (pointInSegment(x, y, i))
        {
            return isDirectionAlignedToSegment(i, direction, threshold);
        }
    }
}

bool Way::pointInWay(double &x, double &y)
{
    for (int i = 1; i < _points.count(); ++i)
    {
        if (pointInSegment(x, y, i))
        {
            return true;
        }
    }
    return false;
}

bool Way::pointInSegment(double &x, double &y, int lastIndex)
{
    Vector2D v1, v2;
    OSMPointPtr pt1, pt2;
    pt1 = _points.at(lastIndex - 1);
    pt2 = _points.at(lastIndex);
    v1.setBy2Points(pt1->x(), pt1->y(), pt2->x(), pt2->y());
    v2.setBy2Points(pt1->x(), pt1->y(), x, y);

    double module1 = v1.module();
    //double module2 = v2.module();
    double scalar = v1.scalar(v2);

    double projection = scalar / module1;

    return ((0 <= scalar) && (scalar <= module1));
}

bool Way::isDirectionAlignedToSegment(int lastIndex, double &direction, double &threshold)
{
    OSMPointPtr pt1 = _points.at(lastIndex - 1);
    OSMPointPtr pt2 = _points.at(lastIndex);
    Vector2D v(pt1->x(), pt1->y(), pt2->x(), pt2->y());
    double angle = v.angle();
    double deltaAngle = fabs(angle - direction);
    if (deltaAngle <= threshold)
        return true;
    deltaAngle = fabs(angle - direction + 3.141592654 / 2);
    return deltaAngle <= threshold;
}


QList<OSMPointPtr> Way::points() const
{
    return _points;
}
