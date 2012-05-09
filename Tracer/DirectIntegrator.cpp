#include "DirectIntegrator.h"
#include "Materials/Material.h"
#include "Lights/LightManager.h"
#include "Lights/Light.h"
#include "BRDFs/Lambertian.h"

DirectIntegrator::DirectIntegrator() {
}

Color DirectIntegrator::shade(ShadeRecord& sr, const Ray& ray) {
   sr.material->applyNormalMap(sr);
   Vector3D wo = ray.direction * -1;
   Color L = sr.material->ambientBRDF->rho(sr, wo) * LightManager::instance().getAmbientLight(sr);

   for(LightIter it = LightManager::instance().begin(); it != LightManager::instance().end(); it++) {
      Color Ld;
      float* samples = (*it)->getSamples();
      for(int s = 0; s < (*it)->getNumLightSamples(); s++) {
         Ld += estimateDirect(sr, *it, wo, samples, s);
      }
      L += Ld / (*it)->getNumLightSamples();
   }
//   L.alpha = diffuseBRDF->getAlpha(sr) + ambientBRDF->getAlpha(sr);
   return L;
}

Color DirectIntegrator::estimateDirect(ShadeRecord& sr, Light* light, const Vector3D& wo, float* samples, int s) {
   Color Ld;
   Vector3D wi;
   float pdf;
   
   Color Li = light->Sample_L(sr, samples[s * 2], samples[s * 2 + 1], wi, pdf);
   if(pdf > 0 && !Li.isBlack()) {
      Color f = sr.material->bsdf.f(sr, wo, wi);
      if(!f.isBlack()) {
         float ndotwi = sr.normal.dot(wi);
         if(ndotwi > 0) {
            Ray shadowRay(sr.hitPoint, wi);
            bool inShadow = light->inShadow(shadowRay, sr);
            if(!inShadow) {
               Ld += f * Li * ndotwi/ pdf;
            }
         }
      }
   }
   return sr.material->getLe(sr) + Ld;
}
