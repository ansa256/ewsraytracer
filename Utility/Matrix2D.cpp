#include "Matrix2D.h"
#include <math.h>

Matrix2D::Matrix2D() {
   setIdentity();
}

void Matrix2D::setIdentity() {
   for(int y = 0; y < 2; y++) {
      for(int x = 0; x < 2; x++) {
         m[x][y] = (x == y) ? 1.0 : 0.0;
      }
   }
}

void Matrix2D::rotate(double r) {
   r *= M_PI / 180.0;
   double cosA = cos(r);
   double sinA = sin(r);
   
   double m00 = m[0][0] * cosA + m[0][1] * sinA;
   double m01 = m[0][0] * -sinA + m[0][1] * cosA;
   double m10 = m[1][0] * cosA + m[1][1] * sinA;
   double m11 = m[1][0] * -sinA + m[1][1] * cosA;
   
   m[0][0] = m00;
   m[0][1] = m01;
   m[1][0] = m10;
   m[1][1] = m11;
}

Point2D Matrix2D::operator*(const Point2D& p) {
   return Point2D(m[0][0] * p.x + m[0][1] * p.y,
                  m[1][0] * p.x + m[1][1] * p.y);
}
