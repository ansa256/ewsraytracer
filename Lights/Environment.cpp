#include "Environment.h"
#include "Samplers/Sampler.h"
#include "Parser/Hash.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Materials/Emissive.h"
#include "Storage/Storage.h"

const float PDF = 1.0 / 2.0 * M_PI;

Environment::Environment() : Light(), material(new Emissive()), samples(NULL), idx(0) {
}

Environment::~Environment() {
   delete material;
   delete[] samples;
}

void Environment::setHash(Hash* hash) {
   material->setHash(hash);

   numSamples = hash->getInteger("numSamples");
   samples = new float[numSamples * 2];
   Sampler::LatinHyperCube(samples, numSamples, 2);
   
   numLightSamples = hash->getInteger("numLightSamples");
}

Vector3D Environment::getLightDirection(ShadeRecord& sr) {
   Vector3D u;

   if(fabs(sr.normal.x) > fabs(sr.normal.y)) {
      u.set(-sr.normal.z, 0.0, sr.normal.x);
   }
   else {
      u.set(0.0, sr.normal.z, -sr.normal.y);
   }
   Vector3D v = sr.normal.cross(u);

   Vector3D sp = Sampler::mapToHemisphere(samples[idx], samples[idx+1]);
   idx = (idx + 2) % (numSamples * 2);
   return u * sp.x + v * sp.y + sr.normal * sp.z;
}

bool Environment::inShadow(const Ray& ray, const ShadeRecord& sr) {
   if(GeometryManager::instance().getStorage()->shadowHit(ray)) {
      return true;
   }
   return false;
}

Color Environment::L(const ShadeRecord& sr) {
   return material->getLe(sr);
}

float Environment::pdf(const ShadeRecord& sr) {
   return PDF;
}
