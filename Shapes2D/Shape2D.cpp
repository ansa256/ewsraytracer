#include "Shape2D.h"
#include "Falloff/FalloffFilter.h"

Shape2D::Shape2D() : filter(NULL) {}

Shape2D::~Shape2D() {
   if(filter != NULL) {
      delete filter;
   }
}
