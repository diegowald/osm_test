#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <QObject>

class Vector2D : public QObject
{
    Q_OBJECT
public:
    explicit Vector2D(QObject *parent = 0);
    Vector2D(double x, double y, QObject *parent = 0);
    Vector2D(double x1, double x2, double y1, double y2, QObject *parent = 0);

    Vector2D(Vector2D &other);

    double module() const;
    double scalar(Vector2D &other) const;

    double x() const;
    double y() const;

    void setX(double value);
    void setY(double value);

    void setBy2Points(double x1, double y1, double x2, double y2);

    Vector2D versor() const;

    double angle() const;
signals:

public slots:

private:
    double _x;
    double _y;
};

#endif // VECTOR2D_H
