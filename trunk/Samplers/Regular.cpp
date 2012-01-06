#include "Regular.h"
#include <math.h>

Regular::Regular(const SamplerBounds& bounds) : Sampler(bounds) {
   nSamples = 1;
}

Regular::~Regular() {
}

void Regular::setHash(Hash* h) {
}

uint32_t Regular::getSamples(Sample* samples) {
   samples->imageX = 0.5;
   samples->imageY = 0.5;
   return nSamples;
}
