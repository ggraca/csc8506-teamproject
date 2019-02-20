#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include "../Common/Vector2.h"
#include "../Common/Vector3.h"
#include "../Common/Vector4.h"
#include "OGLMesh.h"
#include "../Common/Assets.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Common/TextureLoader.h"


#define OBJCOMMENT	"#"			  //The current line of the obj file is a comment
#define OBJMTLLIB		"mtllib"
#define OBJOBJECT		"o"	      //the current line of the obj file defines the start of a new material
#define OBJVERT			"v"			  //the current line of the obj file defines a vertex
#define OBJMESH			"g"			  //the current line of the obj file defines the start of a new face
#define OBJFACE			"f"			  //the current line of the obj file defines a face

#define OBJTEX			"vt"	  	//the current line of the obj file defines texture coordinates
#define OBJNORM			"vn"		  //the current line of the obj file defines a normal

#define OBJUSEMTL		"usemtl"	//the current line of the obj file defines the start of a new material


#define MTLNEW			"newmtl"
#define MTLAMBIENT		"Ka"
#define MTLDIFFUSE		"Kd"
#define MTLSPEC			"Ks"
#define MTLSPECWT		"Ns"
#define MTLTRANS		"d"
#define MTLTRANSALT		"Tr"
#define MTLILLUM		"illum"
#define MTLDIFFUSEMAP	"map_Kd"
#define MTLBUMPMAP		"map_bump"
#define MTLBUMPMAPALT	"bump"


using namespace std;
using namespace NCL;
using namespace NCL::Rendering;

class ChildMeshInterface {
public:
  void AddChild(OGLMesh*m) {
    children.push_back(m);
  }

  virtual ~ChildMeshInterface() {
    for(unsigned int i = 0; i < children.size(); ++i) {
      delete children.at(i);
    }
  }

  vector<OGLMesh*> GetChildren() const { return children; }
  
protected:
  vector<OGLMesh*>children;
  ChildMeshInterface(void){};
};


struct OBJSubMesh {
  std::vector<int> texIndices;
  std::vector<int> vertIndices;
  std::vector<int> normIndices;

  int indexOffset;
  string mtlType;
  string mtlSrc;
};


struct MTLInfo {
  string bump;
  string diffuse;

  OGLTexture* bumpTex;
  OGLTexture* diffuseTex;

  Vector4 colour;

  MTLInfo() {
    bumpTex = nullptr;
    diffuseTex = nullptr;
  }
};


class OBJMesh : public OGLMesh, public ChildMeshInterface {
public:
  OBJMesh(void) {};
  OBJMesh(std::string filename) { LoadOBJMesh(filename); };
  ~OBJMesh(void) {};
  bool	LoadOBJMesh(std::string filename);
  Vector4 colour;

protected:
  void SetTexturesFromMTL(string &mtlFile, string &mtlType);
  void LoadFaceFromFile(std::ifstream &f, OBJSubMesh* &currentMesh, std::vector<OBJSubMesh*> &inputSubMeshes);

  map <string, MTLInfo> materials;

};