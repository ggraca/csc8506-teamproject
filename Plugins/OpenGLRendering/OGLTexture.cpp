#include "OGLTexture.h"
#include "OGLRenderer.h"

#include "../../Common/TextureLoader.h"

using namespace NCL;
using namespace NCL::Rendering;

OGLTexture::OGLTexture()
{
	glGenTextures(1, &texID);
}

OGLTexture::~OGLTexture()
{
	glDeleteTextures(1, &texID);
}

TextureBase* OGLTexture::RGBATextureFromData(char* data, int width, int height, int channels) {
	OGLTexture* tex = new OGLTexture();

	int dataSize = width * height * channels; //This always assumes data is 1 byte per channel

	int sourceType = GL_RGB;

	switch (channels) {
		case 1: sourceType = GL_RED	; break;

		case 2: sourceType = GL_RG	; break;
		case 3: sourceType = GL_RGB	; break;
		case 4: sourceType = GL_RGBA; break;
		//default:
	}

	glBindTexture(GL_TEXTURE_2D, tex->texID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, sourceType, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

TextureBase* OGLTexture::CubeTextureFromData(char** data, int width, int height, int channels) {
	OGLTexture* tex = new OGLTexture();

	glBindTexture(GL_TEXTURE_CUBE_MAP, tex->texID);

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data[i]);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

TextureBase* OGLTexture::RGBATextureFromFilename(const std::string&name) {
	char* texData	= nullptr;
	int width		= 0;
	int height		= 0;
	int channels	= 0;
	int flags		= 0;
	TextureLoader::LoadTexture(name, texData, width, height, channels, flags);

	TextureBase* glTex = RGBATextureFromData(texData, width, height, channels);

	free(texData);

	return glTex;
}

TextureBase* OGLTexture::EmptyTexture(int width, int height, bool depth) {
	OGLTexture* tex = new OGLTexture();

	glBindTexture(GL_TEXTURE_2D, tex->texID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, depth ? GL_DEPTH_COMPONENT : GL_RGBA32F, width, height, 0,
		depth ? GL_DEPTH_COMPONENT : GL_RGBA, depth ? GL_FLOAT : GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

TextureBase* OGLTexture::ShadowTexture(int width, int height) {
	OGLTexture* tex = new OGLTexture();

	glBindTexture(GL_TEXTURE_2D, tex->texID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

TextureBase* OGLTexture::CubeTextureFromFilename(const std::vector<std::string>& faces) {
	char* texData[6];
	int width = 0;
	int height = 0;
	int channels = 0;
	int flags = 0;
	TextureLoader::LoadTexture(faces[0], texData[0], width, height, channels, flags);
	TextureLoader::LoadTexture(faces[1], texData[1], width, height, channels, flags);
	TextureLoader::LoadTexture(faces[2], texData[2], width, height, channels, flags);
	TextureLoader::LoadTexture(faces[3], texData[3], width, height, channels, flags);
	TextureLoader::LoadTexture(faces[4], texData[4], width, height, channels, flags);
	TextureLoader::LoadTexture(faces[5], texData[5], width, height, channels, flags);

	TextureBase* glTex = CubeTextureFromData(texData, width, height, channels);

	for (int i = 0; i < 6; i++)
	{
		free(texData[i]);
	}

	return glTex;
}