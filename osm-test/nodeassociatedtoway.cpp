#include "nodeassociatedtoway.h"

NodeAssociatedToWay::NodeAssociatedToWay(QObject *parent) : QObject(parent)
{
}

void NodeAssociatedToWay::setCoords(double &X, double &Y)
{
    _x = X;
    _y = Y;
}

void NodeAssociatedToWay::setId(long id)
{
    _id = id;
}

void NodeAssociatedToWay::addKeyValue(const QString &key, const QString &value)
{
    values[key] = value;
}


double NodeAssociatedToWay::X() const
{
    return _x;
}


double NodeAssociatedToWay::Y() const
{
    return _y;
}

QString NodeAssociatedToWay::toString() const
{
    QString s;
    QString templ = "%1: %2, ";
    foreach (QString k, values.keys())
    {
        s += templ.arg(k, values[k]);
    }
    return s;
}

QString NodeAssociatedToWay::value(const QString &key)
{
    return values.contains(key) ? values[key] : "x";
}
