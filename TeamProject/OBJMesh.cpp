#include "OBJMesh.h"
#include "Resource.h"

bool OBJLoader::LoadOBJMesh(std::string filename) {
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

	OGLShader* basicShader = new OGLShader("pbrvert.glsl", "pbrfrag.glsl");
	Material* material = Assets::AssetManager::LoadMaterial(sm->mtlType, basicShader);
	cout << sm->mtlType << endl;
	cout << "r: " << material->GetColour().x << endl;
	cout << "g: " << material->GetColour().y << endl;
	cout << "b: " << material->GetColour().z << endl;
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

void OBJLoader::LoadFaceFromFile(std::ifstream &f, OBJSubMesh* &currentMesh, std::vector<OBJSubMesh*> &inputSubMeshes) {
  if (!currentMesh) {
    currentMesh = new OBJSubMesh();
    inputSubMeshes.push_back(currentMesh);
  }

  std::string			faceData;
  getline(f, faceData);

  // f <vertex index>//<normal index>
  bool	skipTex = false;
  if (faceData.find("//") != std::string::npos) {
    skipTex = true;
  }

  // "f  0/0/0" becomes "f 0 0 0" etc
  for (size_t i = 0; i < faceData.length(); ++i) {
    if (faceData[i] == '/') {
      faceData[i] = ' ';
    }
  }

  int tempIndex;
  std::vector<int>	faceIndices;
  std::stringstream	ss(faceData);
  while (ss >> tempIndex) {
    faceIndices.push_back(tempIndex);
  }

  // This face has only vertex information;
  if (faceIndices.size() == 3) {
    currentMesh->vertIndices.push_back(faceIndices.at(0));
    currentMesh->vertIndices.push_back(faceIndices.at(1));
    currentMesh->vertIndices.push_back(faceIndices.at(2));
  }

  // This face has vertex, normal and tex information!
  else if (faceIndices.size() == 9) {
    for (int i = 0; i < 9; i += 3) {
      currentMesh->vertIndices.push_back(faceIndices.at(i));
      currentMesh->texIndices.push_back(faceIndices.at(i + 1));
      currentMesh->normIndices.push_back(faceIndices.at(i + 2));
    }
  }

  // This face has vertex, and one other index...
  else if (faceIndices.size() == 6) {
    for (int i = 0; i < 6; i += 2) {
      currentMesh->vertIndices.push_back(faceIndices.at(i));
      if (!skipTex) {		// a double slash means it's skipping tex info...
        currentMesh->texIndices.push_back(faceIndices.at(i + 1));
      }
      else {
        currentMesh->normIndices.push_back(faceIndices.at(i + 1));
      }
    }
  }

  // This face has more than 3 vertices. We assume it has all 3 properties
  else {

    // First Face
    for (int i = 0; i < 9; i += 3) {
      currentMesh->vertIndices.push_back(faceIndices.at(i));
      currentMesh->texIndices.push_back(faceIndices.at(i + 1));
      currentMesh->normIndices.push_back(faceIndices.at(i + 2));
    }

    // Following Faces
    for (int i = 6; i < faceIndices.size() - 3; i += 3) {
      currentMesh->vertIndices.push_back(faceIndices.at(i));
      currentMesh->texIndices.push_back(faceIndices.at(i + 1));
      currentMesh->normIndices.push_back(faceIndices.at(i + 2));

      currentMesh->vertIndices.push_back(faceIndices.at(i + 3));
      currentMesh->texIndices.push_back(faceIndices.at(i + 4));
      currentMesh->normIndices.push_back(faceIndices.at(i + 5));

      currentMesh->vertIndices.push_back(faceIndices.at(0));
      currentMesh->texIndices.push_back(faceIndices.at(1));
      currentMesh->normIndices.push_back(faceIndices.at(2));
    }
  }
}

void OBJLoader::LoadMaterialsFromMTL(string filename) {
	std::ifstream f(string(Assets::MESHDIR + filename).c_str(), std::ios::in);
	if (!f) return;

	OGLShader* basicShader = new OGLShader("pbrvert.glsl", "pbrfrag.glsl");
	Material* material = nullptr;

	int counter = 0;
	while (!f.eof()) {
		std::string lineHeader, data;
		f >> lineHeader;

		if (lineHeader == MTLNEW) {
			f >> data;
			material = Assets::AssetManager::LoadMaterial(data, basicShader);
			/*material->AddTextureParameter("diffuseTex", (OGLTexture*)Assets::AssetManager::LoadTexture("WoodPlanks/Wood_planks_COLOR.jpg"));
			material->AddTextureParameter("bumpTex", (OGLTexture*)Assets::AssetManager::LoadTexture("WoodPlanks/Wood_planks_NORM.jpg"));
			material->AddTextureParameter("specularTex", (OGLTexture*)Assets::AssetManager::LoadTexture("WoodPlanks/Wood_planks_DISP.jpg"));
			material->AddTextureParameter("metalnessTex", (OGLTexture*)Assets::AssetManager::LoadTexture("WoodPlanks/Wood_planks_SPEC.jpg"));*/
		}
		else if (lineHeader == MTLDIFFUSE) {
			float r, g, b;
			f >> r >> g >> b;
			if (material) material->SetColour(Vector4(r, g, b, 1));
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
	}
}

string OBJLoader::NormalisePath(string path) {
	if (path.find_last_of('/') != string::npos) {
		int at = path.find_last_of('/');
		path = path.substr(at + 1);
	}
	else if (path.find_last_of('\\') != string::npos) {
		int at = path.find_last_of('\\');
		path = path.substr(at + 1);
	}
	return path;
}

GameObject* OBJLoader::ToGameObject(GameWorld* world) {
	GameObject* root = new GameObject();
	
	world->AddGameObject(root);
	
	for (auto& mesh : children) {
		GameObject* go = new GameObject();

		go->AddComponent<RenderObject*>(new RenderObject(
			&go->GetTransform(),
			mesh,
			((OBJMesh*)mesh)->material
		));

		world->AddGameObject(go);
		root->AddChild(go);
	}
	return root;
}