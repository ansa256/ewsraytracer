#ifndef _LINEAR_FILTER_H
#define _LINEAR_FILTER_H

#include "FalloffFilter.h"

class LinearFilter : public FalloffFilter {

public:
   virtual double filter(double f) const { return f; }
};

#endif // _LINEAR_FILTER_H

