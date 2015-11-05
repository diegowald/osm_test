#ifndef FEATURE_H
#define FEATURE_H

#include <QObject>
#include <QMap>


// This elements is the basic feature
class Feature : public QObject
{
    Q_OBJECT
public:
    explicit Feature(QObject *parent = 0);


    void setId(long id);

    // adds the key, value pair to feature dictionary.
    void addKeyValue(const QString &key, const QString &value);

    // Converts feature to string, for debugging purposes.
    QString toString() const;

    // retrieves the value from dictionary.
    QString value(const QString &key, const QString &defaultValue);

    long id() const;

    virtual int numPoints() = 0;
signals:

public slots:

private:
    long _id;
    QMap<QString, QString> values;
};

typedef QSharedPointer<Feature> FeaturePtr;

#endif // FEATURE_H
