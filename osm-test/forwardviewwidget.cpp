#include "forwardviewwidget.h"
#include <QPaintEvent>
#include <QPainter>
#include <QtCore/QtMath>

ForwardViewWidget::ForwardViewWidget(QWidget *parent) : QWidget(parent)
{
    _maxDistance = 1;

    _carX = 0.;
    _carY = 0.;

    _rotation = 0;
}

void ForwardViewWidget::paintEvent(QPaintEvent *evt)
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

    drawWay(painter, _currentWay, QColor(Qt::red));

    foreach (WayPtr w, _ways)
    {
        drawWay(painter, w, QColor(Qt::darkGray));
    }

    foreach (NodeAssociatedToWayPtr node, _nodes) {
        drawSignal(painter, node);
    }

    foreach (NodeAssociatedToWayPtr node, _intersectionNodes)
    {
        drawSignal(painter, node);
    }

    painter.drawText(geo.width() / 2, geo.height() - 20, QString::number(rad2Deg(_rotation)));
    painter.drawText(geo.width() / 2, geo.height() - 10, QString::number(rad2Deg(_vehicleDirection)));

}

void ForwardViewWidget::setMaxDistance(double distance)
{
    _maxDistance = distance;
}

double ForwardViewWidget::scale()
{
    return (geometry().height() * 1.0) / _maxDistance;
}


void ForwardViewWidget::setVehicleCoordinates(double X, double Y)
{
    _carX = X;
    _carY = Y;
}

QPointF ForwardViewWidget::transformToWidgetCoords(QPointF realPoint)
{
    /*double xR1 = realPoint.x() - _carX;
    double yR1 = realPoint.y() - _carY;

    double angle =  3.141592654 / 2 - _rotation;
    if (fabs(_rotation - _vehicleDirection) > 3.141592654 / 2.)
    {
        angle -= 3.141592654;
    }

    double rotatedX = + cos(angle) * xR1 - sin(angle) * yR1;
    double rotatedY = + sin(angle) * xR1 + cos(angle) * yR1;

    double x0 = rotatedX * scale();
    double y0 = rotatedY * scale();

    return QPointF(x0 + geometry().width() / 2., geometry().height() - y0);
    */

    double xR1 = realPoint.x() - _carX;
    double yR1 = realPoint.y() - _carY;

    double angle =  3.141592654 / 2 - _rotation;
    if (fabs(_rotation - _vehicleDirection) > 3.141592654 / 2.)
    {
        angle -= 3.141592654;
    }

    double rotatedX = + cos(-angle) * xR1 - sin(-angle) * yR1;
    double rotatedY = + sin(-angle) * xR1 + cos(-angle) * yR1;

    double x0 = rotatedX * scale();
    double y0 = rotatedY * scale();

    return QPointF(x0 + geometry().width() / 2., geometry().height() / 2 - y0);
}


void ForwardViewWidget::drawSignal(QPainter &painter, NodeAssociatedToWayPtr node)
{
    QPointF pt = transformToWidgetCoords(QPointF(node->X(), node->Y()));
    QPoint p(pt.x(), pt.y());
    if (painter.window().contains(p))
    {
        QPixmap pix = pixmap(node);
        pt.setX(pt.x() - pix.width()/2);
        pt.setY(pt.y() - pix.height()/2);
        painter.drawPixmap(pt, pix);
    }
}

void ForwardViewWidget::drawWay(QPainter &painter, WayPtr way, QColor color)
{
    if (way.isNull())
        return;
    if (way->points().count() == 0)
        return;

    QString lanes = way->value("lanes", "1");

    OSMPointPtr p = way->points().at(0);
    QPointF pt0 = transformToWidgetCoords(QPointF(p->x(), p->y()));
    for (int i = 1; i < way->points().count(); ++i)
    {
        p = way->points().at(i);
        QPointF pt1 = transformToWidgetCoords(QPointF(p->x(), p->y()));
        painter.setPen(QPen(color, 10 * lanes.toDouble()));
        painter.drawLine(pt0, pt1);
        pt0 = pt1;
    }
}

void ForwardViewWidget::setCurrentWay(WayPtr way)
{
    _currentWay = way;
}

void ForwardViewWidget::setIntersectionWays(QList<WayPtr> &ways)
{
    _ways = ways;
}

void ForwardViewWidget::setRotation(double alpha)
{
    _rotation  = alpha;
}

void ForwardViewWidget::setIntersectionNodes(QList<NodeAssociatedToWayPtr> &nodes)
{
    _intersectionNodes = nodes;
}

void ForwardViewWidget::setSignals(QList<NodeAssociatedToWayPtr> &nodes)
{
    _nodes = nodes;
}

void ForwardViewWidget::setVehicleDirection(double direction)
{
    _vehicleDirection = direction;
}

QPixmap ForwardViewWidget::pixmap(NodeAssociatedToWayPtr node)
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

double ForwardViewWidget::rad2Deg(double rad)
{
    return qRadiansToDegrees(rad);
}
