#include "PS4RendererBase.h"
#include "PS4Window.h"
#include <video_out.h>	//Video System
#include "PS4Shader.h"
#include "PS4Mesh.h"
#include <gnmx\basegfxcontext.h>

#include <.\graphics\api_gnm\toolkit\allocators.h>
#include <.\graphics\api_gnm\toolkit\stack_allocator.h>

#include "../../Common/Matrix4.h"

using namespace sce::Vectormath::Scalar::Aos;

namespace SonyMath = sce::Vectormath::Scalar::Aos;

#include <iostream>

using namespace NCL;
using namespace NCL::PS4;

sce::Gnmx::Toolkit::IAllocator	oAllocator;
sce::Gnmx::Toolkit::IAllocator	gAllocator;

PS4RendererBase::PS4RendererBase(PS4Window* window)
	: RendererBase(*window),
	_MaxCMDBufferCount(3), //setting how many gpu and screen buffers need
	  _bufferCount(3),
	  _GarlicMemory(1024 * 1024 * 512),
	  _OnionMemory( 1024 * 1024 * 256)
{
	framesSubmitted		 = 0;
	currentGPUBuffer	 = 0;
	currentScreenBuffer	 = 0;
	prevScreenBuffer	 = 0;

	std::cerr << "Starting Rich Code!" << std::endl;

	currentGFXContext	 = nullptr;

	InitialiseMemoryAllocators();

	InitialiseGCMRendering();
	InitialiseVideoSystem();

	defaultShader = PS4Shader::GenerateShader("/app0/VertexShader.sb","/app0/PixelShader.sb");
	defaultMesh		= PS4Mesh::GenerateQuad();
	defaultTexture	= PS4Texture::LoadTextureFromFile("/app0/doge.gnf");

	viewProjMat		= (Mat4*)onionAllocator->allocate(sizeof(Mat4), Gnm::kEmbeddedDataAlignment4);
	*viewProjMat	= Mat4();

	cameraBuffer.initAsConstantBuffer(viewProjMat, sizeof(Mat4));
	cameraBuffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO); // it's a constant buffer, so read-only is OK

	EndFrame(); //buffers are swapped here
}

PS4RendererBase::~PS4RendererBase()	{
	delete defaultMesh;
	delete defaultTexture;
	delete defaultShader;

	DestroyGCMRendering();
	DestroyVideoSystem();
	DestroyMemoryAllocators();
}

void	PS4RendererBase::InitialiseVideoSystem() {
	screenBuffers = new PS4ScreenBuffer*[_bufferCount];

	for (int i = 0; i < _bufferCount; ++i) {
		screenBuffers[i] = GenerateScreenBuffer(1920, 1080);
	}

	//Now we can open up the video port
	videoHandle = sceVideoOutOpen(0, SCE_VIDEO_OUT_BUS_TYPE_MAIN, 0, NULL); //returns an int handle - similar to opengl int name

	SceVideoOutBufferAttribute attribute; //defines how colour buffer should be accessed
	sceVideoOutSetBufferAttribute(&attribute,
		SCE_VIDEO_OUT_PIXEL_FORMAT_B8_G8_R8_A8_SRGB, //how large each channel is per pixel
		SCE_VIDEO_OUT_TILING_MODE_TILE, //tiling type
		SCE_VIDEO_OUT_ASPECT_RATIO_16_9, //screen width and height
		screenBuffers[0]->colourTarget.getWidth(),
		screenBuffers[0]->colourTarget.getHeight(),
		screenBuffers[0]->colourTarget.getPitch()
	);

	void* bufferAddresses[_bufferCount];

	for (int i = 0; i < _bufferCount; ++i) {
		bufferAddresses[i] = screenBuffers[i]->colourTarget.getBaseAddress();
	}

	sceVideoOutRegisterBuffers(videoHandle, 0, bufferAddresses, _bufferCount, &attribute); //registering screen buffers to video system - takes array of addresses to colour targets
}

void	PS4RendererBase::InitialiseGCMRendering() {
	frames = (PS4Frame*)onionAllocator->allocate(sizeof(PS4Frame) * _MaxCMDBufferCount, alignof(PS4Frame)); //using onion as both cpu and gpu use command buffer

	for (int i = 0; i < _MaxCMDBufferCount; ++i) {
		new (&frames[i])PS4Frame();
	}

	sce::Gnmx::Toolkit::Allocators allocators = sce::Gnmx::Toolkit::Allocators(oAllocator, gAllocator, ownerHandle);
	Gnmx::Toolkit::initializeWithAllocators(&allocators); //letting gnmx toolkit know about allocators
}

void	PS4RendererBase::InitialiseMemoryAllocators() {
	stackAllocators[GARLIC].init(SCE_KERNEL_WC_GARLIC, _GarlicMemory);
	stackAllocators[ONION ].init(SCE_KERNEL_WB_ONION , _OnionMemory);

	oAllocator = Gnmx::Toolkit::GetInterface(&stackAllocators[ONION]);
	gAllocator = Gnmx::Toolkit::GetInterface(&stackAllocators[GARLIC]);

	this->garlicAllocator   = &gAllocator;
	this->onionAllocator	= &oAllocator;
	Gnm::registerOwner(&ownerHandle, "PS4RendererBase");
}

void PS4RendererBase::DestroyMemoryAllocators() {
	stackAllocators[GARLIC].deinit();
	stackAllocators[ONION ].deinit();
}

PS4ScreenBuffer*	PS4RendererBase::GenerateScreenBuffer(uint width, uint height, bool colour, bool depth, bool stencil) {
	PS4ScreenBuffer* buffer = new PS4ScreenBuffer(); //contains gnm randertarget and deptherrendertarget classes

	if (colour) {	
		Gnm::RenderTargetSpec spec;
		spec.init();
		spec.m_width		= width;
		spec.m_height		= height;
		spec.m_numSamples	= Gnm::kNumSamples1;
		spec.m_numFragments = Gnm::kNumFragments1;
		spec.m_colorFormat	= Gnm::kDataFormatB8G8R8A8UnormSrgb; //8 bits per pixel for rgba, auto gamma correction	

		GpuAddress::computeSurfaceTileMode(Gnm::GpuMode::kGpuModeBase, &spec.m_colorTileModeHint, GpuAddress::kSurfaceTypeColorTargetDisplayable, spec.m_colorFormat, 1);

		int32_t success = buffer->colourTarget.init(&spec);

		if (success != SCE_GNM_OK) {
			bool a = true;
		}

		const Gnm::SizeAlign colourAlign = buffer->colourTarget.getColorSizeAlign(); //aligning buffers

		void *colourMemory = stackAllocators[GARLIC].allocate(colourAlign);//garlic as only gpu accessing buffers

		Gnm::registerResource(nullptr, ownerHandle, colourMemory, colourAlign.m_size,
			"Colour", Gnm::kResourceTypeDepthRenderTargetBaseAddress, 0);

		buffer->colourTarget.setAddresses(colourMemory, NULL, NULL);
	}

	if (depth) {
		Gnm::DepthRenderTargetSpec spec;
		spec.init();
		spec.m_width			= width;
		spec.m_height			= height;
		spec.m_numFragments		= Gnm::kNumFragments1;
		spec.m_zFormat			= Gnm::ZFormat::kZFormat32Float;
		spec.m_stencilFormat	= (stencil ? Gnm::kStencil8 : Gnm::kStencilInvalid);

		GpuAddress::computeSurfaceTileMode(Gnm::GpuMode::kGpuModeBase, &spec.m_tileModeHint, GpuAddress::kSurfaceTypeDepthTarget, Gnm::DataFormat::build(spec.m_zFormat), 1);

		void* stencilMemory = 0;

		int32_t success = buffer->depthTarget.init(&spec);

		if (success != SCE_GNM_OK) {
			bool a = true;
		}

		void *depthMemory = stackAllocators[GARLIC].allocate(buffer->depthTarget.getZSizeAlign());

		Gnm::registerResource(nullptr, ownerHandle, depthMemory, buffer->depthTarget.getZSizeAlign().m_size,
			"Depth", Gnm::kResourceTypeDepthRenderTargetBaseAddress, 0);

		if (stencil) {
			stencilMemory = stackAllocators[GARLIC].allocate(buffer->depthTarget.getStencilSizeAlign());

			Gnm::registerResource(nullptr, ownerHandle, stencilMemory, buffer->depthTarget.getStencilSizeAlign().m_size,
				"Stencil", Gnm::kResourceTypeDepthRenderTargetBaseAddress, 0);
		}

		buffer->depthTarget.setAddresses(depthMemory, stencilMemory);
	}

	return buffer;
}

void	PS4RendererBase::DestroyGCMRendering() {
	//onionAllocator->release(frames);
}

void	PS4RendererBase::DestroyVideoSystem() {
	for (int i = 0; i < _bufferCount; ++i) {
		delete screenBuffers[i];
	}
	delete[] screenBuffers;

	sceVideoOutClose(videoHandle);
}

void PS4RendererBase::RenderFrame()			{//renderscene function
	currentFrame->StartFrame();	//blocks until correct tag written to frametag

	currentGFXContext->waitUntilSafeForRendering(videoHandle, currentGPUBuffer); //need gnm to finish processsing before displaying on screen

	BindFBO(currentPS4Buffer);
	ClearBuffer(true, true, true);

	defaultShader->SubmitShaderSwitch(*currentGFXContext);

	//Primitive Setup State
	Gnm::PrimitiveSetup primitiveSetup;
	primitiveSetup.init();
	primitiveSetup.setCullFace(Gnm::kPrimitiveSetupCullFaceNone);
	primitiveSetup.setFrontFace(Gnm::kPrimitiveSetupFrontFaceCcw);
	//primitiveSetup.setPolygonMode()
	currentGFXContext->setPrimitiveSetup(primitiveSetup);

	//Screen Access State
	Gnm::DepthStencilControl dsc;//adds object to command list
	dsc.init();
	//turns on depth test (whether or not to write to depth buffer, how to pass depth test)
	dsc.setDepthControl(Gnm::kDepthControlZWriteEnable, Gnm::kCompareFuncLessEqual); // same as gl_lequal in opengl
	dsc.setDepthEnable(true);
	currentGFXContext->setDepthStencilControl(dsc);

	Gnm::Sampler trilinearSampler;
	trilinearSampler.init();
	trilinearSampler.setMipFilterMode(Gnm::kMipFilterModeLinear);//enabling mipmapping (& so also trilinear filtering)

	//use the following to apply method of filtering for minification and magnification
	//trilinearSampler.setXyFilterMode(Gnm::kFilterModeBilinear, Gnm::kFilterModeBilinear);

	//textures and samplers are dif in gnm (same in opengl) so need to set up separately
	currentGFXContext->setTextures(Gnm::kShaderStagePs, 0, 1, &defaultTexture->GetAPITexture());
	currentGFXContext->setSamplers(Gnm::kShaderStagePs, 0, 1, &trilinearSampler);

	viewProjMat->ToIdentity();

	RenderActiveScene();

	currentFrame->EndFrame();

	framesSubmitted++;
}

void	PS4RendererBase::OnWindowResize(int w, int h)  {

}

void	PS4RendererBase::BeginFrame()   {

}

void PS4RendererBase::EndFrame()			{
	SwapScreenBuffer();
	SwapCommandBuffer();
}

void	PS4RendererBase::SwapScreenBuffer() {
	prevScreenBuffer	= currentScreenBuffer;
	currentScreenBuffer = (currentScreenBuffer + 1) % _bufferCount;
	sceVideoOutSubmitFlip(videoHandle, prevScreenBuffer, SCE_VIDEO_OUT_FLIP_MODE_VSYNC, 0);

	currentPS4Buffer = screenBuffers[currentScreenBuffer];
}

void	PS4RendererBase::SwapCommandBuffer() {
	if (currentGFXContext) {	
		if (currentGFXContext->submit() != sce::Gnm::kSubmissionSuccess) {
			std::cerr << "Graphics queue submission failed?" << std::endl;
		}
		Gnm::submitDone();
	}

	currentGPUBuffer	= (currentGPUBuffer + 1) % _MaxCMDBufferCount;
	 
	currentFrame		= &frames[currentGPUBuffer]; 
	currentGFXContext	= &currentFrame->GetCommandBuffer();
}
 
void	PS4RendererBase::BindFBO(void* buffer) {
	currentPS4Buffer = (PS4ScreenBuffer*)buffer;
	currentGFXContext->setRenderTargetMask(0xF);
	currentGFXContext->setRenderTarget(0, &currentPS4Buffer->colourTarget);
	currentGFXContext->setDepthRenderTarget(&currentPS4Buffer->depthTarget);

	currentGFXContext->setupScreenViewport(0, 0, currentPS4Buffer->colourTarget.getWidth(), currentPS4Buffer->colourTarget.getHeight(), 0.5f, 0.5f); //(x coord, y coord, width, height, depth bias, depth scale)
	currentGFXContext->setScreenScissor(0, 0, currentPS4Buffer->colourTarget.getWidth(), currentPS4Buffer->colourTarget.getHeight());
	currentGFXContext->setWindowScissor(0, 0, currentPS4Buffer->colourTarget.getWidth(), currentPS4Buffer->colourTarget.getHeight(), sce::Gnm::WindowOffsetMode::kWindowOffsetDisable);
	currentGFXContext->setGenericScissor(0, 0, currentPS4Buffer->colourTarget.getWidth(), currentPS4Buffer->colourTarget.getHeight(), sce::Gnm::WindowOffsetMode::kWindowOffsetDisable);
}

void	PS4RendererBase::ClearBuffer(bool colour, bool depth, bool stencil) {
	if (colour) {
		//Vector4 defaultClearColour(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, 1.0f);
		SonyMath::Vector4 defaultClearColour(0.1f, 0.1f, 0.1f, 1.0f);


		SurfaceUtil::clearRenderTarget(*currentGFXContext, &currentPS4Buffer->colourTarget, defaultClearColour);
	}

	if (depth) {
		float defaultDepth = 1.0f;
		SurfaceUtil::clearDepthTarget(*currentGFXContext, &currentPS4Buffer->depthTarget, defaultDepth);
	}

	if (stencil && currentPS4Buffer->depthTarget.getStencilReadAddress()) {
		int defaultStencil = 0;
		SurfaceUtil::clearStencilTarget(*currentGFXContext, &currentPS4Buffer->depthTarget, defaultStencil);
	}
}

void PS4RendererBase::DrawMesh(const PS4Mesh& mesh) {
	defaultMesh->SubmitDraw(*currentGFXContext, Gnm::ShaderStage::kShaderStageVs);
}
