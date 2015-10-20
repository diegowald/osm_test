#include "way.h"
#include "vector2d.h"
#include <QDebug>

#define MAXDIST 1e100;

Way::Way(OSMHandler *osmHandler, long way_id, QObject *parent) : Feature(parent)
{
    _handler = osmHandler;
    setId(way_id);
    _points = _handler->getWayNodes(way_id);
    QMap<QString, QString> values = _handler->getAssociatedInformation(way_id);
    foreach (QString k, values.keys())
    {
        addKeyValue(k, values[k]);
        //qDebug() << k << ", " << values[k];
    }
}


double Way::pointInWay(double &x, double &y, double &direction, double &threshold)
{
    double maxDistance = MAXDIST;
    for (int i = 1; i < _points.count(); ++i)
    {
        double distance = pointInSegment(x, y, i);
        if (distance < maxDistance)
        {
            if (isDirectionAlignedToSegment(i, direction, threshold))
            {
                maxDistance = distance;
            }
            //return isDirectionAlignedToSegment(i, direction, threshold);
        }
    }
    return maxDistance;
}

double Way::pointInWay(double &x, double &y)
{
    double maxDistance = MAXDIST;
    for (int i = 1; i < _points.count(); ++i)
    {
        double distance = distToSegment(x, y, i);
        if (distance < maxDistance)
        {
            maxDistance = distance;
        }
    }
    return maxDistance;
}

double Way::distToSegment(double xp, double yp, int segmentIndex)
{
    Vector2D v1, v2;
    OSMPointPtr pt1, pt2;
    pt1 = _points.at(segmentIndex - 1);
    pt2 = _points.at(segmentIndex);

    return distToSegment(xp, yp,
                         pt1->x(), pt1->y(),
                         pt2->x(), pt2->y());
}

bool Way::pointInSegment(double &x, double &y, int lastIndex)
{
    OSMPointPtr pt1, pt2;
    pt1 = _points.at(lastIndex - 1);
    pt2 = _points.at(lastIndex);


    double l2 = dist2(pt1->x(), pt1->y(), pt2->x(), pt2->y());
    if (l2 == 0.)
        return dist2(x, y, pt1->x(), pt2->x());

    double t = ((x - pt1->x()) * (pt2->x() - pt1->x()) + (y - pt1->y()) * (pt2->y() - pt1->y())) / l2;

    return (0 <= t) && (t <= 1);
}

double Way::sqr(double x)
{
    return x * x;
}

double Way::dist2(double x1, double y1, double x2, double y2)
{
    return sqr(x1 - x2) + sqr(y1 - y2);
}

double Way::distToSegmentSquared(double xp, double yp,
                                 double xv, double yv,
                                 double xw, double yw)
{
    double l2 = dist2(xv, yv, xw, yw);
    if (l2 == 0.)
        return dist2(xp, yp, xv, yv);

    double t = ((xp - xv) * (xw - xv) + (yp - yv) * (yw - yv)) / l2;

    if (t < 0) return dist2(xp, yp, xv, yv);

    if (t > 1) return dist2(xp, yp, xw, yw);

    return dist2(xp, yp,
                 xv + t * (xw - xv),
                 yv + t * (yw - yv));
}

double Way::distToSegment(double xp, double yp,
                          double xv, double yv,
                          double xw, double yw)
{
    return sqrt(distToSegmentSquared(xp, yp, xv, yv, xw, yw));
}


bool Way::isDirectionAlignedToSegment(int lastIndex, double &direction, double &threshold)
{
    double angle = segmentOrientation(lastIndex);
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

double Way::segmentOrientation(int lastIndex)
{
    OSMPointPtr pt1 = _points.at(lastIndex - 1);
    OSMPointPtr pt2 = _points.at(lastIndex);

    double deltaX = pt2->x() - pt1->x();
    double deltaY = pt2->y() - pt1->y();

    qDebug() << deltaY << " / " << deltaX << "= " << 3 * 3.141592654 / 2 * atan2(deltaY, deltaX);
    return 3 * 3.141592654 / 2 * atan2(deltaY, deltaX);
}

double Way::getOrientation(double &x, double &y, double &direction)
{
    for (int i = 1; i < _points.count(); ++i)
    {
        if (pointInSegment(x, y, i))
        {
            qDebug() << direction << " " << segmentOrientation(i) << " " << direction + 3.141592654;
            return segmentOrientation(i);
        }
    }
    return 0;
}

int Way::numPoints()
{
    return _points.length();
}
