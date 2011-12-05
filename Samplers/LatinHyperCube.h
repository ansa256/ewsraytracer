#ifndef _LATIN_HYPERCUBE_H_
#define _LATIN_HYPERCUBE_H_

#include "Sampler.h"

class LatinHyperCube : public Sampler {
   
public:
   LatinHyperCube(const int ns = 1, const int sets = 83);
   ~LatinHyperCube();

   virtual void generateSamples();
};

#endif
