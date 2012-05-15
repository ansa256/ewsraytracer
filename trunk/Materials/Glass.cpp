#include "Glass.h"
#include "BRDFs/SpecularTransmission.h"

Glass::Glass() { }

Glass::~Glass() {
   delete stBRDF;
}

void Glass::setHash(Hash* hash) {
   Phong::setHash(hash);
   stBRDF = new SpecularTransmission(WHITE, 1.6, 1.0);
   bsdf.addBRDF(stBRDF);
}
