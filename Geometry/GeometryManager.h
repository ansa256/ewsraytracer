#ifndef _GEOMETRY_MANAGER_H_
#define _GEOMETRY_MANAGER_H_

#include <map>
#include <memory>
#include <string>

using namespace std;

class GeometryObject;
class Hash;
class Storage;

class GeometryManager {

public:
   static GeometryManager& instance();
   ~GeometryManager();

   void loadObjects(string fname);

   GeometryObject* createObject(string type, Hash* hash, bool addToList = true);

   Storage* getStorage() { return storage; }
   void setStorageConfig(Hash* h);

private:
   GeometryManager();

   static auto_ptr<GeometryManager> s_instance;
   Storage* storage;
};

#endif
