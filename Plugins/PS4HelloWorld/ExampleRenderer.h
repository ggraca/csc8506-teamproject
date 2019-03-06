#pragma once
#include "../PlayStation4/PS4RendererBase.h"

namespace NCL {
	namespace PS4 {
		class ExampleRenderer :
			public PS4RendererBase
		{
		public:
			ExampleRenderer(PS4Window* window);
			~ExampleRenderer();

			void Update(float dt)	 override;

		protected:
			void DrawRenderObject(RenderObject* o);
			//void RenderFrame()		 override;
			void RenderActiveScene() override;


			float rotation;
			RenderObject* defaultObject[2];
		};
	}
}

