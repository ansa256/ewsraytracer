#include "ThinLens.h"
#include "Math/Ray.h"
#include "Math/Point2D.h"
#include "Tracer/Tracer.h"
#include "Parser/Hash.h"
#include "Utility/SDL_Utility.h"
#include "Samplers/Sampler.h"
#include "Film.h"

ThinLens::ThinLens(int w, int h) : Camera(w, h) {
   lensRadius = 1.0;
}

ThinLens::~ThinLens() {
}

void ThinLens::setHash(Hash* hash) {
   Camera::setHash(hash);
   f = hash->getDouble("focusPlaneDistance");
   lensRadius = hash->getDouble("lensRadius");
}

void ThinLens::renderScene(const SamplerBounds& bounds) {
   Color pixelColor;
   Ray ray;
   Point2D lp;

   Sampler* sampler = Sampler::createSampler(bounds, samplerHash);
   Sample* samples = new Sample[sampler->getNumSamples()];
   
   double x, y;
   Point2D dp;   
   int nSamples;
   while((nSamples = sampler->getSamples(samples)) > 0) {
      for(int i = 0; i < nSamples; i++) {
         x = samples[i].imageX - 0.5 * width;
         y = samples[i].imageY - 0.5 * height;
         
         Sampler::mapToDisk(samples[i].lensX, samples[i].lensY, &dp.x, &dp.y);
         lp = dp * lensRadius;
         
         ray.origin = eye + u * lp.x + v * lp.y;
         
         double dirX = x * f / viewPlaneDistance;
         double dirY = y * f / viewPlaneDistance;
         ray.direction = u * (dirX - lp.x) + v * (dirY - lp.y) - w * f;
         ray.direction.normalize();

         film->addSample(samples[i].imageX, samples[i].imageY, tracer->traceRay(ray, 0));         
      }
   }
}
