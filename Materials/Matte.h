#ifndef _MATTE_H_
#define _MATTE_H_

#include "Material.h"

class Lambertian;
class Texture;
class Light;

/**
 * Hash values:
 * <ul>
 * <li>ka <i>val</i> - Ambient contibution
 * <li>kd <i>val</i> - Diffuse contribution
 * <li>color [ r g b ] - Optional material color
 * <li>texture {} - Optional texture hash. See Texture.
 * </ul>
 * If neither color or texture is specified, material defaults to black.
 */
class Matte : public Material {

public:
   Matte();
   virtual ~Matte();

   virtual void setHash(Hash* hash);
};

#endif
