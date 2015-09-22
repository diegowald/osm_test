#include "gpscoordinate.h"

GPSCoordinate::GPSCoordinate(QString time, QString latitude, QString longitude, QString x, QString y, QString z, QString wheel, QString speed, QObject *parent) : QObject(parent)
{
    _time = time;
    _latitude = latitude.toDouble();
    _longitude = longitude.toDouble();
    _x = x.toDouble();
    _y = y.toDouble();
    _z = z.toDouble();
    _wheel = wheel.toDouble();
    _speed = speed.toDouble();
}


QString GPSCoordinate::time() const
{
    return _time;
}

double GPSCoordinate::latitude() const
{
    return _latitude;
}

double GPSCoordinate::longitude() const
{
    return _longitude;
}

double GPSCoordinate::x() const
{
    return _x;
}

double GPSCoordinate::y() const
{
    return _y;
}

double GPSCoordinate::z() const
{
    return _z;
}

double GPSCoordinate::wheel() const
{
    return _wheel;
}

double GPSCoordinate::speed() const
{
    return _speed;
}
