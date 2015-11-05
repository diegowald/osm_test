#ifndef FORWARDVIEWSCHEMEWIDGET_H
#define FORWARDVIEWSCHEMEWIDGET_H
#include "way.h"

#include <QWidget>
#include <QMap>

class ForwardViewSchemeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ForwardViewSchemeWidget(QWidget *parent = 0);

    // This is the most important function
    virtual void paintEvent(QPaintEvent *evt);

    // establish the current vehicle position
    void setVehicleCoordinates(double X, double Y);

    // establish the current vehicle northing.
    void setVehicleDirection(double direction);

    // Sets the ways nearby.
    void setWays(QList<WayPtr> &ways);


    void setRotation(double alpha);

    void setLinearFeatures(QList<WayPtr> &features);
    void setPointFeatures(QList<NodeAssociatedToWayPtr> &pts);

    void setSelectedWay(long id);

private:
    void drawStreet();
    void drawCenterLine();
    void drawBoundaryLine();
    void drawLanes();
    void drawSignals();
    void drawVehicle();
    void drawVehicleDirection();
    void drawLaneNumber(int number, int laneWidth);

signals:

public slots:

private:
    QMap<long, WayPtr> _ways;
    long _selectedWay;
    double _StreetRotation;
    double _vehicleDirection;
    int _streetWidth;
    QList<NodeAssociatedToWayPtr> _symbols;
};

#endif // FORWARDVIEWSCHEMEWIDGET_H
