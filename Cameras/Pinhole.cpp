#include <math.h>
#include <SDL/SDL.h>
#include "Pinhole.h"
#include "Film.h"
#include "Utility/Color.h"
#include "Math/Ray.h"
#include "Tracer/Tracer.h"
#include "Samplers/Sampler.h"
#include "Math/Point2D.h"
#include "Parser/Hash.h"
#include "Utility/SDL_Utility.h"

Pinhole::Pinhole(int w, int h) : Camera(w, h) {
}

void Pinhole::renderScene(const SamplerBounds& bounds) {
   Ray ray;
   ray.origin = eye;
  
   Sampler* sampler = Sampler::createSampler(bounds, samplerHash);
   Sample* samples = new Sample[sampler->getNumSamples()];
   
   double x, y;
   int nSamples;
   while((nSamples = sampler->getSamples(samples)) > 0) {
      for(int i = 0; i < nSamples; i++) {
         x = samples[i].imageX - 0.5 * width;
         y = samples[i].imageY - 0.5 * height;

         ray.direction = u * x + v * y - w * viewPlaneDistance;
         ray.direction.normalize();
         film->addSample(samples[i].imageX, samples[i].imageY, tracer->traceRay(ray, 0));         
      }
   }
   
   delete[] samples;
}
