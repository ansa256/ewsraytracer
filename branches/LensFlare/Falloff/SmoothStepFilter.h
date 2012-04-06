#ifndef _SMOOTH_STEP_FILTER_H_
#define _SMOOTH_STEP_FILTER_H_

#include "FalloffFilter.h"
#include <math.h>
#include "Utility/Math.h"

class SmoothStepFilter : public FalloffFilter {

public:
   SmoothStepFilter(double e0, double e1) : edge0(e0), edge1(e1) {}

   virtual double filter(double f) const {
      return smootherstep(edge0, edge1, f);
   }

private:
   double edge0, edge1;
};

#endif // _SMOOTH_STEP_FILTER_H_
