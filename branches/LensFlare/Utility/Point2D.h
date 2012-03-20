#ifndef LensFlare_Point2D_h
#define LensFlare_Point2D_h

class Point2D {
   
public:
   Point2D(float _x = 0, float _y = 0) : x(_x), y(_y) {}

   Point2D& operator=(const Point2D& p);
   void set(float _x, float _y);

   float x, y;
};

inline Point2D& Point2D::operator=(const Point2D& p) {
   x = p.x;
   y = p.y;
   return *this;
}

inline void Point2D::set(float _x, float _y) {
   x = _x;
   y = _y;
}

#endif
