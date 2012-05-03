#ifndef raytracer_Overlay_h
#define raytracer_Overlay_h

#include <SDL/SDL.h>
#include "Math/Point3D.h"

class Hash;

class Overlay {
  
public:
   virtual ~Overlay() { SDL_FreeSurface(surface); }

   virtual void setHash(Hash* hash) = 0;

   Point3D position;
   SDL_Surface* surface;
};

#endif
