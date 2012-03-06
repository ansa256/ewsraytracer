#include "Matte.h"
#include "Math/Maths.h"
#include "Lights/LightManager.h"
#include "BRDFs/Lambertian.h"
#include "Textures/Texture.h"
#include "Textures/ImageTexture.h"
#include "Parser/Hash.h"
#include "Lights/Light.h"
#include "Samplers/Sampler.h"

Matte::Matte() : ambientBRDF(new Lambertian()), diffuseBRDF(new Lambertian()), kd(0.9f) {
}

Matte::~Matte() {
   delete ambientBRDF;
   delete diffuseBRDF;
}

void Matte::setHash(Hash* hash) {
   kd = hash->getDouble("kd", 0.9);

   if(hash->contains("texture")) {
      ambientBRDF->setTexture(Texture::createTexture(hash->getValue("texture")->getHash()));
      diffuseBRDF->setTexture(Texture::createTexture(hash->getValue("texture")->getHash()));
   }
   else if(hash->contains("textureFile")) {
      setTexture(hash->getString("textureFile"));
   }

   if(hash->contains("color")) {
      Array* a = hash->getValue("color")->getArray();
      ambientBRDF->setColor(Color(a));
      diffuseBRDF->setColor(Color(a));
   }

   if(hash->contains("ambientColor")) {
      ambientBRDF->setColor(Color(hash->getValue("ambientColor")->getArray()));
   }
}

Color Matte::shade(ShadeRecord& sr, const Ray& ray) {
   if(normalMap != NULL) applyNormalMap(sr);
   Vector3D wo = ray.direction * -1;
   Color L = ambientBRDF->rho(sr, wo) * LightManager::instance().getAmbientLight(sr) * (1.f - kd);
   Vector3D wi;
   float pdf;

   for(LightIter it = LightManager::instance().begin(); it != LightManager::instance().end(); it++) {
      Color power;
      Vector3D wis;
      
      float* samples = (*it)->getSamples();
      for(int s = 0; s < (*it)->getNumLightSamples(); s++) {
         Color c = (*it)->Sample_L(sr, samples[s * 2], samples[s * 2 + 1], wi, pdf);
         wis += wi;
         float ndotwi = sr.normal.dot(wi);
         
         if(ndotwi > 0.0) {
            Ray shadowRay(sr.hitPoint, wi);
            bool inShadow = (*it)->inShadow(shadowRay, sr);
            if(!inShadow) {
               power += c * (*it)->G(sr) * ndotwi / pdf;
            }
         }
      }

      power = power / (*it)->getNumLightSamples();
      wis.normalize();
      L += diffuseBRDF->f(sr, wo, wis) * power * kd;
   }

   L.alpha = diffuseBRDF->getAlpha(sr) * kd + ambientBRDF->getAlpha(sr) * (1.f - kd);
   return L;
}

float Matte::getAlpha(const ShadeRecord& sr, const Ray& ray) const {
   return diffuseBRDF->getAlpha(sr);
}

void Matte::setTexture(string texture) {
   ImageTexture* tex = new ImageTexture();
   tex->setTextureFile(texture);
   ambientBRDF->setTexture(tex);

   tex = new ImageTexture();
   tex->setTextureFile(texture);
   diffuseBRDF->setTexture(tex);
}
