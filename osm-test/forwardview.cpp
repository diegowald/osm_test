#include "forwardview.h"

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

void ForwardView::updateScene(double &x, double &y, QList<NodeAssociatedToWayPtr> &nodes, QList<NodeAssociatedToWayPtr> &intersections)
{
/*    //scene.clear();
    qreal scale = 0.1; //qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));

    QMatrix matrix;
    matrix.scale(scale, scale);
    matrix.rotate(0);

    graphicsView->setMatrix(matrix);


    graphicsView->centerOn(x, y);*/
    scene.clear();
    foreach (NodeAssociatedToWayPtr node, nodes)
    {
        QGraphicsTextItem *item = scene.addText(node->value("highway"));
        item->setPos(node->X(), node->Y());
        item->font().setPointSizeF(0.001);
    }

    foreach (NodeAssociatedToWayPtr node, intersections)
    {
        QGraphicsTextItem *item = scene.addText(node->value("name"));
        item->setPos(node->X(), node->Y());
        item->font().setPointSizeF(0.0001);
    }

/*    graphicsView->fitInView(x - .005, y - 0.005, .01, 0.01);
    graphicsView->setSceneRect(0,0,graphicsView->frameSize().width(),graphicsView->frameSize().height());*/
    graphicsView->fitInView(scene.sceneRect(), Qt::KeepAspectRatio);
}
