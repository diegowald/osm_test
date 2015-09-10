#include "nodeassociatedtoway.h"

NodeAssociatedToWay::NodeAssociatedToWay(QObject *parent) : QObject(parent)
{

}

void NodeAssociatedToWay::addKeyValue(const QString &key, const QString &value)
{
    values[key] = value;
}


double NodeAssociatedToWay::X() const
{
    return x;
}


double NodeAssociatedToWay::Y() const
{
    return y;
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
