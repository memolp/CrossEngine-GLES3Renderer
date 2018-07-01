#include "stdio.h"
#include "stdlib.h"
#include "GfxIndexBuffer.h"


CGfxIndexBuffer::CGfxIndexBuffer(void)
	: m_indexType(GL_UNSIGNED_SHORT)
	, m_indexCount(0)
	, m_indexBuffer(0)
{

}

CGfxIndexBuffer::~CGfxIndexBuffer(void)
{
	Destroy();
}

bool CGfxIndexBuffer::CreateIndexBuffer(size_t size, const void *pBuffer, bool bDynamic, GLenum type)
{
	m_indexType = type;
	m_indexCount = size / (m_indexType == GL_UNSIGNED_SHORT ? 2 : 4);

	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, pBuffer, bDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

void CGfxIndexBuffer::Destroy(void)
{
	if (m_indexBuffer) {
		glDeleteBuffers(1, &m_indexBuffer);
	}

	m_indexBuffer = 0;
}

GLenum CGfxIndexBuffer::GetIndexType(void) const
{
	return m_indexType;
}

GLuint CGfxIndexBuffer::GetIndexCount(void) const
{
	return m_indexCount;
}

GLuint CGfxIndexBuffer::GetIndexBuffer(void) const
{
	return m_indexBuffer;
}