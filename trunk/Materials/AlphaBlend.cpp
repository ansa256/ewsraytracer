#include "AlphaBlend.h"
#include "BRDFs/Lambertian.h"
#include "BRDFs/Transparency.h"
#include "Parser/Hash.h"
#include "Textures/Texture.h"

AlphaBlend::~AlphaBlend() {
   delete texture;
}

void AlphaBlend::setHash(Hash* hash) {
   Lambertian* diffuseBRDF = new Lambertian();
   Transparency* transparentBRDF = new Transparency();
   
   texture = Texture::createTexture(hash->getValue("texture")->getHash());
   alphaThreshold = hash->getDouble("alphaThreshold", 0.5);

   ambientBRDF->setTexture(texture, false);
   diffuseBRDF->setTexture(texture, false);
   transparentBRDF->setTexture(texture, false);
   
   if(hash->contains("kd")) {
      double kd = hash->getDouble("kd");
      diffuseBRDF->setColor(kd, kd, kd);
   }
   
   if(hash->contains("ka")) {
      double ka = hash->getDouble("ka");
      ambientBRDF->setColor(ka, ka, ka);
   }
   
   bsdf.addBRDF(diffuseBRDF);
   bsdf.addBRDF(transparentBRDF);
}

bool AlphaBlend::shadowHit(ShadeRecord &sr) {
   float alpha = texture->getAlpha(sr);
   return alpha > alphaThreshold;
}