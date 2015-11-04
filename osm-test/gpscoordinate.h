#ifndef GPSCOORDINATE_H
#define GPSCOORDINATE_H

#include <QObject>

class GPSCoordinate : public QObject
{
    Q_OBJECT
public:
    explicit GPSCoordinate(QString time,
    QString latitude,
    QString longitude,
    QString x,
    QString y,
    QString z,
    QString wheel,
    QString speed,
    QObject *parent = 0);

    explicit GPSCoordinate(QString time,
    double latitude,
    double longitude,
    double x,
    double y,
    double z,
    double wheel,
    double speed,
    QObject *parent = 0);

    QString time() const;
    double latitude() const;
    double longitude() const;
    double x() const;
    double y() const;
    double z() const;
    double wheel() const;
    double speed() const;

    void setDelayToNextCoord(int delta);
    int delayToNextCoord() const;
signals:

public slots:

private:
    QString _time;
    double _latitude;
    double _longitude;
    double _x;
    double _y;
    double _z;
    double _wheel;
    double _speed;
    int _delayToNextCoord;
};

typedef QSharedPointer<GPSCoordinate> GPSCoordinatePtr;

#endif // GPSCOORDINATE_H
