/*
 *  GeometryObject.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _GEOMETRY_OBJECT_
#define _GEOMETRY_OBJECT_

#include "Math/Ray.h"
#include "Utility/Color.h"
#include "Utility/ShadeRecord.h"
#include "Math/Point3D.h"
#include "BBox.h"

class Hash;
class Material;
class Texture;

class GeometryObject {

public:
   GeometryObject();
   virtual ~GeometryObject();

   virtual void setHash(Hash* hash) = 0;
   virtual bool hit(const Ray& ray, ShadeRecord& sr) const = 0;
   virtual bool shadowHit(const Ray& ray, double& tHit) const = 0;

   void setupMaterial(Hash* hash);
   Material* getMaterial() const { return material; }
   void setMaterial(Material* m);

   virtual bool isCompound() { return false; }

   static const double epsilon;
   bool doDelete;
   bool ignoreShadow;
   BBox bbox;

protected:
   mutable Material* material;
   Texture* normalMap;
};

#endif
