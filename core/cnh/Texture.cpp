#include "Texture.h"

cnh::Texture::Texture(const char* filepath, int filterMode, int wrapMode, int channels) {
	int nrChannels;
	unsigned char* data = stbi_load(filepath, &mWidth, &mHeight, &nrChannels, channels);

	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

	if(channels == 3 || channels == 0){glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);	}
	else if (channels == 4) {	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	}
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

cnh::Texture::~Texture() {}

void cnh::Texture::Bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, mID);
}