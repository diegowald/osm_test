#include "osmhandler.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>


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

long OSMHandler::nearestWay(double &x, double &y, double &direction)
{
    QList<long> candidates = nearestWays(x, y);

    return candidates.count() > 0 ? candidates.at(0) : -1;
}

QList<long> OSMHandler::nearestWays(double &x, double &y)
{
    QList<long> results;

    QString sql =
            " select node_id, abs(x - :x) + abs(y- :y) as dst "
            " from osm_nodes where node_id in "
            " (select node_id from osm_way_refs where way_id in "
            " (select way_id from osm_way_tags where k = 'highway')) "
            " order by dst limit 1;";


    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":x", x);
    query.bindValue(":y", y);
    if (query.exec())
    {
        while (query.next())
        {
            results.append(query.value(0).toLongLong());
        }
    }
    else
    {
        qDebug() << query.lastError().text();
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
        qDebug() << query.lastError().text();
    }
    return -1;
}

QList<NodeAssociatedToWayPtr> OSMHandler::getInfoNodes(long way_id)
{
    QMap<long, NodeAssociatedToWayPtr> result;

    QString sql =
            " select osm_nodes.node_id, x, y, k, v "
            " from osm_nodes inner join "
            " osm_node_tags on osm_nodes.node_id = osm_node_tags.node_id "
            " inner join osm_way_refs on osm_nodes.node_id = osm_way_refs.node_id "
            " where osm_way_refs.way_id = :way_id";

    //node_id|x|y|k|v

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":way_id", QVariant::fromValue<long>(way_id));
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
        qDebug() << query.lastError().text();
    }
    qDebug() << way_id << ": " << result.count();
    return result.values();
}

QList<NodeAssociatedToWayPtr> OSMHandler::getIntersections(long way_id)
{
    QMap<long, NodeAssociatedToWayPtr> result;

    QString sql =
            " select intersections.way_id, x, y, k, v  "
            " from osm_way_refs currentway inner join "
            " osm_nodes on currentway.node_id = osm_nodes.node_id inner join "
            " osm_way_refs intersections on "
            " currentway.node_id = intersections.node_id "
            " inner join osm_way_tags on osm_way_tags.way_id = intersections.way_id "
            " where currentway.way_id = :way_id and intersections.way_id != :way_id; ";

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":way_id", QVariant::fromValue<long>(way_id));
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
        qDebug() << query.lastError().text();
    }
    qDebug() << way_id << ": " << result.count();
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
        qDebug() << query.lastError().text();
    }
    qDebug() << way_id << ": " << result.count();
    return result;
}
