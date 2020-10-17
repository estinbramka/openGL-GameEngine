#include "Texture.h"

Texture::Texture(std::string _filename, GLenum _textureTarget)
{
	filename = _filename;
	textureTarget = _textureTarget;
}

bool Texture::Load()
{
	glGenTextures(1, &textureObject);
	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(textureTarget, textureObject);
		glTexImage2D(textureTarget, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(textureTarget);

		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture " << filename << std::endl;
		stbi_image_free(data);
		return false;
	}
	stbi_image_free(data);
	return true;
}

void Texture::Bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(textureTarget, textureObject);
}
