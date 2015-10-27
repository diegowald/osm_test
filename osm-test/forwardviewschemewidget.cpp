#include "forwardviewschemewidget.h"
#include <QPainter>
#include <QDebug>

ForwardViewSchemeWidget::ForwardViewSchemeWidget(QWidget *parent) : QWidget(parent)
{

}

void ForwardViewSchemeWidget::paintEvent(QPaintEvent *evt)
{
    QPainter painter(this);
    QRect geo = this->geometry();

    _streetWidth = width() - 30;

    // Background
    painter.setBackground(QBrush(QColor(Qt::lightGray), Qt::SolidPattern));
    painter.setBackgroundMode(Qt::OpaqueMode);

    geo.setLeft(0);
    geo.setTop(0);
    geo.setWidth(width());
    geo.setHeight(height());
    painter.fillRect(geo, QColor::fromRgb(220, 200, 240));

    if (_selectedWay > 0)
    {
        drawStreet();
        drawCenterLine();
        drawBoundaryLine();
        drawLanes();
        drawSignals();
        drawVehicle();
        drawVehicleDirection();
    }
}

void ForwardViewSchemeWidget::setVehicleCoordinates(double X, double Y)
{
}

void ForwardViewSchemeWidget::setVehicleDirection(double direction)
{
    _vehicleDirection = direction;
}

void ForwardViewSchemeWidget::setWays(QList<WayPtr> &ways)
{
    _ways.clear();
    foreach (WayPtr way, ways)
    {
        _ways[way->id()] = way;
    }
}

void ForwardViewSchemeWidget::setRotation(double alpha)
{
    _StreetRotation = alpha;
}

void ForwardViewSchemeWidget::setLinearFeatures(QList<WayPtr> &features)
{
}

void ForwardViewSchemeWidget::setPointFeatures(QList<NodeAssociatedToWayPtr> &pts)
{
}

void ForwardViewSchemeWidget::setSelectedWay(long id)
{
    _selectedWay = id;
}


void ForwardViewSchemeWidget::drawStreet()
{
    QPainter painter(this);
    QRect geo;
    geo.setLeft(0);
    geo.setTop(0);
    geo.setWidth(_streetWidth);
    geo.setHeight(height());
    painter.fillRect(geo, QColor::fromRgb(64, 64, 64));
}

void ForwardViewSchemeWidget::drawCenterLine()
{
    QPainter painter(this);
    QRect geo;
    geo.setLeft(3);
    geo.setTop(0);
    geo.setWidth(6);
    geo.setHeight(height());
    painter.fillRect(geo, QColor::fromRgb(255, 204, 64));
}

void ForwardViewSchemeWidget::drawLanes()
{
    WayPtr way = _ways[_selectedWay];
    int numlanes = way->value("lanes", "1").toInt() - 1;
    if (numlanes > 0)
    {
        QPainter painter(this);
        QRect geo;
        int w = _streetWidth / numlanes;
        for (int i = 0; i < numlanes; ++i)
        {
            geo.setLeft(6 + i * w);
            geo.setTop(0);
            geo.setWidth(6);
            geo.setHeight(height());
            painter.fillRect(geo, QColor::fromRgb(255, 255, 255));
            painter.drawText(6 + i * w/ 2, 30, QString::number(i + 1));
        }
    }
}

void ForwardViewSchemeWidget::drawBoundaryLine()
{
    WayPtr way = _ways[_selectedWay];

    QPainter painter(this);
    QRect geo;
    geo.setLeft(_streetWidth - 9);
    geo.setTop(0);
    geo.setWidth(6);
    geo.setHeight(height());
    painter.fillRect(geo, QColor::fromRgb(85, 142, 213));
}

void ForwardViewSchemeWidget::drawSignals()
{
}

void ForwardViewSchemeWidget::drawVehicle()
{
    QPainter painter(this);
    QPixmap pix(":/signals/redCar");
    pix = pix.scaled(40, 60, Qt::KeepAspectRatio);
    QPointF pt;
    //    pt.setX((width() - pix.width())/2);
    //    pt.setY((height() - pix.height())/2);

    pt.setX((_streetWidth - pix.width()) / 2);
    pt.setY(height() - pix.height() - 3);
    painter.drawPixmap(pt, pix);
}

void ForwardViewSchemeWidget::drawVehicleDirection()
{
    QPainter painter(this);
    QPixmap pix(":/signals/arrow");
    pix = pix.scaled(40, 60, Qt::KeepAspectRatio);
    QPointF pt;
    //    pt.setX((width() - pix.width())/2);
    //    pt.setY((height() - pix.height())/2);

    qDebug() << _StreetRotation << ", " << _vehicleDirection;

    double streeAngle = _StreetRotation * 180 / 3.141592654;
    double carAngle = _vehicleDirection * 180 / 3.141592654;
    qDebug() << "Angle: " << streeAngle - carAngle;
    double a = streeAngle - carAngle;
    if (fabs(a) > 90)
        a += 180.0;

    QTransform transform;
    QTransform t = transform.rotate(a);
    pix = pix.transformed(t);

    pt.setX((_streetWidth - pix.width()) / 2);
    pt.setY(height() - pix.height() - 3);
    painter.drawPixmap(pt, pix);
}
