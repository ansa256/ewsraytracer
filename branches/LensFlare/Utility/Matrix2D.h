#ifndef LensFlare_Matrix2D_h
#define LensFlare_Matrix2D_h

#include "Point2D.h"

class Matrix2D {
   
public:
   double m[2][2];

   Matrix2D();
   
   void setIdentity();
   void rotate(double r);
   
   Point2D operator*(const Point2D& p);
};

#endif
