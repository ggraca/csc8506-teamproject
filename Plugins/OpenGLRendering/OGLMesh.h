#pragma once
#include "../../Common/MeshGeometry.h"
#include "glad\glad.h"

#include <string>

namespace NCL {
	namespace Rendering {
		class OGLMesh : public MeshGeometry
		{
		public:
			enum MeshBuffer {
				VERTEX_BUFFER,
				COLOUR_BUFFER,
				TEXTURE_BUFFER,
				NORMAL_BUFFER,
				TANGENT_BUFFER,
				INDEX_BUFFER,
				SKELINDEX_BUFFER,
				SKELWEIGHT_BUFFER,
				MAX_BUFFER
			};

			friend class OGLRenderer;
			OGLMesh();
			OGLMesh(const std::string&filename);
			~OGLMesh();

			void RecalculateNormals();
			void GenerateTangents();

			void UploadToGPU() override;
			bool OnGPU() { return onGpu; }

			static OGLMesh* GenerateQuad();
			static OGLMesh* GenerateQuad(float minX, float maxX, float minY, float maxY, int currentWidth, int currentHeight);
			static void ChangeQuad(OGLMesh* quad, float minX, float maxX, float minY, float maxY);

		protected:
			GLuint	GetVAO()			const { return vao;			}
			int		GetSubMeshCount()	const { return subCount;	}

			void BindVertexAttribute(int attribSlot, int bufferID, int bindingID, int elementCount, int elementSize, int elementOffset);

			Vector3 GenerateTangent(const Vector3 &a, const Vector3 &b,
				const Vector3& c, const Vector2 & ta,
				const Vector2& tb, const Vector2 & tc);
			//void BufferData();

			int		subCount;

			GLuint vao;
			GLuint oglType;
			GLuint buffers[MAX_BUFFER];

			bool onGpu = false;
		};
	}
}