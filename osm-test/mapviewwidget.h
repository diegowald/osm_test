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

    void setLinearFeatures(QList<WayPtr> &features);
    void setPointFeatures(QList<NodeAssociatedToWayPtr> &pts);

signals:

public slots:

private:
    double scale();
    QPointF transformToWidgetCoords(QPointF realPoint);
    void drawWay(QPainter &painter, FeaturePtr feature);
    void drawSignal(QPainter &painter, NodeAssociatedToWayPtr node);
    QPixmap pixmap(NodeAssociatedToWayPtr node);
    void classifyFeatures();
    void classifyPoints();

    void paintMap(QPainter &painter);

    void drawWaterWays(QPainter &painter, FeaturePtr feature);
    void drawHighways(QPainter &painter, FeaturePtr feature);
    void drawGreenAreas(QPainter &painter, FeaturePtr feature);

    void drawAerialway(QPainter &painter, FeaturePtr feature);
    void drawAeroway(QPainter &painter, FeaturePtr feature);
    void drawAmenity(QPainter &painter, FeaturePtr feature);
    void drawHealthcare(QPainter &painter, FeaturePtr feature);
    void drawBarrier(QPainter &painter, FeaturePtr feature);
    void drawBoundary(QPainter &painter, FeaturePtr feature);
    void drawAdmin_level(QPainter &painter, FeaturePtr feature);
    void drawBuilding(QPainter &painter, FeaturePtr feature);
    void drawEmergency(QPainter &painter, FeaturePtr feature);
    void drawGeological(QPainter &painter, FeaturePtr feature);
    void drawHistoric(QPainter &painter, FeaturePtr feature);
    void drawLanduse(QPainter &painter, FeaturePtr feature);
    void drawLeisure(QPainter &painter, FeaturePtr feature);
    void drawMan_made(QPainter &painter, FeaturePtr feature);
    void drawMilitary(QPainter &painter, FeaturePtr feature);
    void drawNatural(QPainter &painter, FeaturePtr feature);
    void drawOffice(QPainter &painter, FeaturePtr feature);
    void drawPlace(QPainter &painter, FeaturePtr feature);
    void drawPower(QPainter &painter, FeaturePtr feature);
    void drawPublic_transport(QPainter &painter, FeaturePtr feature);
    void drawRailway(QPainter &painter, FeaturePtr feature);
    void drawBridge(QPainter &painter, FeaturePtr feature);

    void drawPolygon(QPainter &painter, FeaturePtr feature, QColor &borderColor, QColor &color);
    void drawPolyline(QPainter &painter, FeaturePtr feature, QColor &color, int penWidth);

private:
    double _maxDistance;
    double _carX;
    double _carY;

    QList<WayPtr> _ways;
    double _vehicleDirection;
    double _rotation;

    QList<WayPtr> _linearFeatures;
    QList<NodeAssociatedToWayPtr> _pointFeatures;

    QList<FeaturePtr> _waterWays;
    QList<FeaturePtr> _highways;
    QList<FeaturePtr> _greenAreas;

    QList<FeaturePtr> _aerialway;
    QList<FeaturePtr> _aeroway;
    QList<FeaturePtr> _amenity;
    QList<FeaturePtr> _healthcare;
    QList<FeaturePtr> _barrier;
    QList<FeaturePtr> _boundary;
    QList<FeaturePtr> _admin_level;
    QList<FeaturePtr> _building;
    QList<FeaturePtr> _emergency;
    QList<FeaturePtr> _geological;
    QList<FeaturePtr> _historic;
    QList<FeaturePtr> _landuse;
    QList<FeaturePtr> _leisure;

    QList<FeaturePtr> _man_made;
    QList<FeaturePtr> _military;
    QList<FeaturePtr> _natural;
    QList<FeaturePtr> _office;
    QList<FeaturePtr> _place;
    QList<FeaturePtr> _power;
    QList<FeaturePtr> _public_transport;
    QList<FeaturePtr> _railway;
    QList<FeaturePtr> _bridge;


    QList<double> _prevX;
    QList<double> _prevY;
};

#endif // MAPVIEWWIDGET_H
