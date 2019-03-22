#pragma once
#include "../../Common/TextureBase.h"
#include <string>
#include <gnm\texture.h>
#include "PS4MemoryAware.h"
namespace NCL {
	namespace PS4 {
		class PS4Texture :
			public Rendering::TextureBase, public PS4MemoryAware
		{
		public:
			friend class PS4RendererBase;

			static PS4Texture* LoadTextureFromFile(const std::string& filename);

			const sce::Gnm::Texture&  GetAPITexture() {
				return apiTexture;
			}
			PS4Texture();
			~PS4Texture();


		protected:

			sce::Gnm::Texture apiTexture;
		};
	}
}