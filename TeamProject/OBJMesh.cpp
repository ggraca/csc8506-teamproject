#include "OBJMesh.h"



bool	OBJMesh::LoadOBJMesh(std::string filename) {
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

  while (!f.eof()) {
    std::string lineHeader;
    f >> lineHeader;

    if (lineHeader == OBJCOMMENT) { continue; }
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
    else if (lineHeader == OBJMTLLIB) { f >> currentMtlLib; }
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
    else if (lineHeader == OBJFACE) {	//This is an object face!
      if (!currentMesh) {
        currentMesh = new OBJSubMesh();
        inputSubMeshes.push_back(currentMesh);
      }

      std::string			faceData;		//Keep the entire line in this!
      std::vector<int>	faceIndices;	//Keep the extracted indices in here!

      getline(f, faceData);		//Use a string helper function to read in the entire face line

      /*
      It's possible an OBJ might have normals defined, but not tex coords!
      Such files should then have a face which looks like this:

        f <vertex index>//<normal index>
        instead of <vertex index>/<tex coord>/<normal index>

        you can be some OBJ files will have "/ /" instead of "//" though... :(
      */
      bool	skipTex = false;
      if (faceData.find("//") != std::string::npos) {
        skipTex = true;
      }

      /*
      Count the number of slashes, but also convert the slashes to spaces
      so that string streaming of ints doesn't fail on the slash

        "f  0/0/0" becomes "f 0 0 0" etc
      */
      for (size_t i = 0; i < faceData.length(); ++i) {
        if (faceData[i] == '/') {
          faceData[i] = ' ';
        }
      }

      /*
      Now string stream the indices from the string into a temporary
      vector.
      */
      int tempIndex;
      std::stringstream	ss(faceData);
      while (ss >> tempIndex) {
        faceIndices.push_back(tempIndex);
      }

      if (faceIndices.size() == 3) {	//This face has only vertex information;
        currentMesh->vertIndices.push_back(faceIndices.at(0));
        currentMesh->vertIndices.push_back(faceIndices.at(1));
        currentMesh->vertIndices.push_back(faceIndices.at(2));
      }
      else if (faceIndices.size() == 9) {	//This face has vertex, normal and tex information!
        for (int i = 0; i < 9; i += 3) {
          currentMesh->vertIndices.push_back(faceIndices.at(i));
          currentMesh->texIndices.push_back(faceIndices.at(i + 1));
          currentMesh->normIndices.push_back(faceIndices.at(i + 2));
        }
      }
      else if (faceIndices.size() == 6) {	//This face has vertex, and one other index...
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
    else {
      std::cout << "OBJMesh::LoadOBJMesh Unknown file data:" << lineHeader << std::endl;
    }
  }

  f.close();

  //We now have all our mesh data loaded in...Now to convert it into OpenGL vertex buffers!
  for (unsigned int i = 0; i < inputSubMeshes.size(); ) {
    OBJSubMesh*sm = inputSubMeshes[i];

    /*
    We're going to be lazy and turn the indices into an absolute list
    of vertex attributes (it makes handling the submesh list easier)
    */
    if (sm->vertIndices.empty()) {
      delete sm;
      inputSubMeshes.erase(inputSubMeshes.begin() + i);
      continue;
    }
    else {
      OBJMesh*m;

      if (i == 0) m = this;
      else m = new OBJMesh();

      m->SetTexturesFromMTL(sm->mtlSrc, sm->mtlType);

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

      // TODO: Tangents
      // m->GenerateTangents();

      m->UploadToGPU();

      if (i != 0) AddChild(m);
    }

    delete inputSubMeshes[i];
    ++i;
  }
  return true;
}

void	OBJMesh::SetTexturesFromMTL(string &mtlFile, string &mtlType) {
  if (mtlType.empty() || mtlFile.empty()) {
    return;
  }

  map <string, MTLInfo>::iterator i = materials.find(mtlType);

  if (i != materials.end()) {
    if (!i->second.diffuse.empty()) {
      //texture = i->second.diffuseNum;
    }
    if (!i->second.bump.empty()) {
      //bumpTexture = i->second.bumpNum;
    }
    return;
  }
    
  std::ifstream f(string(Assets::MESHDIR + mtlFile).c_str(), std::ios::in);
  if (!f) return;

  MTLInfo currentMTL;
  string  currentMTLName;

  int mtlCount = 0;

  while (!f.eof()) {
    std::string currentLine;
    f >> currentLine;

    if (currentLine == MTLNEW) {
      if (mtlCount > 0) {
        materials.insert(std::make_pair(currentMTLName, currentMTL));
      }
      currentMTL.diffuse = "";
      currentMTL.bump = "";

      f >> currentMTLName;

      mtlCount++;
    }
    else if (currentLine == MTLDIFFUSEMAP) {
      f >> currentMTL.diffuse;

      if (currentMTL.diffuse.find_last_of('/') != string::npos) {
        int at = currentMTL.diffuse.find_last_of('/');
        currentMTL.diffuse = currentMTL.diffuse.substr(at + 1);
      }
      else if (currentMTL.diffuse.find_last_of('\\') != string::npos) {
        int at = currentMTL.diffuse.find_last_of('\\');
        currentMTL.diffuse = currentMTL.diffuse.substr(at + 1);
      }

      if (!currentMTL.diffuse.empty()) {
        //currentMTL.diffuseTex = (OGLTexture*) TextureLoader::LoadAPITexture(Assets::TEXTUREDIR + currentMTL.diffuse);
      }
    }
    else if (currentLine == MTLBUMPMAP || currentLine == MTLBUMPMAPALT) {
      f >> currentMTL.bump;

      if (currentMTL.bump.find_last_of('/') != string::npos) {
        int at = currentMTL.bump.find_last_of('/');
        currentMTL.bump = currentMTL.bump.substr(at + 1);
      }
      else if (currentMTL.bump.find_last_of('\\') != string::npos) {
        int at = currentMTL.bump.find_last_of('\\');
        currentMTL.bump = currentMTL.bump.substr(at + 1);
      }

      if (!currentMTL.bump.empty()) {
        //currentMTL.bumpNum = SOIL_load_OGL_texture(string(TEXTUREDIR + currentMTL.bump).c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
      }
    }
  }

  materials.insert(std::make_pair(currentMTLName, currentMTL));

  SetTexturesFromMTL(mtlFile, mtlType);
}