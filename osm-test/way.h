#ifndef WAY_H
#define WAY_H

#include <QObject>
#include "osmhandler.h"
#include "osmpoint.h"
#include "feature.h"

class Way : public Feature
{
    Q_OBJECT
public:
    explicit Way(OSMHandler *osmHandler, long way_id, QObject *parent = 0);

    double pointInWay(double &x, double &y, double &direction, double &threshold);
    double pointInWay(double &x, double &y);

    QList<OSMPointPtr> points() const;

    double getOrientation(double &x, double &y, double &direction);

    virtual int numPoints();

private:
    bool pointInSegment(double &x, double& y, int lastIndex);
    bool isDirectionAlignedToSegment(int lastIndex, double &direction, double &threshold);
    double segmentOrientation(int lastIndex);

    double sqr(double x);
    double dist2(double x1, double y1, double x2, double y2);

    double distToSegmentSquared(double xp, double yp,
                                double xv, double yv,
                                double xw, double yw);
    double distToSegment(double xp, double yp,
                         double xv, double yv,
                         double xw, double yw);

    double distToSegment(double xp, double yp, int segmentIndex);

signals:

public slots:

private:
    OSMHandler *_handler;
    QList<OSMPointPtr> _points;
    long _id;
};

typedef QSharedPointer<Way> WayPtr;

#endif // WAY_H
