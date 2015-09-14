#ifndef WAYSIGNALDETECTOR_H
#define WAYSIGNALDETECTOR_H

#include <QObject>
#include "osmhandler.h"

// This class gathers the upcoming signals from the current way;

class WaySignalDetector : public QObject
{
    Q_OBJECT
public:
    explicit WaySignalDetector(OSMHandlerPtr osmHandler, QObject *parent = 0);

    virtual ~WaySignalDetector();

    QList<NodeAssociatedToWayPtr> getUpcommingSignals(double &x, double &y, double &direction);
    QList<NodeAssociatedToWayPtr> getUpcommingIntersections(double &x, double &y, double &direction);

signals:

public slots:

private:
    OSMHandlerPtr _handler;
};

typedef QSharedPointer<WaySignalDetector> WaySignalDetectorPtr;

#endif // WAYSIGNALDETECTOR_H
