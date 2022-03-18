#include "Texture2D.h"
#include "OpenGLInclude.h"
#include <stbImage/stb_image.h>
#include "Defines.h"

using namespace OpenGLDemos::Common;

Texture2D::Texture2D(const std::string &path, unsigned int type)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	stbi_set_flip_vertically_on_load(true);
	unsigned char *imageData = stbi_load(path.c_str(), &m_width, &m_height, &m_channel, 0);
	if (imageData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, type, m_width, m_height, 0, type, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(imageData);
	}
	else
	{
		OD_PRINT_INFO("load texture failed : %s", path.c_str());
		OD_Assert(false);
	}
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_texture);
}

void Texture2D::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture2D::activeAndBind(int i)
{
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}