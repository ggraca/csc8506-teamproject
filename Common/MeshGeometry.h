#pragma once
#include <vector>

using std::vector;

namespace NCL {
	namespace Maths {
		class Vec2;
		class Vec3;
		class Vec4;
		class Mat4;
	}
	using namespace Maths;

	enum class GeometryPrimitive {
		Points,
		Lines,
		Triangles,
		TriangleFan,
		TriangleStrip,
		Patches
	};

	class MeshGeometry
	{
	public:
		virtual ~MeshGeometry();

		GeometryPrimitive GetPrimitiveType() const {
			return primType;
		}

		void SetPrimitiveType(GeometryPrimitive type) {
			primType = type;
		}

		unsigned int GetVertexCount() const {
			return (unsigned int) positions.size();
		}

		unsigned int GetIndexCount()  const {
			return (unsigned int) indices.size();
		}

		const vector<Vec3>&		GetPositionData()		const { return positions; }
		const vector<Vec2>&		GetTextureCoordData()	const { return texCoords; }
		const vector<Vec4>&		GetColourData()			const { return colours; }
		const vector<Vec3>&		GetNormalData()			const { return normals; }
		const vector<Vec3>&		GetTangentData()		const { return tangents; }
		const vector<unsigned int>& GetIndexData()			const { return indices; }

		void SetVertexPositions(const vector<Vec3>& newVerts);
		void SetVertexTextureCoords(const vector<Vec2>& newTex);

		void SetVertexColours(const vector<Vec4>& newColours);
		void SetVertexNormals(const vector<Vec3>& newNorms);
		void SetVertexTangents(const vector<Vec3>& newTans);
		void SetVertexIndices(const vector<unsigned int>& newIndices);


		void TransformVertices(const Mat4& byMatrix);

		void RecalculateNormals();
		void RecalculateTangents();

		virtual void UploadToGPU() = 0;

	protected:
		MeshGeometry();
		MeshGeometry(const std::string&filename);

		GeometryPrimitive	primType;
		vector<Vec3>		positions;

		vector<Vec2>		texCoords;
		vector<Vec4>		colours;
		vector<Vec3>		normals;
		vector<Vec3>		tangents;
		vector<unsigned int>	indices;
	};
}