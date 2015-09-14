#ifndef NODEASSOCIATEDTOWAY_H
#define NODEASSOCIATEDTOWAY_H

#include <QObject>
#include <QMap>
#include <QSharedPointer>


class NodeAssociatedToWay : public QObject
{
    Q_OBJECT
public:
    explicit NodeAssociatedToWay(QObject *parent = 0);

    void setCoords(double &X, double &Y);
    void setId(long id);
    void addKeyValue(const QString &key, const QString &value);

    double X() const;
    double Y() const;
    QString toString() const;
    QString value(const QString &key);
signals:

public slots:

private:
    double _x;
    double _y;
    long _id;
    QMap<QString, QString> values;
};

typedef QSharedPointer<NodeAssociatedToWay> NodeAssociatedToWayPtr;

#endif // NODEASSOCIATEDTOWAY_H
