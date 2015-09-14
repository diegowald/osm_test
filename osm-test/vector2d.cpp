#include "vector2d.h"

Vector2D::Vector2D(QObject *parent) : QObject(parent)
{
    _x = 0.;
    _y = 0.;
}

Vector2D::Vector2D(double x, double y, QObject *parent) : QObject(parent)
{
    _x = x;
    _y = y;
}

Vector2D::Vector2D(double x1, double x2, double y1, double y2, QObject *parent) : QObject(parent)
{
    setBy2Points(x1, y1, x2, y2);
}

double Vector2D::module() const
{
    return sqrt(_x * _x + _y * _y);
}

double Vector2D::scalar(Vector2D &other) const
{
    return _x * other._x + _y * other._y;
}

double Vector2D::x() const
{
    return _x;
}

double Vector2D::y() const
{
    return _y;
}

void Vector2D::setX(double value)
{
    _x = value;
}

void Vector2D::setY(double value)
{
    _y = value;
}

void Vector2D::setBy2Points(double x1, double y1, double x2, double y2)
{
    _x = x2 - x1;
    _y = y2 - y1;
}

Vector2D Vector2D::versor() const
{
    Vector2D v;
    double _module = module();
    v.setX(_x / _module);
    v.setY(_y / _module);
    return v;
}

Vector2D::Vector2D(Vector2D &other) : QObject(other.parent())
{
    this->_x = other._x;
    this->_y = other._y;
}


double Vector2D::angle() const
{
    if (fabs(_x) < 0.0001)
    {
        return ((_y < 0) ? -1 : 1) *  3.141592654 / 2;
    }
    else
    {
        return atan(_y / _x);
    }
}
