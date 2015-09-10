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

    void addKeyValue(const QString &key, const QString &value);

    double X() const;
    double Y() const;
    QString toString() const;
signals:

public slots:

private:
    double x;
    double y;
    QMap<QString, QString> values;
};

typedef QSharedPointer<NodeAssociatedToWay> NodeAssociatedToWayPtr;

#endif // NODEASSOCIATEDTOWAY_H
