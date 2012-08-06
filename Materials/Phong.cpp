#include "Phong.h"
#include "Math/Maths.h"
#include "Lights/LightManager.h"
#include "Lights/Light.h"
#include "BRDFs/Lambertian.h"
#include "BRDFs/SpecularHighlight.h"
#include "Textures/Texture.h"
#include "Parser/Hash.h"
#include "Textures/ImageTexture.h"
#include "Samplers/Sampler.h"

Phong::Phong() : texture(NULL) {
}

Phong::~Phong() {
   if(texture != NULL) {
      delete texture;
   }
}

void Phong::setHash(Hash* hash) {
   Lambertian* diffuseBRDF = new Lambertian();
   SpecularHighlight* specularBRDF = new SpecularHighlight();

   if(hash->contains("texture")) {
      texture = Texture::createTexture(hash->getValue("texture")->getHash());
      ambientBRDF->setTexture(texture, false);
      diffuseBRDF->setTexture(texture, false);
   }
   else if(hash->contains("textureFile")) {
      texture = new ImageTexture();
      ((ImageTexture*) texture)->setTextureFile(hash->getString("textureFile"));
      ambientBRDF->setTexture(texture, false);
      diffuseBRDF->setTexture(texture, false);
   }

   if(hash->contains("color")) {
      Array* a = hash->getValue("color")->getArray();
      ambientBRDF->setColor(Color(a));
      diffuseBRDF->setColor(Color(a));
   }

   if(hash->contains("ambientColor")) {
      ambientBRDF->setColor(Color(hash->getValue("ambientColor")->getArray()));
   }

   if(hash->contains("specColor")) {
      specularBRDF->setColor(Color(hash->getValue("specColor")->getArray()));
   }
   else {
      float ks = hash->getDouble("ks", 0.1);
      specularBRDF->setColor(Color(ks, ks, ks));
   }
   specularBRDF->setExp(hash->getDouble("exp"));

   if(hash->contains("specularMask")) {
      specularBRDF->setTexture(Texture::createTexture(hash->getValue("specularMask")->getHash()), true);
   }
   
   bsdf.addBRDF(diffuseBRDF);
   bsdf.addBRDF(specularBRDF);
}
