#ifndef COSINEFILTER_H
#define COSINEFILTER_H

#include <math.h>
#include "FalloffFilter.h"

class CosineFilter : public FalloffFilter {

public:
   virtual double filter(double f) const {
      return 1.f - cos(f * M_PI / 2.f);
   }
};

#endif // COSINEFILTER_H
