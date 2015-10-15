#include "mapviewwidget.h"
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

MapViewWidget::MapViewWidget(QWidget *parent) : QWidget(parent)
{
    _maxDistance = 1;

    _carX = 0.;
    _carY = 0.;

    _rotation = 0;
}

void MapViewWidget::paintEvent(QPaintEvent *evt)
{
    QPainter painter(this);
    QRect geo = this->geometry();

    paintMap(painter);


    QPixmap pix(":/signals/car");
    QPointF pt;
    //    pt.setX((width() - pix.width())/2);
    //    pt.setY((height() - pix.height())/2);

    pt.setX(_carX);
    pt.setY(_carY);
    pt = transformToWidgetCoords(pt);
    pt.setX(pt.x() - pix.width() / 2);
    pt.setY(pt.y() - pix.height() / 2);
    painter.drawPixmap(pt, pix);

    for (int i = 0; i < _prevX.count(); ++i)
    {
        pt.setX(_prevX.at(i));
        pt.setY(_prevY.at(i));
        pt = transformToWidgetCoords(pt);
        painter.drawEllipse(pt, 3, 3);
    }
}

void MapViewWidget::setMaxDistance(double distance)
{
    _maxDistance = distance;
}

double MapViewWidget::scale()
{
    return (geometry().height() * 1.0) / _maxDistance;
}


void MapViewWidget::setVehicleCoordinates(double X, double Y)
{
    _prevX.append(_carX);
    _prevY.append(_carY);
    _carX = X;
    _carY = Y;
}

QPointF MapViewWidget::transformToWidgetCoords(QPointF realPoint)
{
    double xR1 = realPoint.x() - _carX;
    double yR1 = realPoint.y() - _carY;

    double rotatedX = + cos(-_vehicleDirection) * xR1 - sin(-_vehicleDirection) * yR1;
    double rotatedY = + sin(-_vehicleDirection) * xR1 + cos(-_vehicleDirection) * yR1;

    double x0 = rotatedX * scale();
    double y0 = rotatedY * scale();

    return QPointF(x0 + geometry().width() / 2., geometry().height() / 2 - y0);
}


void MapViewWidget::drawSignal(QPainter &painter, NodeAssociatedToWayPtr node)
{
    QPointF pt = transformToWidgetCoords(QPointF(node->X(), node->Y()));
    QPixmap pix = pixmap(node);
    pt.setX(pt.x() - pix.width()/2);
    pt.setY(pt.y() - pix.height()/2);
    painter.drawPixmap(pt, pix);
}

void MapViewWidget::drawWay(QPainter &painter, FeaturePtr feature, QColor color)
{
    if (feature.isNull())
        return;
    if (feature->numPoints() > 1)
    {
        WayPtr way = qSharedPointerDynamicCast<Way>(feature);
        OSMPointPtr p = way->points().at(0);
        QPointF pt0 = transformToWidgetCoords(QPointF(p->x(), p->y()));
        for (int i = 1; i < way->points().count(); ++i)
        {
            p = way->points().at(i);
            QPointF pt1 = transformToWidgetCoords(QPointF(p->x(), p->y()));
            QPen pen(color);
            pen.setWidth(2);
            painter.setPen(pen);
            painter.drawLine(pt0, pt1);
            pt0 = pt1;
        }
    }
}


void MapViewWidget::setWays(QList<WayPtr> &ways)
{
    _ways = ways;
}

void MapViewWidget::setRotation(double alpha)
{
    _rotation  = alpha;
}

void MapViewWidget::setVehicleDirection(double direction)
{
    _vehicleDirection = direction;
}

QPixmap MapViewWidget::pixmap(NodeAssociatedToWayPtr node)
{
    QString signalType = node->value("highway", "");
    QPixmap pix;
    if (signalType == "traffic_signals")
    {
        pix = QPixmap(":/signals/stop");
    }
    else
    {
        pix =  QPixmap(":/signals/noParking");
    }
    /*    turning_circle
    bus_stop
    speed_camera
    street_lamp
*/
    return pix.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void MapViewWidget::setLinearFeatures(QList<WayPtr> &features)
{
    _linearFeatures = features;
    classifyFeatures();
}

void MapViewWidget::setPointFeatures(QList<NodeAssociatedToWayPtr> &pts)
{
    _pointFeatures = pts;
    classifyPoints();
}


//based on http://wiki.openstreetmap.org/wiki/Map_Features
void MapViewWidget::classifyFeatures()
{
    _highways.clear();
    _waterWays.clear();

    _waterWays.clear();
    _highways.clear();
    _greenAreas.clear();

    _aerialway.clear();
    _aeroway.clear();
    _amenity.clear();
    _healthcare.clear();
    _barrier.clear();
    _boundary.clear();
    _admin_level.clear();
    _building.clear();
    _emergency.clear();
    _geological.clear();
    _historic.clear();
    _landuse.clear();
    _leisure.clear();

    _man_made.clear();
    _military.clear();
    _natural.clear();
    _office.clear();
    _place.clear();
    _power.clear();
    _public_transport.clear();
    _railway.clear();
    _bridge.clear();


    foreach (WayPtr way, _linearFeatures)
    {
        if (way->value("aerialway", "").length() > 0)
        {
            _aerialway.append(way);
        }
        else if (way->value("aeroway", "").length() > 0)
        {
            _aeroway.append(way);
        }
        else if (way->value("amenity", "").length() > 0)
        {
            _amenity.append(way);
        }
        else if (way->value("healthcare", "").length() > 0)
        {
            _healthcare.append(way);
        }
        else if (way->value("barrier", "").length() > 0)
        {
            _barrier.append(way);
        }
        else if (way->value("boundary", "").length() > 0)
        {
            _boundary.append(way);
        }
        else if (way->value("admin_level", "").length() > 0)
        {
            _admin_level.append(way);
        }
        else if (way->value("building", "").length() > 0)
        {
            _building.append(way);
        }
        else if (way->value("emergency", "").length() > 0)
        {
            _emergency.append(way);
        }
        else if (way->value("geological", "").length() > 0)
        {
            _geological.append(way);
        }
        else if (way->value("historic", "").length() > 0)
        {
            _historic.append(way);
        }
        else if (way->value("landuse", "").length() > 0)
        {
            _landuse.append(way);
        }
        else if (way->value("leisure", "").length() > 0)
        {
            _leisure.append(way);
        }
        else if (way->value("man_made", "").length() > 0)
        {
            _man_made.append(way);
        }
        else if (way->value("military", "").length() > 0)
        {
            _military.append(way);
        }
        else if (way->value("natural", "").length() > 0)
        {
            _natural.append(way);
        }
        else if (way->value("office", "").length() > 0)
        {
            _office.append(way);
        }
        else if (way->value("place", "").length() > 0)
        {
            _place.append(way);
        }
        else if (way->value("power", "").length() > 0)
        {
            _power.append(way);
        }
        else if (way->value("public_transport", "").length() > 0)
        {
            _public_transport.append(way);
        }
        else if (way->value("railway", "").length() > 0)
        {
            _railway.append(way);
        }
        else if (way->value("bridge", "").length() > 0)
        {
            _bridge.append(way);
        }
        else if (way->value("highway", "").length() > 0)
        {
            _highways.append(way);
        }
        else if (way->value("waterway", "").length() > 0)
        {
            _waterWays.append(way);
        }
        else
        {
            qDebug() << way->toString();
        }
    }
}

void MapViewWidget::classifyPoints()
{
}

void MapViewWidget::paintMap(QPainter &painter)
{
    // Background
    painter.setBackground(QBrush(QColor(Qt::red), Qt::SolidPattern));
    painter.setBackgroundMode(Qt::OpaqueMode);

    QRect geo;
    geo.setLeft(0);
    geo.setTop(0);
    geo.setWidth(width());
    geo.setHeight(height());
    painter.fillRect(geo, Qt::red);

    foreach (FeaturePtr w, _waterWays)
    {
        drawWay(painter, w, QColor(Qt::blue));
    }

    foreach (FeaturePtr w, _highways)
    {
        drawWay(painter, w, QColor(Qt::darkGray));
    }

    /*foreach (FeaturePtr w, _ways)
    {
        drawWay(painter, w, QColor(Qt::darkGray));
    }*/

    foreach (NodeAssociatedToWayPtr node, _pointFeatures)
    {
        drawSignal(painter, node);
    }
}

void MapViewWidget::drawWaterWays(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawHighways(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawGreenAreas(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawAerialway(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawAeroway(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawAmenity(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawHealthcare(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawBarrier(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawBoundary(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawAdmin_level(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawBuilding(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawEmergency(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawGeological(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawHistoric(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawLanduse(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawLeisure(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawMan_made(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawMilitary(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawNatural(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawOffice(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawPlace(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawPower(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawPublic_transport(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawRailway(QPainter &painter, FeaturePtr feature)
{
}

void MapViewWidget::drawBridge(QPainter &painter, FeaturePtr feature)
{
}
