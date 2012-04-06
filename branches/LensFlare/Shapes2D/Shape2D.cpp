#include "Shape2D.h"
#include "Falloff/LinearFilter.h"

Shape2D::Shape2D() : filter(new LinearFilter()) {}

Shape2D::~Shape2D() {
   delete filter;
}

void Shape2D::setFilter(FalloffFilter* f) {
   delete filter;
   filter = f;
}
