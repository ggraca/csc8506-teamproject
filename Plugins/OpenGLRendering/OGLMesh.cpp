#include "OGLMesh.h"
#include "../../Common/Vector2.h"
#include "../../Common/Vector3.h"
#include "../../Common/Vector4.h"

using namespace NCL;
using namespace NCL::Rendering;
using namespace NCL::Maths;

OGLMesh::OGLMesh() {
	vao			= 0;
	subCount	= 1;

	for (int i = 0; i < MAX_BUFFER; ++i) {
		buffers[i] = 0;
	}
}

OGLMesh::OGLMesh(const std::string&filename) : MeshGeometry(filename){
	vao		 = 0;
	subCount = 1;

	for (int i = 0; i < MAX_BUFFER; ++i) {
		buffers[i] = 0;
	}
}

OGLMesh::~OGLMesh()	{
	glDeleteVertexArrays(1, &vao);			//Delete our VAO
	glDeleteBuffers(MAX_BUFFER, buffers);	//Delete our VBOs
}

void CreateVertexBuffer(GLuint& buffer, int byteCount, char* data) {
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, byteCount, data, GL_STATIC_DRAW);
}

void OGLMesh::BindVertexAttribute(int attribSlot, int buffer, int bindingID, int elementCount, int elementSize, int elementOffset) {
	glEnableVertexAttribArray(attribSlot);
	glVertexAttribFormat(attribSlot, elementCount, GL_FLOAT, false, 0);
	glVertexAttribBinding(attribSlot, bindingID);

	glBindVertexBuffer(bindingID, buffer, elementOffset, elementSize);
}

void OGLMesh::UploadToGPU() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	int numVertices = GetVertexCount();
	int numIndices	= GetIndexCount();

	//Buffer vertex data
	if (!GetPositionData().empty()) {
		CreateVertexBuffer(buffers[VERTEX_BUFFER], numVertices * sizeof(Vector3), (char*)GetPositionData().data());
		BindVertexAttribute(VERTEX_BUFFER, buffers[VERTEX_BUFFER], VERTEX_BUFFER, 3, sizeof(Vector3), 0);
	}

	if (!GetColourData().empty()) {	//buffer colour data
		CreateVertexBuffer(buffers[COLOUR_BUFFER], numVertices * sizeof(Vector4), (char*)GetColourData().data());
		BindVertexAttribute(COLOUR_BUFFER, buffers[COLOUR_BUFFER], COLOUR_BUFFER, 4, sizeof(Vector4), 0);
	}
	if (!GetTextureCoordData().empty()) {	//Buffer texture data
		CreateVertexBuffer(buffers[TEXTURE_BUFFER], numVertices * sizeof(Vector2), (char*)GetTextureCoordData().data());
		BindVertexAttribute(TEXTURE_BUFFER, buffers[TEXTURE_BUFFER], TEXTURE_BUFFER, 2, sizeof(Vector2), 0);
	}

	if (!GetNormalData().empty()) {	//Buffer normal data
		CreateVertexBuffer(buffers[NORMAL_BUFFER], numVertices * sizeof(Vector3), (char*)GetNormalData().data());
		BindVertexAttribute(NORMAL_BUFFER, buffers[NORMAL_BUFFER], NORMAL_BUFFER, 3, sizeof(Vector3), 0);
	}

	if (!GetTangentData().empty()) {	//Buffer tangent data
		CreateVertexBuffer(buffers[TANGENT_BUFFER], numVertices * sizeof(Vector3), (char*)GetTangentData().data());
		BindVertexAttribute(TANGENT_BUFFER, buffers[TANGENT_BUFFER], TANGENT_BUFFER, 3, sizeof(Vector3), 0);
	}

	if (!GetIndexData().empty()) {		//buffer index data
		glGenBuffers(1, &buffers[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), (int*)GetIndexData().data(), GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

OGLMesh* OGLMesh::GenerateQuad() {
	OGLMesh* newMesh = new OGLMesh();

	vector<Vector3> vertices;
	vertices.push_back(Vector3(-1.0f, -1.0f, 0.0f));
	vertices.push_back(Vector3(-1.0f, 1.0f, 0.0f));
	vertices.push_back(Vector3(1.0f, -1.0f, 0.0f));
	vertices.push_back(Vector3(1.0f, 1.0f, 0.0f));
	newMesh->SetVertexPositions(vertices);

	vector<Vector2> textureCoords;
	textureCoords.push_back(Vector2(0.0f, 1.0f));
	textureCoords.push_back(Vector2(0.0f, 0.0f));
	textureCoords.push_back(Vector2(1.0f, 1.0f));
	textureCoords.push_back(Vector2(1.0f, 0.0f));
	newMesh->SetVertexTextureCoords(textureCoords);

	vector<Vector4> colours;
	colours.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	colours.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	colours.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	colours.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	newMesh->SetVertexColours(colours);

	vector<Vector3> normals;
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	newMesh->SetVertexNormals(normals);

	vector<Vector3> tangents;
	tangents.push_back(Vector3(1.0f, 0.0f, 0.0f));
	tangents.push_back(Vector3(1.0f, 0.0f, 0.0f));
	tangents.push_back(Vector3(1.0f, 0.0f, 0.0f));
	tangents.push_back(Vector3(1.0f, 0.0f, 0.0f));
	newMesh->SetVertexTangents(tangents);

	return newMesh;
}

OGLMesh* OGLMesh::GenerateQuad(float minX, float maxX, float minY, float maxY) {
	OGLMesh* newMesh = new OGLMesh();
	float width = 1280;
	float height = 720;
	vector<Vector3> vertices;
	vertices.push_back(Vector3((minX / width) * 2 - 1, (minY / height) * 2 - 1, 0.0f));
	vertices.push_back(Vector3((minX / width) * 2 - 1, (maxY / height) * 2 - 1, 0.0f));
	vertices.push_back(Vector3((maxX / width) * 2 - 1, (minY / height) * 2 - 1, 0.0f));
	vertices.push_back(Vector3((maxX / width) * 2 - 1, (maxY / height) * 2 - 1, 0.0f));
	newMesh->SetVertexPositions(vertices);

	vector<Vector2> textureCoords;
	textureCoords.push_back(Vector2(0.0f, 1.0f));
	textureCoords.push_back(Vector2(0.0f, 0.0f));
	textureCoords.push_back(Vector2(1.0f, 1.0f));
	textureCoords.push_back(Vector2(1.0f, 0.0f));
	newMesh->SetVertexTextureCoords(textureCoords);

	vector<Vector4> colours;
	colours.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	colours.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	colours.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	colours.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	newMesh->SetVertexColours(colours);

	vector<Vector3> normals;
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
	newMesh->SetVertexNormals(normals);

	//vector<Vector3> tangents;
	//tangents.push_back(Vector3(1.0f, 0.0f, 0.0f));
	//tangents.push_back(Vector3(1.0f, 0.0f, 0.0f));
	//tangents.push_back(Vector3(1.0f, 0.0f, 0.0f));
	//tangents.push_back(Vector3(1.0f, 0.0f, 0.0f));
	//newMesh->SetVertexTangents(tangents);

	return newMesh;
}

void OGLMesh::ChangeQuad(OGLMesh* quad, float minX, float maxX, float minY, float maxY) {

	float width = 1280;
	float height = 720;
	vector<Vector3> vertices;
	vertices.push_back(Vector3((minX / width) * 2 - 1, (minY / height) * 2 - 1, 0.0f));
	vertices.push_back(Vector3((minX / width) * 2 - 1, (maxY / height) * 2 - 1, 0.0f));
	vertices.push_back(Vector3((maxX / width) * 2 - 1, (minY / height) * 2 - 1, 0.0f));
	vertices.push_back(Vector3((maxX / width) * 2 - 1, (maxY / height) * 2 - 1, 0.0f));
	quad->SetVertexPositions(vertices);
}

void OGLMesh::RecalculateNormals() {
	normals.clear();

	if (indices.size() > 0) {
		for (size_t i = 0; i < positions.size(); i++) {
			normals.emplace_back(Vector3());
		}

		for (size_t i = 0; i < indices.size(); i += 3) {
			Vector3& a = positions[indices[i+0]];
			Vector3& b = positions[indices[i+1]];
			Vector3& c = positions[indices[i+2]];

			Vector3 normal = Vector3::Cross(b - a, c - a);
			normal.Normalise();

			normals[indices[i + 0]] += normal;
			normals[indices[i + 1]] += normal;
			normals[indices[i + 2]] += normal;
		}
		for (size_t i = 0; i < normals.size(); ++i) {
			normals[i].Normalise();
		}
	}
	else {

	}
}