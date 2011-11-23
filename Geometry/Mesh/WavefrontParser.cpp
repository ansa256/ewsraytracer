#include "WavefrontParser.h"
#include "Parser/Hash.h"
#include "Storage/KdTree.h"
#include "Storage/Grid.h"
#include "Storage/ObjectList.h"
#include "Mesh.h"
#include "Materials/Phong.h"
#include <sstream>

WavefrontParser::WavefrontParser() : scale(1.0), textureDir(""), storage(NULL), materials(), includes(), excludes(), useMaterials(false)
{
}

WavefrontParser::~WavefrontParser()
{
}

void WavefrontParser::setHash(Hash* h) {
   if(h->contains("scale")) {
      scale = h->getDouble("scale");
   }
   if(h->contains("textureDir")) {
      textureDir = h->getString("textureDir");
   }
   if(h->contains("storage")) {
      Hash* s = h->getValue("storage")->getHash();
      string type = s->getString("type");

      if(type == "grid") {
         storage = new Grid();
      }
      else if(type == "kdtree") {
         storage = new KdTree();
      }
      else {
         storage = new ObjectList();
      }
      storage->setHash(s);
   }
   else {
      storage = new KdTree();
   }

   if(h->contains("materials")) {
      loadMaterials(h->getString("materials"));
   }

   if(h->contains("includes")) {
      stringstream strstr(h->getString("includes"));
      string s;
      while (getline(strstr, s, ',')) {
         includes.push_back(s);
      }
   }

   if(h->contains("excludes")) {
      stringstream strstr(h->getString("excludes"));
      string s;
      while (getline(strstr, s, ',')) {
         excludes.push_back(s);
      }
   }
}

bool WavefrontParser::load(const string& filename) {
   ifstream in(filename.c_str(), ios::in | ios::binary);
   if (!in.good()) {
      fprintf(stderr, "Read3DSFile: Error opening %s\n", filename.c_str());
      return false;
   }

   string line;
   string matName = "";
   bool done = false;
   Mesh* mesh = NULL;
   string s;
   int vertexOffset = 0;
   int normalOffset = 0;
   int textureOffset = 0;
   int vertexCount = 0;
   int normalCount = 0;
   int textureCount = 0;

   while(!done) {
      getNextLine(in, line);
      stringstream strstr(line);

      if(line[0] == 'o') {
         vertexOffset += vertexCount;
         vertexCount = 0;
         normalOffset += normalCount;
         normalCount = 0;
         textureOffset += textureCount;
         textureCount = 0;

         if(mesh != NULL) {
            if(useMaterials && !matName.empty()) {
               mesh->setMaterial(materials[matName]);
            }
            matName = "";
            storage->addObject(mesh);
            mesh = NULL;
         }

         string name = line.substr(2);
         if(includes.empty() || find(includes.begin(), includes.end(), name) != includes.end()) {
            if(find(excludes.begin(), excludes.end(), name) == excludes.end()) {
               mesh = new Mesh();
            }
         }
      }
      else if(line[0] == 'v' && line[1] == ' ') {
         if(mesh != NULL) {
            float x, y, z;
            strstr >> s >> x >> y >> z;
            mesh->addPoint(new Point3D(x, y, z));
         }
         vertexCount++;
      }
      else if(line[0] == 'v' && line[1] == 't') {
         if(mesh != NULL) {
            float u, v;
            strstr >> s >> u >> v;
            mesh->addTextureCoord(u, v);
         }
         textureCount++;
      }
      else if(line[0] == 'v' && line[1] == 'n') {
         if(mesh != NULL) {
            float x, y, z;
            strstr >> s >> x >> y >> z;
            Vector3D* normal = new Vector3D(x, y, z);
            normal->normalize();
            mesh->addNormal(normal);
         }
         normalCount++;
      }
      else if(line[0] == 'f') {
         if(mesh != NULL) {
            handleFace(line, mesh, vertexOffset, normalOffset, textureOffset);
         }
      }
      else if(strncmp(line.c_str(), "usemtl", 6) == 0) {
         matName = line.substr(7);
      }

      done = line.empty();
   }

   if(mesh != NULL) {
      if(useMaterials && !matName.empty()) {
         mesh->setMaterial(materials[matName]);
      }
      storage->addObject(mesh);
   }

   in.close();
   storage->setup();

   return true;
}

void WavefrontParser::handleFace(string line, Mesh* mesh, int vertexOffset, int normalOffset, int textureOffset) {
   vector<string> verticies = faceSplit(line);
   vector<int> vertex1 = vertexSplit(verticies[1]);
   vector<int> vertex2 = vertexSplit(verticies[2]);

   for(unsigned i = 3; i < verticies.size(); i++) {
      vector<int> vertex3 = vertexSplit(verticies[i]);

      WavefrontFace* face = (WavefrontFace*) mesh->addFace(vertex1[0] - vertexOffset - 1, vertex2[0] - vertexOffset - 1, vertex3[0] - vertexOffset - 1, WAVEFRONT);
      if(vertex1.size() > 1) {
         face->setTextureIdxs(vertex1[1] - textureOffset - 1, vertex2[1] - textureOffset - 1, vertex3[1] - textureOffset - 1);
      }
      if(vertex1.size() == 3) {
         face->setNormalIdxs(vertex1[2] - normalOffset - 1, vertex2[2] - normalOffset - 1, vertex3[2] - normalOffset - 1);
      }
      vertex2 = vertex3;
   }
}

void WavefrontParser::getNextLine(ifstream& in, string& line) {
   for (;;) {
      getline(in, line);

      if(in.good()) {
         if(line.length() > 0 && line[0] != '#') {
            break;
         }
      }
      else {
         line.clear();
         break;
      }
   }
}

vector<string> WavefrontParser::faceSplit(string str) const {
   stringstream ss(str);
   string s;
   vector<string> results;

   while (getline(ss, s, ' ')) {
      results.push_back(s);
   }
   return results;
}

vector<int> WavefrontParser::vertexSplit(string str) const {
   stringstream ss(str);
   string s;
   vector<int> results;

   while (getline(ss, s, '/')) {
      results.push_back(atoi(s.c_str()));
   }
   return results;
}

void WavefrontParser::loadMaterials(string fname) {
   ifstream in(fname.c_str(), ios::in | ios::binary);
   if (!in.good()) {
      fprintf(stderr, "Read3DSFile: Error opening %s\n", fname.c_str());
      return;
   }

   string line;
   string s;
   bool done = false;
   Phong* material = NULL;

   while(!done) {
      getNextLine(in, line);
      stringstream strstr(line);

      if(strncmp(line.c_str(), "newmtl", 6) == 0) {
         material = new Phong();
         string name = line.substr(7);
         materials[name] = material;
      }
      else if(strncmp(line.c_str(), "Kd", 2) == 0) {
         float r, g, b;
         strstr >> s >> r >> g >> b;
         material->setDiffuseColor(new Color(r, g, b));
      }
      else if(strncmp(line.c_str(), "Ka", 2) == 0) {
         float r, g, b;
         strstr >> s >> r >> g >> b;
         material->setAmbientColor(new Color(r, g, b));
      }
      else if(strncmp(line.c_str(), "map_Kd", 6) == 0) {
         material->setTexture(textureDir + line.substr(7));
      }

      done = line.empty();
   }

   in.close();
   useMaterials = true;
}
