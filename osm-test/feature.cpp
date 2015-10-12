#include "feature.h"

Feature::Feature(QObject *parent) : QObject(parent)
{

}

void Feature::setId(long id)
{
    _id = id;
}

void Feature::addKeyValue(const QString &key, const QString &value)
{
    values[key] = value;
}

QString Feature::toString() const
{
    QString s;
    QString templ = "%1: %2, ";
    foreach (QString k, values.keys())
    {
        s += templ.arg(k, values[k]);
    }
    return s;
}

QString Feature::value(const QString &key, const QString &defaultValue)
{
    return values.contains(key) ? values[key] : defaultValue;
}

long Feature::id() const
{
    return _id;
}
