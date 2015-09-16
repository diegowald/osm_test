#ifndef FORWARDVIEW_H
#define FORWARDVIEW_H

#include <QFrame>
#include <QGraphicsView>
#include <QList>
#include "nodeassociatedtoway.h"
#include "way.h"

class ForwardView;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(ForwardView *v) : QGraphicsView(), view(v) { }

protected:
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *) Q_DECL_OVERRIDE;
#endif

private:
    ForwardView *view;
};


class ForwardView : public QFrame
{
    Q_OBJECT
public:
    explicit ForwardView(QWidget *parent = 0);
    QGraphicsView *view() const;

    void updateScene(double &x, double &y, WayPtr way, QList<WayPtr> intersectionsWays, QList<NodeAssociatedToWayPtr> &nodes, QList<NodeAssociatedToWayPtr> &intersections);

signals:

public slots:

private:
    qreal scale(double coord);
    void drawWay(WayPtr way, bool mainWay);
    QPixmap pixmap(NodeAssociatedToWayPtr node);

private:
    GraphicsView *graphicsView;
    QGraphicsScene scene;
};

#endif // FORWARDVIEW_H
