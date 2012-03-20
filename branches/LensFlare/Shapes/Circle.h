#ifndef LensFlare_Circle_h
#define LensFlare_Circle_h

#include "Shape2D.h"

class Circle : public Shape2D {
  
public:
   Circle(int x, int y, int r);
   virtual ~Circle();
   
   virtual bool hit(float x, float y, Color& color) const;
   
   void setInnerColor(const Color& c) { innerColor = c; }
   void setOuterColor(const Color& c) { outerColor = c; }

private:
   int cx, cy;
   int radius;
   Color innerColor;
   Color outerColor;
};

#endif
