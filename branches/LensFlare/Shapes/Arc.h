#ifndef LensFlare_Arc_h
#define LensFlare_Arc_h

#include "Shape2D.h"

class Arc : public Shape2D {
  
public:
   Arc(int x, int y, int r, float a1, float a2);
   virtual ~Arc();
   
   virtual bool hit(float x, float y, Color& color) const;
   
   void setInnerColor(const Color& c) { innerColor = c; }
   void setOuterColor(const Color& c) { outerColor = c; }

private:
   int cx, cy;
   int radius;
   float angle1, angle2;
   Color innerColor;
   Color outerColor;
};

#endif

