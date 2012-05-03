#ifndef raytracer_Sun_h
#define raytracer_Sun_h

#include "Overlay.h"
#include "Utility/Color.h"

class Sun : public Overlay {
   
public:
   Sun();
   
   virtual void setHash(Hash* hash);
   void create();
   
private:
   int radius;
   int spikeLength;
   int numSpikes;
   float startAngle;
   Color color;
};

#endif
