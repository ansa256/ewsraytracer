#include "Phong.h"
#include "Math/Maths.h"
#include "Lights/LightManager.h"
#include "Lights/Light.h"
#include "BRDFs/Lambertian.h"
#include "BRDFs/GlossySpecular.h"
#include "Textures/Texture.h"
#include "Parser/Hash.h"
#include "Textures/ImageTexture.h"
#include "Samplers/Sampler.h"

Phong::Phong() {
}

Phong::~Phong() {
}

void Phong::setHash(Hash* hash) {
   Lambertian* diffuseBRDF = new Lambertian();
   GlossySpecular* specularBRDF = new GlossySpecular();

   if(hash->contains("texture")) {
      ambientBRDF->setTexture(Texture::createTexture(hash->getValue("texture")->getHash()));
      diffuseBRDF->setTexture(Texture::createTexture(hash->getValue("texture")->getHash()));
   }
   else if(hash->contains("textureFile")) {
      string texture = hash->getString("textureFile");
      ImageTexture* tex = new ImageTexture();
      tex->setTextureFile(texture);
      ambientBRDF->setTexture(tex);

      tex = new ImageTexture();
      tex->setTextureFile(texture);
      diffuseBRDF->setTexture(tex);
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
      specularBRDF->setSpecularMask(Texture::createTexture(hash->getValue("specularMask")->getHash()));
   }
   
   bsdf.addBRDF(diffuseBRDF);
   bsdf.addBRDF(specularBRDF);
}
