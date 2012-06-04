#include "Marble.h"
#include "BRDFs/SpecularReflection.h"
#include "BRDFs/Lambertian.h"
#include "BRDFs/SpecularHighlight.h"
#include "Parser/Hash.h"
#include "Textures/Texture.h"

Marble::Marble() {
}

void Marble::setHash(Hash* hash) {
   hash->addValue("type", new Value("marble"));
   hash->addValue("numOctaves", new Value(8));
   hash->addValue("gain", new Value(0.5));
   hash->addValue("lacunarity", new Value(2.0));

   ambientBRDF->setTexture(Texture::createTexture(hash));

   Lambertian* diffuseBRDF = new Lambertian();
   diffuseBRDF->setTexture(Texture::createTexture(hash));

   SpecularHighlight* specularBRDF = new SpecularHighlight();
   specularBRDF->setColor(Color(0.2, 0.2, 0.2));
   specularBRDF->setExp(10.0);

   SpecularReflection* psBRDF = new SpecularReflection();
   psBRDF->setColor(Color(0.1, 0.1, 0.1, 1.0));

   bsdf.addBRDF(diffuseBRDF);
   bsdf.addBRDF(specularBRDF);
   bsdf.addBRDF(psBRDF);
}
