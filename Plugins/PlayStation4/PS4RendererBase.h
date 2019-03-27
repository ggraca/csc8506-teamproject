#pragma once
#include "../../Common/RendererBase.h"

#include "PS4MemoryAware.h"

#include <gnm.h>
#include <gnmx\fetchshaderhelper.h>

#include <..\samples\sample_code\graphics\api_gnm\toolkit\allocators.h>
#include <..\samples\sample_code\graphics\api_gnm\toolkit\stack_allocator.h>
#include <..\samples\sample_code\graphics\api_gnm\toolkit\toolkit.h>
#include "PS4Frame.h"
#include "PS4Texture.h"

using namespace sce;
using namespace Gnmx;
using namespace Gnmx::Toolkit;

namespace NCL {
	namespace Maths {
		class Mat4;
	}
	namespace PS4 {
		enum MemoryLocation {
			GARLIC,
			ONION,
			MEMORYMAX
		};

		struct PS4ScreenBuffer { // these are instead of FBOs - keep state of colour and depth buffers
			sce::Gnm::RenderTarget		colourTarget;
			sce::Gnm::DepthRenderTarget depthTarget;
		};

		class PS4Window;
		class PS4Shader;
		class PS4Mesh;

		class RenderObjectPS4;

		class PS4RendererBase :
			public RendererBase, public PS4MemoryAware
		{
		public:
			PS4RendererBase(PS4Window* window);
			~PS4RendererBase();

		protected:
			virtual void RenderActiveScene() {};

			void	OnWindowResize(int w, int h) override;
			void	BeginFrame()    override;
			void	RenderFrame()	override;
			void	EndFrame()		override;

			void	SwapScreenBuffer();
			void	SwapCommandBuffer();
			
			void	DrawMesh(const PS4Mesh& mesh);
			void	BindFBO(void*buffer);
			void	ClearBuffer(bool colour, bool depth, bool stencil);

		private:
			void	InitialiseMemoryAllocators();
			void	InitialiseVideoSystem();
			void	InitialiseGCMRendering();

			void	DestroyMemoryAllocators();
			void	DestroyVideoSystem();
			void	DestroyGCMRendering();

			//need to generate front, back, and depth buffers as GNM doesn't do automatically like OpenGL
			PS4ScreenBuffer* GenerateScreenBuffer(uint width, uint height, bool colour = true, bool depth = true, bool stencil = false);

		protected:
			int currentGPUBuffer;

			const int _MaxCMDBufferCount;

			//VIDEO SYSTEM VARIABLES
			int videoHandle;		//Handle to video system

		//SCREEN BUFFER VARIABLES
			const int			_bufferCount;	//How many screen buffers should we have
			int					currentScreenBuffer;
			int					prevScreenBuffer;
			PS4ScreenBuffer**	screenBuffers;	//Pointer to our screen buffers
		//Memory Allocation
			const int _GarlicMemory;
			const int _OnionMemory;

			sce::Gnmx::Toolkit::StackAllocator	stackAllocators[MEMORYMAX];

			//default data
			PS4Shader*	defaultShader;
			PS4Mesh*	defaultMesh;
			PS4Texture* defaultTexture;

			//Individual Frames
			PS4Frame*	frames;

			Maths::Mat4*	viewProjMat;
			Gnm::Buffer	cameraBuffer;

			int framesSubmitted;

			//Per frame pointers...
			PS4ScreenBuffer*		currentPS4Buffer;  //Pointer to whichever buffer we're currently using...
			Gnmx::GnmxGfxContext*	currentGFXContext;
			PS4Frame*				currentFrame;
		};
	}
}

