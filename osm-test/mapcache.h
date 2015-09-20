#ifndef MAPCACHE_H
#define MAPCACHE_H

#include <QObject>
#include "osmhandler.h"
#include "way.h"

class MapCache : public QObject
{
    Q_OBJECT
public:
    explicit MapCache(OSMHandlerPtr osmHandler, QObject *parent = 0);

    QList<WayPtr> nearestWays(double X, double Y, double maxDistance);
    QList<WayPtr> getLinearFeatures(double X, double Y, double maxDistance);
    QList<NodeAssociatedToWayPtr> getPointFeatures(double X, double Y, double maxDistance);
signals:

public slots:
private:

    OSMHandlerPtr _handler;
    QMap<long, WayPtr> _ways;
    QMap<long, WayPtr> _features;
};

typedef QSharedPointer<MapCache> MapCachePtr;

#endif // MAPCACHE_H
