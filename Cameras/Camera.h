#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <SDL/SDL.h>
#include "Math/Point3D.h"
#include "Math/Vector3D.h"
#include "Sampler.h"

class Tracer;
class Hash;
class Film;

void* renderThread(void* arg);

class Camera {

public:
   Camera(int w, int h);
   virtual ~Camera();

   virtual void setHash(Hash* hash);

   void render();
   virtual void renderScene(const SamplerBounds& bounds) = 0;

   void setPosition(const Point3D& loc) { eye = loc; }
   void setSurface(SDL_Surface* s) { surface = s; }
   void setThreadParameters(int tc, int w, int h);
   void rotate(double x, double y, double z);
   
   Hash* samplerHash;

protected:
   void computeUVW(Hash* h);

   Point3D eye;
   Vector3D u, v, w;
   Tracer* tracer;
   SDL_Surface* surface;
   Film* film;

   int width;
   int height;
   float viewPlaneDistance;
   int threadCount;

private:
   int boxw;
   int boxh;
};

#endif
