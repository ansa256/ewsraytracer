#include "LatinHyperCube.h"
#include "Math/Maths.h"

LatinHyperCube::LatinHyperCube(const int ns, const int sets) : Sampler(ns, sets) {
   generateSamples();
}

LatinHyperCube::~LatinHyperCube() {
}

void LatinHyperCube::generateSamples() {
   float delta = 1.f / numSamples;
   for (int p = 0; p < numSets; p++) {
      for(uint32_t i = 0; i < numSamples; i++) {
         float x = (i + (rand_float())) * delta;
         float y = (i + (rand_float())) * delta;
         samples.push_back(new Point2D(x, y));
      }
   }

   for(int p = 0; p < numSets; p++) {
      for (int i = 0; i < numSamples; i++) {
         int j = rand_int(i, numSamples - 1);
         swap(samples[i + p * numSamples]->x, samples[j + p * numSamples]->x);
      
         j = rand_int(i, numSamples - 1);
         swap(samples[i + p * numSamples]->y, samples[j + p * numSamples]->y);
      }
   }
}
