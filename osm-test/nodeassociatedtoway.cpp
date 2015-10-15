#include "nodeassociatedtoway.h"

NodeAssociatedToWay::NodeAssociatedToWay(QObject *parent) : Feature(parent)
{
}

void NodeAssociatedToWay::setCoords(double &X, double &Y)
{
    _x = X;
    _y = Y;
}

double NodeAssociatedToWay::X() const
{
    return _x;
}


double NodeAssociatedToWay::Y() const
{
    return _y;
}

int NodeAssociatedToWay::numPoints()
{
    return 1;
}
