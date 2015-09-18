#include "forwardview.h"
#include <QDebug>

#include <QLayout>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QPrintDialog>
#endif
#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#else
#include <QtWidgets>
#endif
#include <qmath.h>

#ifndef QT_NO_WHEELEVENT

void GraphicsView::wheelEvent(QWheelEvent *e)
{
/*    if (e->modifiers() & Qt::ControlModifier) {
        if (e->delta() > 0)
            view->zoomIn(6);
        else
            view->zoomOut(6);
        e->accept();
    } else {
        QGraphicsView::wheelEvent(e);
    }*/
}
#endif


ForwardView::ForwardView(QWidget *parent) : QFrame(parent)
{
    graphicsView = new GraphicsView(this);
    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    graphicsView->setScene(&scene);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(graphicsView);
    setLayout(layout);
}

QGraphicsView *ForwardView::view() const
{
    return static_cast<QGraphicsView *>(graphicsView);
}

void ForwardView::updateScene(double &x, double &y, double &maxDistance,
                              WayPtr way, double &orientation,
                              QList<WayPtr> intersectionsWays,
                              QList<NodeAssociatedToWayPtr> &nodes,
                              QList<NodeAssociatedToWayPtr> &intersections)
{
    //scene.clear();

    QGraphicsItem *wayItem = drawWay(way, true);

    foreach (WayPtr w, intersectionsWays)
    {
        drawWay(w, false);
    }

    QPen pen1(Qt::blue);
    foreach (NodeAssociatedToWayPtr node, nodes)
    {
        qreal x = scale(node->X());
        qreal y = scale(node->Y());
        //QGraphicsLineItem *item = scene.addLine(0, 0, 1, 1, pen1);

        QGraphicsPixmapItem * item = scene.addPixmap(pixmap(node));
        item->setPos(x, y);
        QMatrix matrix;
        matrix.scale(.002, .0020);
        matrix.rotate(180);
        item->setMatrix(matrix);
        item->setPos(x, y);

        qDebug() << "signal " << x << ", " << y;
        qDebug() << "item rect" << item->boundingRect();
    }

    QPen pen2(Qt::red);
    foreach (NodeAssociatedToWayPtr node, intersections)
    {
        /*qreal x = scale(node->X());
        qreal y = scale(node->Y());
        QGraphicsPixmapItem *item = scene.addPixmap(QPixmap(":/signals/stop"));
        qDebug() << item->boundingRect();
        //QGraphicsLineItem *item = scene.addLine(0, 0, .1, .1, pen2);
        QMatrix matrix;
        matrix.scale(.002, .0020);
        matrix.rotate(180);
        item->setMatrix(matrix);
        item->setPos(x, y);
        qDebug() << "int: " << x << ", " << y;
        qDebug() << "item rect" << item->boundingRect();*/
    }

    graphicsView->fitInView(scene.sceneRect(), Qt::KeepAspectRatio);

    qDebug() << "scene " << scene.sceneRect();


    qreal s = 50.;

    QMatrix matrix;
    matrix.scale(s, s);
    double angle = orientation * 180. / 3.141592654 + 180.;
    qDebug() << angle;
    matrix.rotate(-angle);

    graphicsView->setMatrix(matrix);
    //graphicsView->centerOn(scale(x), scale(y));
    graphicsView->ensureVisible(QRectF(scale(x - maxDistance / 2.), scale(y + maxDistance), maxDistance, maxDistance));

    QGraphicsTextItem *i = scene.addText("x");
    i->setScale(.1);
    i->setPos(scale(x), scale(y));
}

qreal ForwardView::scale(double coord)
{
    return coord * 1.e3;
}


QGraphicsLineItem *ForwardView::drawWay(WayPtr way, bool mainWay)
{
    QGraphicsLineItem *item = NULL;
    if (!way.isNull() && way->points().count() > 0)
    {
        QPen roadPen(mainWay ? Qt::darkGray : Qt::green);
        roadPen.setWidthF(mainWay ? .2 : .1);
        QList<OSMPointPtr> points = way->points();
        double x0 = scale(points.at(0)->x());
        double y0 = scale(points.at(0)->y());

        for (int i = 1; i < points.count(); ++i)
        {
            double x = scale(points.at(i)->x());
            double y = scale(points.at(i)->y());

            item = scene.addLine(0, 0, x - x0, y - y0, roadPen);
            qDebug() << "line " << x0 << ", " <<  y0 << ", " << x << ", " << y;
            item->setPos(x0, y0);
            x0 = x;
            y0 = y;
            qDebug() << "item rect" << item->boundingRect();
        }
    }
    return item;
}

QPixmap ForwardView::pixmap(NodeAssociatedToWayPtr node)
{
    QString signalType = node->value("highway");

    if (signalType == "traffic_signals")
    {
        return QPixmap(":/signals/stop");
    }
    else
    {
        return QPixmap(":/signals/noParking");
    }
/*    turning_circle
    bus_stop
    speed_camera
    street_lamp
*/
}
