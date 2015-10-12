#ifndef FORWARDVIEWWIDGET_H
#define FORWARDVIEWWIDGET_H

#include <QWidget>
#include "way.h"
#include "nodeassociatedtoway.h"

class ForwardViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ForwardViewWidget(QWidget *parent = 0);

    virtual void paintEvent(QPaintEvent *evt);

    void setMaxDistance(double distance);

    void setVehicleCoordinates(double X, double Y);

    void setCurrentWay(WayPtr way);
    void setIntersectionWays(QList<WayPtr> &ways);

    void setSignals(QList<NodeAssociatedToWayPtr> &nodes);
    void setIntersectionNodes(QList<NodeAssociatedToWayPtr> &nodes);
    void setRotation(double alpha);
    void setVehicleDirection(double direction);


signals:

public slots:

private:
    double scale();
    QPointF transformToWidgetCoords(QPointF realPoint);
    void drawWay(QPainter &painter, WayPtr way, QColor color);
    void drawSignal(QPainter &painter, NodeAssociatedToWayPtr node);
    QPixmap pixmap(NodeAssociatedToWayPtr node);
    double rad2Deg(double rad);

private:
    double _maxDistance;
    double _carX;
    double _carY;

    WayPtr _currentWay;
    QList<WayPtr> _ways;
    double _rotation;
    double _vehicleDirection;

    QList<NodeAssociatedToWayPtr> _nodes;
    QList<NodeAssociatedToWayPtr> _intersectionNodes;

};

#endif // FORWARDVIEWWIDGET_H
