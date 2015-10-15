#ifndef NODEASSOCIATEDTOWAY_H
#define NODEASSOCIATEDTOWAY_H

#include <QObject>
#include <QMap>
#include <QSharedPointer>
#include "feature.h"

class NodeAssociatedToWay : public Feature
{
    Q_OBJECT
public:
    explicit NodeAssociatedToWay(QObject *parent = 0);

    void setCoords(double &X, double &Y);

    double X() const;
    double Y() const;

    virtual int numPoints();

signals:

public slots:

private:
    double _x;
    double _y;
};

typedef QSharedPointer<NodeAssociatedToWay> NodeAssociatedToWayPtr;

#endif // NODEASSOCIATEDTOWAY_H
