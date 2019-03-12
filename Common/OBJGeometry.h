#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "MeshGeometry.h"
#include "Assets.h"
#include "TextureLoader.h"
#include "Material.h"


#define OBJCOMMENT		"#"			//The current line of the obj file is a comment
#define OBJOBJECT		"o"			//the current line of the obj file defines the start of a new material
#define OBJVERT			"v"			//the current line of the obj file defines a vertex
#define OBJMESH			"g"			//the current line of the obj file defines the start of a new face
#define OBJFACE			"f"			//the current line of the obj file defines a face

#define OBJTEX			"vt"		//the current line of the obj file defines texture coordinates
#define OBJNORM			"vn"		//the current line of the obj file defines a normal

#define OBJMTLLIB		"mtllib"	//the current line of the obj file defines the mtl file where materials are defined
#define OBJUSEMTL		"usemtl"	//the current line of the obj file defines the material that the next mesh will use

#define MTLNEW			"newmtl"
#define MTLAMBIENT		"Ka"
#define MTLDIFFUSE		"Kd"
#define MTLSPEC			"Ks"
#define MTLSPECWT		"Ns"
#define MTLTRANS		"d"
#define MTLTRANSALT		"Tr"
#define MTLTRANS2		"Tf"
#define MTLILLUM		"illum"
#define MTLDIFFUSEMAP	"map_Kd"
#define MTLSPECMAP		"map_Ks"
#define MTLBUMPMAP		"map_bump"
#define MTLBUMPMAPALT	"bump"


using namespace std;
using namespace NCL;
using namespace NCL::Rendering;


class ChildMeshInterface {
public:
	void AddChild(MeshGeometry* m) {
		children.push_back(m);
	}

	virtual ~ChildMeshInterface() {
		for(unsigned int i = 0; i < children.size(); ++i) {
			delete children.at(i);
		}
	}

	vector<MeshGeometry*> GetChildren() const { return children; }
  
protected:
	vector<MeshGeometry*>children;
	ChildMeshInterface(void){};
};

struct OBJSubMesh {
	std::vector<int> texIndices;
	std::vector<int> vertIndices;
	std::vector<int> normIndices;

	string mtlType;
	string mtlSrc;
};

// TODO: Make this compatible with PS4
class OBJMesh : public OGLMesh {
public:
	OBJMesh(NCL::Rendering::Material* mat) : material(mat) {}
	NCL::Rendering::Material* material = nullptr; // TODO: fix this naming
	static OBJMesh* FromSubMesh(OBJSubMesh* sm, vector<Vector3>& inputVertices, vector<Vector2>& inputTexCoords, vector<Vector3>& inputNormals);
};

class OBJGeometry : public ChildMeshInterface {
public:
	OBJGeometry(void) {}
	OBJGeometry(std::string filename) { LoadOBJMesh(filename); }
	~OBJGeometry(void) {}
	bool	LoadOBJMesh(std::string filename);
	Vector4 colour;

protected:
	void LoadFaceFromFile(std::ifstream &f, OBJSubMesh* &currentMesh, std::vector<OBJSubMesh*> &inputSubMeshes);
	void LoadMaterialsFromMTL(string filename);
	string NormalisePath(string path);
};