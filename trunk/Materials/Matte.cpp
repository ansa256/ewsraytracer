#include "Matte.h"
#include "Math/Maths.h"
#include "Lights/LightManager.h"
#include "BRDFs/Lambertian.h"
#include "Textures/Texture.h"
#include "Textures/ImageTexture.h"
#include "Parser/Hash.h"
#include "Lights/Light.h"
#include "Samplers/Sampler.h"

Matte::Matte() {
}

Matte::~Matte() {
}

void Matte::setHash(Hash* hash) {
   Lambertian* diffuseBRDF = new Lambertian();

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
      diffuseBRDF->setColor(Color(a));
   }

   if(hash->contains("ambientColor")) {
      ambientBRDF->setColor(Color(hash->getValue("ambientColor")->getArray()));
   }
   
   bsdf.addBRDF(diffuseBRDF);
}
