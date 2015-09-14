#ifndef FORWARDVIEW_H
#define FORWARDVIEW_H

#include <QFrame>
#include <QGraphicsView>
#include <QList>
#include "nodeassociatedtoway.h"

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

    void updateScene(double &x, double &y, QList<NodeAssociatedToWayPtr> &nodes, QList<NodeAssociatedToWayPtr> &intersections);

signals:

public slots:

private:
    GraphicsView *graphicsView;
    QGraphicsScene scene;
};

#endif // FORWARDVIEW_H
