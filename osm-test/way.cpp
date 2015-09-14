#include "way.h"

Way::Way(OSMHandlerPtr osmHandler, long way_id, QObject *parent) : QObject(parent)
{
    _handler = osmHandler;
    _id = way_id;
    _points = _handler->getWayNodes(way_id);
}


bool Way::pointInWay(double &x, double &y, double &direction)
{
    //-> obtengo el segmento en el que puede estar el elemento.
    //-> si no esta en ningun segmento, no pertenece al camino.
    //-> cheqeuo la alineacion.
    //-> si el vehiculo esta dentro de un rango -> true
    return false;
}

bool Way::pointInWay(double &x, double &y)
{
    return false;
}
