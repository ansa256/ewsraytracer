#include "GeometryObject.h"

const double GeometryObject::epsilon = 1.0 * pow(10, -6);

GeometryObject::GeometryObject() : doDelete(true), ignoreShadow(false), bbox() {
}

GeometryObject::~GeometryObject() {
}
