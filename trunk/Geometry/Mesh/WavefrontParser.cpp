#include "WavefrontParser.h"
#include "Parser/Hash.h"
#include "Storage/KdTree.h"
#include "Storage/Grid.h"
#include "Storage/ObjectList.h"
#include "Mesh.h"
#include <sstream>

WavefrontParser::WavefrontParser() : scale(1.0), textureDir(""), storage(NULL)
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
}

bool WavefrontParser::load(const string& filename) {
   in.open(filename.c_str(), ios::in | ios::binary);
   if (!in.good()) {
      fprintf(stderr, "Read3DSFile: Error opening %s\n", filename.c_str());
      return false;
   }

   string line;
   bool done = false;
   Mesh* mesh = NULL;
   string s;
   int vertexOffset = 0;
   int normalOffset = 0;
   int vertexCount = 0;
   int normalCount = 0;

   while(!done) {
      getNextLine(line);
      stringstream strstr(line);

      if(line[0] == 'o') {
         if(mesh != NULL) {
            vertexOffset += vertexCount;
            vertexCount = 0;
            normalOffset += normalCount;
            normalCount = 0;
            storage->addObject(mesh);
         }
         mesh = new Mesh();
      }
      else if(line[0] == 'v' && line[1] == ' ') {
         float x, y, z;
         strstr >> s >> x >> y >> z;
         mesh->addPoint(new Point3D(x, y, z));
         vertexCount++;
      }
      else if(line[0] == 'v' && line[1] == 't') {
         float u, v;
         strstr >> s >> u >> v;
//         mesh->addTextureCoord(u, v);
      }
      else if(line[0] == 'v' && line[1] == 'n') {
         float x, y, z;
         strstr >> s >> x >> y >> z;
         mesh->addNormal(new Vector3D(x, y, z));
         normalCount++;
      }
      else if(line[0] == 'f') {
         handleFace(line, mesh, vertexOffset, normalOffset);
      }

      done = line.empty();
   }

   in.close();
   storage->setup();

   return true;
}

void WavefrontParser::handleFace(string line, Mesh* mesh, int vertexOffset, int normalOffset) {
   vector<string> verticies = faceSplit(line);
   vector<int> vertex1 = vertexSplit(verticies[1]);
   vector<int> vertex2 = vertexSplit(verticies[2]);

   for(unsigned i = 3; i < verticies.size(); i++) {
      vector<int> vertex = vertexSplit(verticies[i]);
      WavefrontFace* face = (WavefrontFace*) mesh->addFace(vertex1[0] - vertexOffset - 1, vertex2[0] - vertexOffset - 1, vertex[0] - vertexOffset - 1, WAVEFRONT);
      face->setNormalIdxs(vertex1[2] - normalOffset - 1, vertex2[2] - normalOffset - 1, vertex[2] - normalOffset - 1);
//      face->setTextureIdxs(vertex1[1] - 1, vertex2[1] - 1, vertex[1] - 1);
   }
}

void WavefrontParser::getNextLine(string& line) {
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
