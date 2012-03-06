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

Phong::Phong() : ambientBRDF(new Lambertian()), diffuseBRDF(new Lambertian()), specularBRDF(new GlossySpecular()), kd(0.9f) {
}

Phong::~Phong() {
   delete ambientBRDF;
   delete diffuseBRDF;
   delete specularBRDF;
}

void Phong::setHash(Hash* hash) {
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
}

Color Phong::shade(ShadeRecord& sr, const Ray& ray) {
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
      L += (diffuseBRDF->f(sr, wo, wis) + specularBRDF->f(sr, wo, wis)) * power * kd;
   }

   L.alpha = diffuseBRDF->getAlpha(sr) * kd + ambientBRDF->getAlpha(sr) * (1.f - kd);
   return L;
}

float Phong::getAlpha(const ShadeRecord& sr, const Ray& ray) const {
   return diffuseBRDF->getAlpha(sr);
}

void Phong::setTexture(string texture) {
   ImageTexture* tex = new ImageTexture();
   tex->setTextureFile(texture);
   ambientBRDF->setTexture(tex);

   tex = new ImageTexture();
   tex->setTextureFile(texture);
   diffuseBRDF->setTexture(tex);
}

