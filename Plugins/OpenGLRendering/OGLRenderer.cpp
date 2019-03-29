#include "OGLRenderer.h"
#include "OGLShader.h"
#include "OGLMesh.h"
#include "OGLTexture.h"

#include "../../Common/SimpleFont.h"

#include "../../Common/MeshGeometry.h"

#ifdef _WIN32
#include "../../Common/Win32Window.h"

#include "KHR\khrplatform.h"
#include "glad\glad.h"

#include "GL/GL.h"
#include "KHR/WGLext.h"
#endif

using namespace NCL;
using namespace NCL::Rendering;

OGLRenderer::OGLRenderer(Window& w) : RendererBase(w)	{
#ifdef _WIN32
	InitWithWin32(w);
#endif
	boundMesh	= nullptr;
	boundShader = nullptr;

	currentWidth	= (int)w.GetScreenSize().x;
	currentHeight	= (int)w.GetScreenSize().y;

	TextureLoader::RegisterAPILoadFunction(OGLTexture::RGBATextureFromFilename);
	TextureLoader::RegisterAPILoadCubeFunction(OGLTexture::CubeTextureFromFilename);
	ShaderLoader::RegisterAPILoadFunction(OGLShader::LoadShader);

	font		= new SimpleFont("PressStart2P.fnt", "PressStart2P.png");

	OGLTexture* t = (OGLTexture*)font->GetTexture();

	if (t) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t->texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	debugShader = (OGLShader*)Assets::AssetManager::LoadShader("DebugShader", "debugVert.glsl", "debugFrag.glsl");
}

OGLRenderer::~OGLRenderer()	{
	delete font;

#ifdef _WIN32
	DestroyWithWin32();
#endif
}

void OGLRenderer::OnWindowResize(int w, int h)	 {
	currentWidth	= w;
	currentHeight	= h;
}

void OGLRenderer::BeginFrame()		{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	BindShader(nullptr);
	BindMesh(nullptr);
}

void OGLRenderer::RenderFrame()		{

}

void OGLRenderer::EndFrame()		{
	DrawDebugData();
	::SwapBuffers(deviceContext);
}

void OGLRenderer::GenerateIrradianceMap(TextureBase* skybox, TextureBase* irradianceMap, ShaderBase* convShader, MeshGeometry* cube, void* ConvFBO) {
	BindShader(convShader);

	Matrix4 captureProjection = Matrix4::Perspective(0.1f, 10.0f, 1.0f, 90.0f);
	Matrix4 captureViews[] =
	{
	   Matrix4::BuildViewMatrix(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f,   0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
	   Matrix4::BuildViewMatrix(Vector3(0.0f, 0.0f, 0.0f), Vector3(-1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
	   Matrix4::BuildViewMatrix(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f,   1.0f,  0.0f), Vector3(0.0f,  0.0f,  1.0f)),
	   Matrix4::BuildViewMatrix(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f,  -1.0f,  0.0f), Vector3(0.0f,  0.0f, -1.0f)),
	   Matrix4::BuildViewMatrix(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f,   0.0f,  1.0f), Vector3(0.0f, -1.0f,  0.0f)),
	   Matrix4::BuildViewMatrix(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f,   0.0f, -1.0f), Vector3(0.0f, -1.0f,  0.0f))
	};

	BindMatrix4ToShader(captureProjection, "projMatrix");
	BindTextureCubeToShader(skybox, "cubeTex", 0);

	SetViewport(0, 0, 32, 32);
	BindFBO(ConvFBO);
	BindMesh(cube);

	for (unsigned int i = 0; i < 6; ++i)
	{
		BindMatrix4ToShader(captureViews[i], "viewMatrix");
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, ((OGLTexture*)irradianceMap)->GetObjectID(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DrawBoundMesh();
	}

	SetViewport(0, 0, currentWidth, currentHeight);
	BindFBO(nullptr);
}

void OGLRenderer::BindShader(ShaderBase*s) {
	if (!s) {
		glUseProgram(0);
		boundShader = nullptr;
	}
	else if (OGLShader* oglShader = dynamic_cast<OGLShader*>(s)) {
		glUseProgram(oglShader->programID);
		boundShader = oglShader;
	}
	else {
		std::cout << __FUNCTION__ << " has received invalid shader?!" << std::endl;
		boundShader = nullptr;
	}
}

void OGLRenderer::BindMesh(MeshGeometry*m) {
	if (!m) {
		glBindVertexArray(0);
		boundMesh = nullptr;
	}
	else if (OGLMesh* oglMesh = dynamic_cast<OGLMesh*>(m)) {
		if (!oglMesh->OnGPU()) {
			oglMesh->UploadToGPU();
		}
		if (oglMesh->GetVAO() == 0) {
			std::cout << __FUNCTION__ << " has received invalid mesh?!" << std::endl;
		}
		glBindVertexArray(oglMesh->GetVAO());
		boundMesh = oglMesh;
	}
	else {
		std::cout << __FUNCTION__ << " has received invalid mesh?!" << std::endl;
		boundMesh = nullptr;
	}
}

void OGLRenderer::DrawBoundMesh(int subLayer, int numInstances) {
	if (!boundMesh) {
		std::cout << __FUNCTION__ << " has been called without a bound mesh!" << std::endl;
		return;
	}
	if (!boundShader) {
		std::cout << __FUNCTION__ << " has been called without a bound shader!" << std::endl;
		return;
	}
	GLuint mode = 0;

	switch (boundMesh->GetPrimitiveType()) {
		case GeometryPrimitive::Triangles:		mode = GL_TRIANGLES;		break;
		case GeometryPrimitive::Points:			mode = GL_POINTS;			break;
		case GeometryPrimitive::Lines:			mode = GL_LINES;			break;
		case GeometryPrimitive::TriangleFan:	mode = GL_TRIANGLE_FAN;		break;
		case GeometryPrimitive::TriangleStrip:	mode = GL_TRIANGLE_STRIP;	break;
		case GeometryPrimitive::Patches:		mode = GL_PATCHES;			break;
	}

	if (boundMesh->GetIndexCount() > 0) {
		glDrawElements(mode, boundMesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(mode, 0, boundMesh->GetVertexCount());
	}
}

void OGLRenderer::BindTextureToShader(const TextureBase*t, const std::string& uniform, int texUnit) const{
	GLint texID = 0;

	if (!boundShader) {
		std::cout << __FUNCTION__ << " has been called without a bound shader!" << std::endl;
		return;//Debug message time!
	}
	
	GLuint slot = glGetUniformLocation(boundShader->programID, uniform.c_str());

	if (slot < 0) {
		return;
	}

	if (const OGLTexture* oglTexture = dynamic_cast<const OGLTexture*>(t)) {
		texID = oglTexture->texID;
	}

	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, texID);

	glUniform1i(slot, texUnit);
}

void OGLRenderer::BindTextureCubeToShader(const TextureBase*t, const std::string& uniform, int texUnit) const {
	GLint texID = 0;

	if (!boundShader) {
		std::cout << __FUNCTION__ << " has been called without a bound shader!" << std::endl;
		return;//Debug message time!
	}

	GLuint slot = glGetUniformLocation(boundShader->programID, uniform.c_str());

	if (slot < 0) {
		return;
	}

	if (const OGLTexture* oglTexture = dynamic_cast<const OGLTexture*>(t)) {
		texID = oglTexture->texID;
	}

	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	glUniform1i(slot, texUnit);
}

void OGLRenderer::BindBoolToShader(const bool val, const std::string& uniform) const {
	if (!boundShader) {
		std::cout << __FUNCTION__ << " has been called without a bound shader!" << std::endl;
		return;//Debug message time!
	}

	GLuint slot = glGetUniformLocation(boundShader->programID, uniform.c_str());

	if (slot < 0) {
		return;
	}

	glUniform1i(slot, val);
}

void OGLRenderer::BindIntToShader(const int val, const std::string& uniform) const {
	if (!boundShader) {
		std::cout << __FUNCTION__ << " has been called without a bound shader!" << std::endl;
		return;//Debug message time!
	}

	GLuint slot = glGetUniformLocation(boundShader->programID, uniform.c_str());

	if (slot < 0) {
		return;
	}

	glUniform1i(slot, val);
}

void OGLRenderer::BindFloatToShader(const float val, const std::string& uniform) const {
	if (!boundShader) {
		std::cout << __FUNCTION__ << " has been called without a bound shader!" << std::endl;
		return;//Debug message time!
	}

	GLuint slot = glGetUniformLocation(boundShader->programID, uniform.c_str());

	if (slot < 0) {
		return;
	}

	glUniform1f(slot, val);
}

void OGLRenderer::BindVector2ToShader(const Vector2& val, const std::string& uniform) const {
	if (!boundShader) {
		std::cout << __FUNCTION__ << " has been called without a bound shader!" << std::endl;
		return;//Debug message time!
	}

	GLuint slot = glGetUniformLocation(boundShader->programID, uniform.c_str());

	if (slot < 0) {
		return;
	}

	glUniform2fv(slot, 1, (float*)&val);
}

void OGLRenderer::BindVector3ToShader(const Vector3& val, const std::string& uniform) const {
	if (!boundShader) {
		std::cout << __FUNCTION__ << " has been called without a bound shader!" << std::endl;
		return;//Debug message time!
	}

	GLuint slot = glGetUniformLocation(boundShader->programID, uniform.c_str());

	if (slot < 0) {
		return;
	}

	glUniform3fv(slot, 1, (float*)&val);
}

void OGLRenderer::BindVector4ToShader(const Vector4& val, const std::string& uniform) const {
	if (!boundShader) {
		std::cout << __FUNCTION__ << " has been called without a bound shader!" << std::endl;
		return;//Debug message time!
	}

	GLuint slot = glGetUniformLocation(boundShader->programID, uniform.c_str());

	if (slot < 0) {
		return;
	}

	glUniform4fv(slot, 1, (float*)&val);
}

void OGLRenderer::BindMatrix4ToShader(const Matrix4& val, const std::string& uniform) const {
	if (!boundShader) {
		std::cout << __FUNCTION__ << " has been called without a bound shader!" << std::endl;
		return;//Debug message time!
	}

	GLuint slot = glGetUniformLocation(boundShader->programID, uniform.c_str());

	if (slot < 0) {
		return;
	}

	glUniformMatrix4fv(slot, 1, false, (float*)&val);
}

void OGLRenderer::GenerateFrameBuffer(void* buffer, std::vector<TextureBase*>& bufferTexs, TextureBase* depth) {
	glGenFramebuffers(1, (GLuint*)buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, *(GLuint*)(buffer));

	if (depth) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D, ((OGLTexture*)depth)->GetObjectID(), 0);
	}

	if (bufferTexs.size() != 0) {
		GLenum* drawBuffer = new GLenum[bufferTexs.size()];
		for (unsigned int i = 0; i < bufferTexs.size(); i++)
		{
			drawBuffer[i] = GL_COLOR_ATTACHMENT0 + i;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
				GL_TEXTURE_2D, ((OGLTexture*)bufferTexs[i])->GetObjectID(), 0);
		}
		glDrawBuffers((GLsizei) bufferTexs.size(), drawBuffer);
		delete[] drawBuffer;
	}
	else {
		glDrawBuffer(GL_NONE);
	}
}

void OGLRenderer::DeleteFrameBuffer(void* buffer) {
	glDeleteFramebuffers(1, (GLuint*)buffer);
}

void OGLRenderer::BindFBO(void* buffer) {
	if (!buffer) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, *(GLuint*)buffer);
	}
}

void OGLRenderer::ClearBuffer(bool depth, bool color, bool stencil) const {
	int bit = 0;
	if (depth) {
		bit = bit | GL_DEPTH_BUFFER_BIT;
	}
	if (color) {
		bit = bit | GL_COLOR_BUFFER_BIT;
	}
	if (stencil) {
		bit = bit | GL_STENCIL_BUFFER_BIT;
	}
	glClear(bit);
}

void OGLRenderer::SetViewport(int x, int y, int width, int height) const {
	glViewport(x, y, width, height);
}

//void OGLRenderer::DrawString(const std::string& text, const Vector2&pos, const Vector4& colour) {
//	DebugString s;
//	s.colour = colour;
//	s.ndcPos = (pos / Vector2((float)currentWidth, (float)currentHeight));
//	
//	s.ndcPos.x = (s.ndcPos.x * 2.0f) - 1.0f;
//	s.ndcPos.y = (s.ndcPos.y * 2.0f) - 1.0f;
//	s.size = 1.0f;
//	s.text = text;
//	debugStrings.emplace_back(s);
//}

void OGLRenderer::DrawString(const std::string& text, const Vector2&pos, const Vector4& colour, const float& size) {
	DebugString s;
	s.colour = colour;
	s.ndcPos = (pos / Vector2((float)currentWidth, (float)currentHeight));

	s.ndcPos.x = (s.ndcPos.x * 2.0f) - 1.0f;
	s.ndcPos.y = (s.ndcPos.y * 2.0f) - 1.0f;
	s.size = size;
	s.text = text;
	debugStrings.emplace_back(s);
}

void OGLRenderer::DrawLine(const Vector3& start, const Vector3& end, const Vector4& colour) {
	DebugLine l;
	l.start		= start;
	l.end		= end;
	l.colour	= colour;
	debugLines.emplace_back(l);
}

void OGLRenderer::DrawDebugData() {
	BindShader(debugShader);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int switchLocation = glGetUniformLocation(debugShader->GetProgramID(), "useMatrix");

	glUniform1i(switchLocation, 0);
	DrawDebugStrings();
	SetupDebugMatrix(debugShader);
	glUniform1i(switchLocation, 1);
	DrawDebugLines();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OGLRenderer::DrawDebugStrings() {
	vector<Vector3> vertPos;
	vector<Vector2> vertTex;
	vector<Vector4> vertColours;

	for (DebugString&s : debugStrings) {
		font->BuildVerticesForString(s.text, s.ndcPos, s.colour, vertPos, vertTex, vertColours, s.size);
	}

	OGLMesh textMesh = OGLMesh();

	textMesh.SetVertexPositions(vertPos);
	textMesh.SetVertexTextureCoords(vertTex);
	textMesh.SetVertexColours(vertColours);

	textMesh.UploadToGPU();

	BindMesh(&textMesh);
	BindTextureToShader(font->GetTexture(), "mainTex", 0);
	DrawBoundMesh();

	debugStrings.clear();
}

void OGLRenderer::DrawDebugLines() {
	vector<Vector3> vertPos;
	vector<Vector4> vertCol;

	for (DebugLine&s : debugLines) {
		vertPos.emplace_back(s.start);
		vertPos.emplace_back(s.end);

		vertCol.emplace_back(s.colour);
		vertCol.emplace_back(s.colour);
	}

	OGLMesh lineMesh = OGLMesh();

	lineMesh.SetVertexPositions(vertPos);
	lineMesh.SetVertexColours(vertCol);
	lineMesh.SetPrimitiveType(GeometryPrimitive::Lines);

	lineMesh.UploadToGPU();

	BindMesh(&lineMesh);
	BindTextureToShader(nullptr, "mainTex", 0);
	DrawBoundMesh();

	debugLines.clear();
}

#ifdef _WIN32
void OGLRenderer::InitWithWin32(Window& w) {
	Win32Code::Win32Window* realWindow = (Win32Code::Win32Window*)&w;

	if (!(deviceContext = GetDC(realWindow->GetHandle()))) {
		std::cout << __FUNCTION__ << " Failed to create window!" << std::endl;
		return;
	}

	//A pixel format descriptor is a struct that tells the Windows OS what type of front / back buffers we want to create etc
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion	= 1;
	pfd.dwFlags		= PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   //It must be double buffered, it must support OGL(!), and it must allow us to draw to it...
	pfd.iPixelType	= PFD_TYPE_RGBA;	//We want our front / back buffer to have 4 channels!
	pfd.cColorBits	= 32;				//4 channels of 8 bits each!
	pfd.cDepthBits	= 24;				//24 bit depth buffer
	pfd.cStencilBits = 8;				//plus an 8 bit stencil buffer
	pfd.iLayerType	= PFD_MAIN_PLANE;

	GLuint		PixelFormat;
	if (!(PixelFormat = ChoosePixelFormat(deviceContext, &pfd))) {	// Did Windows Find A Matching Pixel Format for our PFD?
		std::cout << __FUNCTION__ << " Failed to choose a pixel format!" << std::endl;
		return;
	}

	if (!SetPixelFormat(deviceContext, PixelFormat, &pfd)) {		// Are We Able To Set The Pixel Format?
		std::cout << __FUNCTION__ << " Failed to set a pixel format!" << std::endl;
		return;
	}

	HGLRC		tempContext;		//We need a temporary OpenGL context to check for OpenGL 3.2 compatibility...stupid!!!
	if (!(tempContext = wglCreateContext(deviceContext))) {	// Are We Able To get the temporary context?
		std::cout << __FUNCTION__ <<"  Cannot create a temporary context!" << std::endl;
		wglDeleteContext(tempContext);
		return;
	}

	if (!wglMakeCurrent(deviceContext, tempContext)) {	// Try To Activate The Rendering Context
		std::cout << __FUNCTION__ << " Cannot set temporary context!" << std::endl;
		wglDeleteContext(tempContext);
		return;
	}
	if (!gladLoadGL()) {
		std::cout << __FUNCTION__ << " Cannot initialise GLAD!" << std::endl;	//It's all gone wrong!
		return;
	}
	//Now we have a temporary context, we can find out if we support OGL 3.x
	char* ver = (char*)glGetString(GL_VERSION); // ver must equal "3.2.0" (or greater!)
	int major = ver[0] - '0';		//casts the 'correct' major version integer from our version string
	int minor = ver[2] - '0';		//casts the 'correct' minor version integer from our version string

	if (major < 3) {					//Graphics hardware does not support OGL 3! Erk...
		std::cout << __FUNCTION__ << " Device does not support OpenGL 3.x!" << std::endl;
		wglDeleteContext(tempContext);
		return;
	}

	if (major == 3 && minor < 2) {	//Graphics hardware does not support ENOUGH of OGL 3! Erk...
		std::cout << __FUNCTION__ << " Device does not support OpenGL 3.2!" << std::endl;
		wglDeleteContext(tempContext);
		return;
	}
	//We do support OGL 3! Let's set it up...

	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major,	//TODO: Maybe lock this to 3? We might actually get an OpenGL 4.x context...
		WGL_CONTEXT_MINOR_VERSION_ARB, minor,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#ifdef OPENGL_DEBUGGING 
		| WGL_CONTEXT_DEBUG_BIT_ARB
#endif		//No deprecated stuff!! DIE DIE DIE glBegin!!!!
		,WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,		//We want everything OpenGL 3.2 provides...
		0					//That's enough attributes...
	};

	//Everywhere else in the Renderers, we use function pointers provided by GLEW...but we can't initialise GLEW yet! So we have to use the 'Wiggle' API
	//to get a pointer to the function that will create our OpenGL 3.2 context...
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	renderContext = wglCreateContextAttribsARB(deviceContext, 0, attribs);

	// Check for the context, and try to make it the current rendering context
	if (!renderContext || !wglMakeCurrent(deviceContext, renderContext)) {
		std::cout << __FUNCTION__ <<" Cannot set OpenGL 3 context!" << std::endl;	//It's all gone wrong!
		wglDeleteContext(renderContext);
		wglDeleteContext(tempContext);
		return;
	}

	wglDeleteContext(tempContext);	//We don't need the temporary context any more!

	std::cout << __FUNCTION__ << " Initialised OpenGL " << major << "." << minor << " rendering context" << std::endl;	//It's all gone wrong!

	glEnable(GL_FRAMEBUFFER_SRGB);
	//If we get this far, everything's going well!
}

void OGLRenderer::DestroyWithWin32() {
	wglDeleteContext(renderContext);
}
#endif