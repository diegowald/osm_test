#include "osmhandler.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
//#include <QDebug>
#include "way.h"

OSMHandler::OSMHandler(const QString &databaseFile, QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databaseFile);
    db.open();
}

OSMHandler::~OSMHandler()
{
    db.close();
}

long OSMHandler::nearestWay(double &x, double &y, double &direction, double &threshold)
{
    QList<long> candidates = nearestWays(x, y, threshold);

    double angleThreshold = 45. / 180. * 3.141592654;
    foreach (long way_id, candidates)
    {
        Way way(this, way_id);
        if (way.pointInWay(x, y, direction, angleThreshold))
            return way_id;
    }
    return -1;
}

QList<long> OSMHandler::nearestWays(double &x, double &y, double &threshold)
{
    QList<long> results;

    QString sql =
            "select osm_way_refs.way_id "
            " from osm_nodes "
            " inner join osm_way_refs on osm_nodes.node_id = osm_way_refs.node_id "
            " inner join osm_way_tags on osm_way_refs.way_id = osm_way_tags.way_id "
            " and osm_way_tags.k = 'highway' "
            " where x between :x1 and :x2 "
            " and   y between :y1 and :y2;";


    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":x1", x - threshold / 2);
    query.bindValue(":x2", x + threshold / 2);
    query.bindValue(":y1", y - threshold / 2);
    query.bindValue(":y2", y + threshold / 2);
//qDebug() << sql;
//qDebug() << ":x" << x;
//qDebug() << ":y"<< y;
//qDebug() << ":maxDist" << threshold;

    if (query.exec())
    {
        while (query.next())
        {
            results.append(query.value(0).toLongLong());
        }
    }
    else
    {
        //qDebug() << sql << ": " << query.lastError().text();
    }
    return results;
}

long OSMHandler::wayByNodeId(long nodeId)
{
    QString sql =
            " select way_id from osm_way_refs where node_id = :node_id;";


    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":node_id", QVariant::fromValue<long>(nodeId));
    if (query.exec())
    {
        while (query.next())
        {
            return query.value(0).toLongLong();
        }
    }
    else
    {
        //qDebug() << query.lastError().text();
    }
    return -1;
}

QList<NodeAssociatedToWayPtr> OSMHandler::getInfoNodes(long way_id, double &x, double &y,  double &maxDistance)
{
    QMap<long, NodeAssociatedToWayPtr> result;

    QString sql =
            " select osm_nodes.node_id, x, y, k, v "
            " from osm_nodes inner join "
            " osm_node_tags on osm_nodes.node_id = osm_node_tags.node_id "
            " inner join osm_way_refs on osm_nodes.node_id = osm_way_refs.node_id "
            " where osm_way_refs.way_id = :way_id "
            " and x between :x1 and :x2 "
            " and y between :y1 and :y2;";

    //node_id|x|y|k|v

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":way_id", QVariant::fromValue<long>(way_id));
    query.bindValue(":x1", x - maxDistance / 2);
    query.bindValue(":x2", x + maxDistance / 2);
    query.bindValue(":y1", y - maxDistance / 2);
    query.bindValue(":y2", y + maxDistance / 2);

    if (query.exec())
    {
        while (query.next())
        {
            long node_id = query.value(0).toLongLong();
            double x = query.value(1).toDouble();
            double y = query.value(2).toDouble();
            QString key = query.value(3).toString();
            QString value = query.value(4).toString();

            if (!result.contains(node_id))
            {
                NodeAssociatedToWayPtr node = NodeAssociatedToWayPtr::create();
                node->setCoords(x, y);
                node->setId(node_id);
                result[node_id] = node;
            }
            result[node_id]->addKeyValue(key, value);
        }
    }
    else
    {
        //qDebug() << query.lastError().text();
    }
    //qDebug() << way_id << ": " << result.count();
    return result.values();
}

QList<NodeAssociatedToWayPtr> OSMHandler::getIntersections(long way_id, double &x, double &y, double &maxDistance)
{
    QMap<long, NodeAssociatedToWayPtr> result;

    QString sql =
            " select intersections.way_id, x, y, k, v  "
            " from osm_way_refs currentway inner join "
            " osm_nodes on currentway.node_id = osm_nodes.node_id inner join "
            " osm_way_refs intersections on "
            " currentway.node_id = intersections.node_id "
            " inner join osm_way_tags on osm_way_tags.way_id = intersections.way_id "
            " where currentway.way_id = :way_id and intersections.way_id != :way_id "
            " and x between :x1 and :x2 "
            " and y between :y1 and :y2;";

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":way_id", QVariant::fromValue<long>(way_id));
    query.bindValue(":x1", x - maxDistance / 2);
    query.bindValue(":x2", x + maxDistance / 2);
    query.bindValue(":y1", y - maxDistance / 2);
    query.bindValue(":y2", y + maxDistance / 2);
    if (query.exec())
    {
        while (query.next())
        {
            long way_idIntersection = query.value(0).toLongLong();
            double x = query.value(1).toDouble();
            double y = query.value(2).toDouble();
            QString key = query.value(3).toString();
            QString value = query.value(4).toString();

            if (!result.contains(way_idIntersection))
            {
                NodeAssociatedToWayPtr node = NodeAssociatedToWayPtr::create();
                node->setCoords(x, y);
                node->setId(way_idIntersection);
                result[way_idIntersection] = node;
            }
            result[way_idIntersection]->addKeyValue(key, value);
        }
    }
    else
    {
        //qDebug() << query.lastError().text();
    }
    //qDebug() << way_id << ": " << result.count();
    return result.values();
}

QList<OSMPointPtr> OSMHandler::getWayNodes(long way_id)
{
    QList<OSMPointPtr> result;

    QString sql =
            " select osm_nodes.node_id, x, y, sub from osm_nodes "
            " inner join osm_way_refs on osm_nodes.node_id = osm_way_refs.node_id "
            " where osm_way_refs.way_id = :way_id order by osm_way_refs.sub;";

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":way_id", QVariant::fromValue<long>(way_id));
    //qDebug() << sql;
    //qDebug() << way_id;
    if (query.exec())
    {
        while (query.next())
        {
            long node_id = query.value(0).toLongLong();
            double x = query.value(1).toDouble();
            double y = query.value(2).toDouble();

            OSMPointPtr point = OSMPointPtr::create(node_id, x, y);

            result.append(point);
        }
    }
    else
    {
        //qDebug() << query.lastError().text();
    }
    //qDebug() << way_id << ": " << result.count();
    return result;
}
