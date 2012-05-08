#include "Sun.h"
#include "Shapes2D/Ellipse.h"
#include "Shapes2D/ThickLine.h"
#include "Utility/FalloffFilter.h"
#include "Utility/SDL_Utility.h"
#include "Utility/Gaussian.h"
#include "Parser/Hash.h"

Sun::Sun() { }

void Sun::setHash(Hash* hash) {
   radius = hash->getInteger("radius");
   spikeLength = hash->getInteger("spikeLength");
   numSpikes = hash->getInteger("numSpikes");
   startAngle = hash->getDouble("startAngle");
   color.set(hash->getValue("color")->getArray());
   position.set(hash->getValue("position")->getArray());
   
   create();
}

void Sun::create() {
   int size = spikeLength * 2;
   SDL_Surface* surf = createSurface(1600, size);
   Uint32 black = SDL_MapRGBA(surf->format, 0, 0, 0, 0);
   SDL_FillRect(surf, NULL, black);
   
   int cx = 800, cy = spikeLength;
   
   Color blue(0.1, 0.3, 0.8, 0);

   float offset = 360.f / numSpikes;

   for(float a = startAngle; a < 360 + startAngle; a += offset) {
      ThickLine line(cx, cy, spikeLength, a, radius/2-10, WHITE, color);
      line.draw(surf);
   }
   
   ThickLine blue1(cx, cy, 800, 0, radius/2, WHITE, blue);
   blue1.draw(surf);
   ThickLine blue2(cx, cy, 800, 180, radius/2, WHITE, blue);
   blue2.draw(surf);

   Ellipse center(cx, cy, radius, radius, WHITE, color);
   center.setFilter(new SmoothStepFilter(0.2, 1.0));
   center.draw(surf);
   
   Color h1(0.7, 0.5, 0.1, 0.2);
   Color h2(0.7, 0.5, 0.1, 0.0);
   Ellipse outer(cx, cy, 300, 300, h1, h2);
   outer.setFilter(new SmoothStepFilter(0.75, 1.0));
//   outer.draw(surf);
   
   int fSize;
   double* filter = CreateGaussianFilter(3, 6, fSize);
   surface = Convolute(surf, filter, fSize);

   delete[] filter;
   SDL_FreeSurface(surf);
}
