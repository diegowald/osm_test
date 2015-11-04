#include "forwardviewwidget.h"
#include <QPaintEvent>
#include <QPainter>
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

    return QPointF(x0 + geometry().width() / 2., geometry().height() /*/ 2*/ - y0);
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
            int streetWidth = 30 * feature->value("lanes", "1").toInt();
            drawPolyline(painter, feature, color, streetWidth, Qt::SolidLine);

            int w = 2;
            color = QColor::fromRgb(255, 255, 0);
            drawPolylineWithOffset(painter, feature, color, w, Qt::DashLine, streetWidth / 2 - 3);
            drawPolylineWithOffset(painter, feature, color, w, Qt::DashLine, -( streetWidth / 2 - 3));
            break;
        }
    }

    painter.setPen(QColor::fromRgb(255, 0, 20));
    painter.drawEllipse(QPoint(width() / 2, height()), 2, 3);
}

void ForwardViewWidget::setSelectedWay(long id)
{
    _selectedWay = id;
}
