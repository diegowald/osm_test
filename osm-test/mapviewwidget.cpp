#include "mapviewwidget.h"
#include <QPaintEvent>
#include <QPainter>
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

/*    int x, y, width, height;

    x = geo.x()-10;
    y = geo.y()-10;
    width = geo.width()-3;
    height = geo.height()-5;

    painter.fillRect(x, y, width, height, QColor(220,220,220));

    painter.drawText(x+10, y+10, "Machine " + QString::number(1234));

    painter.drawLine(
                transformToWidgetCoords(QPointF(0,0)),
                transformToWidgetCoords(QPointF(0.5, 0.3)));
*/
    foreach (WayPtr w, _ways)
    {
        drawWay(painter, w, QColor(Qt::darkGray));
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
    _carX = X;
    _carY = Y;
}

QPointF MapViewWidget::transformToWidgetCoords(QPointF realPoint)
{
    double xR1 = realPoint.x() - _carX;
    double yR1 = realPoint.y() - _carY;

    double rotatedX = cos(-_rotation) * xR1 - sin(-_rotation) * yR1;
    double rotatedY = sin(-_rotation) * xR1 + cos(-_rotation) * yR1;

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

void MapViewWidget::drawWay(QPainter &painter, WayPtr way, QColor color)
{
    if (way.isNull())
        return;
    if (way->points().count() == 0)
        return;
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
    QString signalType = node->value("highway");
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
