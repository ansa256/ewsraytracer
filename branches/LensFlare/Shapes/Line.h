#ifndef LensFlare_Line_h
#define LensFlare_Line_h

#include "Shape2D.h"
#include "Utility/Point2D.h"

class Line : public Shape2D {

public:
   Line(const Point2D& p, int len, int angle);
   virtual ~Line() {}
   
   virtual bool hit(float x, float y, Color& color) const;
   
private:
   Point2D p1, p2;
};

#endif
