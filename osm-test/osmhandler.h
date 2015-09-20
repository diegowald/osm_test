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

    long nearestWay(double &x, double &y, double &direction, double &threshold);
    QList<long> nearestWays(double &x, double &y, double &threshold);

    long wayByNodeId(long nodeId);

    QList<NodeAssociatedToWayPtr> getInfoNodes(long way_id, double &x, double &y,  double &maxDistance);
    QList<NodeAssociatedToWayPtr> getIntersections(long way_id, double &x, double &y, double &maxDistance);

    QList<OSMPointPtr> getWayNodes(long way_id);

    QList<long> features(double &x, double &y, double &maxDistance);

    QMap<QString, QString> getAssociatedInformation(long way_id);

signals:

public slots:

private:
    QSqlDatabase db;
};

typedef QSharedPointer<OSMHandler> OSMHandlerPtr;

#endif // OSMHANDLER_H
