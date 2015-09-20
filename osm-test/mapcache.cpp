#include "mapcache.h"

MapCache::MapCache(OSMHandlerPtr osmHandler, QObject *parent) : QObject(parent)
{

}

QList<WayPtr> MapCache::nearestWays(double X, double Y, double maxDistance)
{
    QList<WayPtr> result;
    QList<long> way_ids = _handler->nearestWays(X, Y, maxDistance);

    foreach (long id, way_ids)
    {
        if (!_ways.contains(id))
        {
            WayPtr w = WayPtr::create(_handler.data(), id);
            _ways[id] = w;
        }
        result.append(_ways[id]);
    }

    return result;
}

QList<WayPtr> MapCache::getLinearFeatures(double X, double Y, double maxDistance)
{
    QList<WayPtr> result;
    QList<long> way_ids = _handler->features(X, Y, maxDistance);
    
    foreach (long id, way_ids) {
        if (!_features.contains(id))
        {
            WayPtr w = WayPtr::create(_handler.data(), id);
            _features[id] = w;
        }
        result.append(_features[id]);
    }
    return result;
}

QList<NodeAssociatedToWayPtr> MapCache::getPointFeatures(double X, double Y, double maxDistance)
{
    QList<NodeAssociatedToWayPtr> result;
    return result;
}
