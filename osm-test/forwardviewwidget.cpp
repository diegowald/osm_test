#include "forwardviewwidget.h"
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QtMath>

ForwardViewWidget::ForwardViewWidget(QWidget *parent) : MapViewWidget(parent)
{
    _selectedWay = -1;
}

QPointF ForwardViewWidget::transformToWidgetCoords(QPointF realPoint)
{
    double xR1 = realPoint.x() - _carX;
    double yR1 = realPoint.y() - _carY;

    double angle = /*copysign(*/_rotation/*, _vehicleDirection);*/;

    double rotatedX = + cos(-angle) * xR1 - sin(-angle) * yR1;
    double rotatedY = + sin(-angle) * xR1 + cos(-angle) * yR1;

    double x0 = rotatedX * scale();
    double y0 = rotatedY * scale();

    return QPointF(x0 + geometry().width() / 2., geometry().height() / 2 - y0);
}

void ForwardViewWidget::paintEvent(QPaintEvent *evt)
{
    MapViewWidget::paintEvent(evt);

    QPainter painter(this);
    double deg = qRadiansToDegrees(_rotation);
    painter.drawText(0, 20, QString::number(deg));

    foreach (FeaturePtr feature, _highways)
    {
        if (feature->id() == _selectedWay)
        {
            QColor color = QColor::fromRgb(10, 10, 10);
            int w = 25;
            drawPolyline(painter, feature, color, w);
            break;
        }
    }
}

void ForwardViewWidget::setSelectedWay(long id)
{
    _selectedWay = id;
}
