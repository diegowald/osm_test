#ifndef MAPVIEWWIDGET_H
#define MAPVIEWWIDGET_H

#include <QWidget>
#include "way.h"

class MapViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapViewWidget(QWidget *parent = 0);
    virtual void paintEvent(QPaintEvent *evt);

    void setMaxDistance(double distance);

    void setVehicleCoordinates(double X, double Y);
    void setVehicleDirection(double direction);

    void setWays(QList<WayPtr> &ways);
    void setRotation(double alpha);

signals:

public slots:

private:
    double scale();
    QPointF transformToWidgetCoords(QPointF realPoint);
    void drawWay(QPainter &painter, WayPtr way, QColor color);
    void drawSignal(QPainter &painter, NodeAssociatedToWayPtr node);
    QPixmap pixmap(NodeAssociatedToWayPtr node);

private:
    double _maxDistance;
    double _carX;
    double _carY;

    QList<WayPtr> _ways;
    double _vehicleDirection;
    double _rotation;
};

#endif // MAPVIEWWIDGET_H
