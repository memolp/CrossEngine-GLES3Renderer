#include "stdio.h"
#include "stdlib.h"
#include "GfxTextureBase.h"


CGfxTextureBase::CGfxTextureBase(GLuint name)
	: m_name(name)

	, m_texture(0)
	, m_extern(GL_FALSE)

	, m_format(GL_INVALID_ENUM)
	, m_internalFormat(GL_INVALID_ENUM)

	, m_width(0)
	, m_height(0)

	, m_mipLevels(0)
	, m_arrayLayers(0)

	, refCount(0)
{

}

CGfxTextureBase::~CGfxTextureBase(void)
{
	Destroy();
}

bool CGfxTextureBase::Create(GLuint texture)
{
	Destroy();

	m_texture = texture;
	m_extern = GL_TRUE;

	return true;
}

void CGfxTextureBase::Destroy(void)
{
	if (m_extern == GL_FALSE) {
		if (m_texture) {
			glDeleteTextures(1, &m_texture);
		}
	}

	m_texture = 0;
	m_extern = GL_FALSE;

	m_format = GL_INVALID_ENUM;
	m_internalFormat = GL_INVALID_ENUM;

	m_width = 0;
	m_height = 0;

	m_mipLevels = 0;
	m_arrayLayers = 0;
}

GLuint CGfxTextureBase::GetName(void) const
{
	return m_name;
}

GLuint CGfxTextureBase::GetTexture(void) const
{
	return m_texture;
}

GLenum CGfxTextureBase::GetFormat(void) const
{
	return m_format;
}

GLenum CGfxTextureBase::GetInternalFormat(void) const
{
	return m_internalFormat;
}

GLuint CGfxTextureBase::GetWidth(void) const
{
	return m_width;
}

GLuint CGfxTextureBase::GetHeight(void) const
{
	return m_height;
}
