#include "Tracer.h"
#include "Math/Maths.h"
#include "Geometry/GeometryManager.h"
#include "Storage/Storage.h"
#include "Materials/Material.h"

Tracer::Tracer() : bgColor(), texture(NULL) {
}

Tracer::~Tracer() {
   if(texture != NULL) {
      delete texture;
   }
}

Color Tracer::traceRay(const Ray& ray, bool useBackground) {
   ShadeRecord sr;
   ray.tHit = HUGE_VALUE;
   
   if(GeometryManager::instance().getStorage()->hit(ray, sr)) {
//      Color c = sr.material->shade(sr, ray);
      Color c = integrator.shade(sr, ray, this);
      
      if(c.getAlphaF() < 1.0) {
         Ray newRay(sr.hitPoint, ray.direction);
         Color newColor = traceRay(newRay);
         return c * c.alpha + newColor * (1.0 - c.alpha);
      }
      return c;
   }
   else {
      if(!useBackground) {
         return BLACK;
      }
      if(texture != NULL) {
         sr.localHitPoint.set(ray.direction.x, ray.direction.y, ray.direction.z);
         return texture->getColor(sr);
      }
      return bgColor;
   }
}
