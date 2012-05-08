#include "Metal.h"
#include "Parser/Hash.h"
#include "BRDFs/Microfacet.h"
#include "BRDFs/Fresnel.h"
#include "BRDFs/Lambertian.h"
#include "Textures/ImageTexture.h"
#include "Lights/LightManager.h"
#include "Lights/Light.h"

Metal::Metal() : microfacet(NULL), ambient(new Lambertian()), diffuse(new Lambertian()), kd(1.0) {   
}

Metal::~Metal() {
}

void Metal::setHash(Hash* hash) {
   kd = hash->getDouble("kd", 1.0);
   
   float eta = hash->getDouble("eta", .5); // 1.38f);
   float k = hash->getDouble("k", 5); // 1.687f);

   MicrofacetDistribution *md = new Blinn();  
   Fresnel *frMf = new FresnelConductor(eta, k);
   microfacet = new Microfacet(frMf, md);
   
   if(hash->contains("texture")) {
      ambient->setTexture(Texture::createTexture(hash->getValue("texture")->getHash()));
      diffuse->setTexture(Texture::createTexture(hash->getValue("texture")->getHash()));
      microfacet->setTexture(Texture::createTexture(hash->getValue("texture")->getHash()));
   }
   else if(hash->contains("textureFile")) {
      setTexture(hash->getString("textureFile"));
   }
   
   if(hash->contains("color")) {
      Array* a = hash->getValue("color")->getArray();
      ambient->setColor(Color(a));
      diffuse->setColor(Color(a));
      microfacet->setColor(Color(a));
   }
   
   if(hash->contains("ambientColor")) {
      ambient->setColor(Color(hash->getValue("ambientColor")->getArray()));
   }
}

Color Metal::shade(ShadeRecord& sr, const Ray& ray) {
   if(normalMap != NULL) applyNormalMap(sr);
   Vector3D wo = ray.direction * -1;
   Color L = ambient->rho(sr, wo) * LightManager::instance().getAmbientLight(sr) * (1.f - kd);
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
               power += c * ndotwi / pdf;
            }
         }
      }
      
      power = power / (*it)->getNumLightSamples();
      wis.normalize();
      L += (diffuse->f(sr, wo, wis) + microfacet->f(sr, wo, wis)) * power * kd;
//      L += microfacet->f(sr, wo, wis) * power * kd;
   }
   
   L.alpha = diffuse->getAlpha(sr) * kd + ambient->getAlpha(sr) * (1.f - kd);
   return L;
}

float Metal::getAlpha(const ShadeRecord& sr, const Ray& ray) const {
   return microfacet->getAlpha(sr);
}

void Metal::setTexture(string texture) {
   ImageTexture* tex = new ImageTexture();
   tex->setTextureFile(texture);
   ambient->setTexture(tex);
   
   tex = new ImageTexture();
   tex->setTextureFile(texture);
   microfacet->setTexture(tex);
}
