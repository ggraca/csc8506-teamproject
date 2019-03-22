#include "OBJGeometry.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLShader.h"

bool OBJGeometry::LoadOBJMesh(std::string filename) {
	std::ifstream f(filename.c_str(), std::ios::in);
	if (!f) return false;

	std::vector<Vector2> inputTexCoords;
	std::vector<Vector3> inputVertices;
	std::vector<Vector3> inputNormals;
	std::vector<OBJSubMesh*> inputSubMeshes;

	OBJSubMesh* currentMesh = new OBJSubMesh();
	inputSubMeshes.push_back(currentMesh);	//It's safe to assume our OBJ will have a mesh in it ;)

	string currentMtlLib;
	string currentMtlType;

	vector<string> mtlFiles;

	while (!f.eof()) {
		std::string lineHeader;
		f >> lineHeader;

		if (lineHeader == OBJCOMMENT) {
			continue;
		}
		else if (lineHeader == OBJVERT) {
			Vector3 vertex;
			f >> vertex.x; f >> vertex.y; f >> vertex.z;
			inputVertices.push_back(vertex);
		}
		else if (lineHeader == OBJTEX) {
			Vector2 texCoord;
			f >> texCoord.x; f >> texCoord.y;
			inputTexCoords.push_back(texCoord);
		}
		else if (lineHeader == OBJNORM) {
			Vector3 normal;
			f >> normal.x; f >> normal.y; f >> normal.z;
			inputNormals.push_back(normal);
		}
		else if (lineHeader == OBJFACE) {
			LoadFaceFromFile(f, currentMesh, inputSubMeshes);
		}
		else if (lineHeader == OBJMTLLIB) {
			f >> currentMtlLib;
			mtlFiles.push_back(currentMtlLib);
		}
		else if (lineHeader == OBJUSEMTL) {
			currentMesh = new OBJSubMesh();
			inputSubMeshes.push_back(currentMesh);

			currentMesh->mtlSrc = currentMtlLib;

			f >> currentMtlType;

			currentMesh->mtlType = currentMtlType;
		}
		else if (lineHeader == OBJMESH || lineHeader == OBJOBJECT) {
			currentMesh = new OBJSubMesh();
			inputSubMeshes.push_back(currentMesh);

			currentMesh->mtlSrc = currentMtlLib;
			currentMesh->mtlType = currentMtlType;
		}    
		else {
			//std::cout << "OBJMesh::LoadOBJMesh Unknown file data:" << lineHeader << std::endl;
		}
	}

	f.close();

	// We now know where the materials are defined and we can import them with our Asset Manager
	for (auto filename : mtlFiles) {
		LoadMaterialsFromMTL(filename);
	}

	// We now have all our mesh data loaded in...Now to convert it into OpenGL vertex buffers!
	for (unsigned int i = 0; i < inputSubMeshes.size(); ++i) {	
		OBJMesh* m = OBJMesh::FromSubMesh(inputSubMeshes[i], inputVertices, inputTexCoords, inputNormals);
		if (m) AddChild(m);
		delete inputSubMeshes[i];
	}
	return true;
}

OBJMesh* OBJMesh::FromSubMesh(OBJSubMesh* sm, vector<Vector3>& inputVertices, vector<Vector2>& inputTexCoords, vector<Vector3>& inputNormals) {
	if (sm->vertIndices.empty()) return nullptr;

	ShaderBase* pbrShader = Assets::AssetManager::LoadShader("PBRShader", "pbrvert.glsl", "pbrfrag.glsl");
	Material* material = Assets::AssetManager::LoadMaterial(sm->mtlType, pbrShader);
	OBJMesh* m = new OBJMesh(material);

	for (unsigned int j = 0; j < sm->vertIndices.size(); ++j) {
		m->positions.push_back(inputVertices[sm->vertIndices[j] - 1]);
	}

	if (!sm->texIndices.empty()) {
		for (unsigned int j = 0; j < sm->texIndices.size(); ++j) {
			m->texCoords.push_back(inputTexCoords[sm->texIndices[j] - 1]);
		}
	}

	if (sm->normIndices.empty()) {
		m->RecalculateNormals();
	}
	else {
		for (unsigned int j = 0; j < sm->normIndices.size(); ++j) {
			m->normals.push_back(inputNormals[sm->normIndices[j] - 1]);
		}
	}

	for (unsigned int j = 0; j < sm->vertIndices.size(); ++j) {
		m->colours.push_back(Vector4(0, 0, 1, 1));
	}

	// TODO: Tangents
	// m->GenerateTangents();

	m->UploadToGPU();

	return m;
}

void OBJGeometry::LoadFaceFromFile(std::ifstream &f, OBJSubMesh* &currentMesh, std::vector<OBJSubMesh*> &inputSubMeshes) {
	if (!currentMesh) {
		currentMesh = new OBJSubMesh();
		inputSubMeshes.push_back(currentMesh);
	}

	std::string data;
	getline(f, data);

	vector<VertData> verts;
	vector<string> faceData = split_string(data, ' ');

	for (auto vertData : faceData) {
		VertData vert;
		bool hasTex = true;
		if (vertData.find("//") != std::string::npos) hasTex = false;

		vector<string> attribues = split_string(vertData, '/');
		vert.vertIndex = stoi(attribues[0]);

		if (attribues.size() == 3) {
			vert.vertTex = stoi(attribues[1]);
			vert.vertNormal = stoi(attribues[2]);
		}
		else if (attribues.size() == 2) {
			if (hasTex) vert.vertTex = stoi(attribues[1]);
			else vert.vertNormal = stoi(attribues[1]);
		}

		verts.push_back(vert);
	}

	
	// First Face
	for (int i = 0; i < 3; i++) {
		currentMesh->vertIndices.push_back(verts[i].vertIndex);
		if (verts[i].vertTex != -1) currentMesh->texIndices.push_back(verts[i].vertTex);
		if (verts[i].vertNormal != -1) currentMesh->normIndices.push_back(verts[i].vertNormal);
	}

	//// Following Faces
	for (unsigned int i = 2; i < verts.size(); i++) {
		currentMesh->vertIndices.push_back(verts[i - 1].vertIndex);
		if (verts[i].vertTex != -1) currentMesh->texIndices.push_back(verts[i - 1].vertTex);
		if (verts[i].vertNormal != -1) currentMesh->normIndices.push_back(verts[i - 1].vertNormal);

		currentMesh->vertIndices.push_back(verts[i].vertIndex);
		if (verts[i].vertTex != -1) currentMesh->texIndices.push_back(verts[i].vertTex);
		if (verts[i].vertNormal != -1) currentMesh->normIndices.push_back(verts[i].vertNormal);

		currentMesh->vertIndices.push_back(verts[0].vertIndex);
		if (verts[i].vertTex != -1) currentMesh->texIndices.push_back(verts[0].vertTex);
		if (verts[i].vertNormal != -1) currentMesh->normIndices.push_back(verts[0].vertNormal);
	}
}

void OBJGeometry::LoadMaterialsFromMTL(string filename) {
	std::ifstream f(string(Assets::MESHDIR + filename).c_str(), std::ios::in);
	if (!f) return;

	ShaderBase* pbrShader = Assets::AssetManager::LoadShader("PBRShader", "pbrvert.glsl", "pbrfrag.glsl");
	Material* material = nullptr;

	int counter = 0;
	while (!f.eof()) {
		std::string lineHeader, data;
		f >> lineHeader;

		if (lineHeader == MTLNEW) {
			f >> data;
			material = Assets::AssetManager::LoadMaterial(data, pbrShader);
		}
		else if (lineHeader == MTLDIFFUSE) {
			float r, g, b;
			f >> r >> g >> b;
			if (material) {
				Vector4 currentColour = material->GetColour();
				material->SetColour(Vector4(r, g, b, currentColour.w));
			}
		}
		else if (lineHeader == MTLDIFFUSEMAP) {
			f >> data;
			cout << "diffuse: " << NormalisePath(data) << endl;
			material->AddTextureParameter("diffuseTex", (OGLTexture*)Assets::AssetManager::LoadTexture(NormalisePath(data)));
		}
		else if (lineHeader == MTLBUMPMAP || lineHeader == MTLBUMPMAPALT) {
			f >> data;
			material->AddTextureParameter("bumpTex", (OGLTexture*)Assets::AssetManager::LoadTexture(NormalisePath(data)));
		}
		else if (lineHeader == MTLSPECMAP) {
			f >> data;
			material->AddTextureParameter("specularTex", (OGLTexture*)Assets::AssetManager::LoadTexture(NormalisePath(data)));
		}
		else if (lineHeader == MTLTRANS2) {
			float alpha;
			f >> alpha >> alpha >> alpha;
			if (material) {
				Vector4 currentColour = material->GetColour();
				material->SetColour(Vector4(currentColour.x, currentColour.y, currentColour.z, alpha));
			}
		}
	}
}

string OBJGeometry::NormalisePath(string path) {
	if (path.find_last_of('/') != string::npos) {
		int at = (int) path.find_last_of('/');
		path = path.substr(at + 1);
	}
	else if (path.find_last_of('\\') != string::npos) {
		int at = (int) path.find_last_of('\\');
		path = path.substr(at + 1);
	}
	return path;
}