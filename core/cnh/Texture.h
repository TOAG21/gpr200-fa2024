#ifndef TEXTURE_H
#define TEXTURE_H

#include "../ew/external/glad.h"
#include "../ew/external/stb_image.h"

namespace cnh
{
	class Texture
	{
	public:
		Texture(const char* filepath, int filterMode, int wrapMode, int channels);
		~Texture();
		void Bind(unsigned int slot = 0);
	private:
		unsigned int mID;
		int mWidth, mHeight;
	};
}

#endif