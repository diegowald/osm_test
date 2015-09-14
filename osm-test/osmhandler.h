#ifndef OSMHANDLER_H
#define OSMHANDLER_H

#include <QObject>
#include <QSqlDatabase>
#include "nodeassociatedtoway.h"
#include "osmpoint.h"


class OSMHandler : public QObject
{
    Q_OBJECT

public:
    OSMHandler(const QString &databaseFile, QObject *parent = 0);
    virtual ~OSMHandler();

    long nearestWay(double &x, double &y, double &direction);
    QList<long> nearestWays(double &x, double &y);

    long wayByNodeId(long nodeId);

    QList<NodeAssociatedToWayPtr> getInfoNodes(long way_id);
    QList<NodeAssociatedToWayPtr> getIntersections(long way_id);

    QList<OSMPointPtr> getWayNodes(long way_id);

signals:

public slots:

private:
    QSqlDatabase db;
};

typedef QSharedPointer<OSMHandler> OSMHandlerPtr;

#endif // OSMHANDLER_H
