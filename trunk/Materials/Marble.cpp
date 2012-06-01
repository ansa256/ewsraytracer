#include "Marble.h"
#include "BRDFs/SpecularReflection.h"
#include "BRDFs/Lambertian.h"
#include "BRDFs/SpecularHighlight.h"
#include "Parser/Hash.h"
#include "Textures/Texture.h"

Marble::Marble() {
}

void Marble::setHash(Hash* hash) {
   Value* type = new Value("marble");
   hash->addValue("type", *type);

   Value* val1 = new Value(8);
   hash->addValue("numOctaves", *val1);
   
   Value* val2 = new Value(0.5);
   hash->addValue("gain", *val2);

   Value* val3 = new Value(2.0);
   hash->addValue("lacunarity", *val3);

   ambientBRDF->setTexture(Texture::createTexture(hash));

   Lambertian* diffuseBRDF = new Lambertian();
   diffuseBRDF->setTexture(Texture::createTexture(hash));

   SpecularHighlight* specularBRDF = new SpecularHighlight();
   specularBRDF->setColor(Color(0.2, 0.2, 0.2));
   specularBRDF->setExp(2000.0);

   SpecularReflection* psBRDF = new SpecularReflection();
   psBRDF->setColor(Color(0.1, 0.1, 0.1, 1.0));

   bsdf.addBRDF(diffuseBRDF);
   bsdf.addBRDF(specularBRDF);
   bsdf.addBRDF(psBRDF);
}
