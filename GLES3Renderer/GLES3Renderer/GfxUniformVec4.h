#pragma once
#include "glew.h"
#include "glm/glm.hpp"
#include "GfxUniformBuffer.h"


class CGfxUniformVec4
{
public:
	CGfxUniformVec4(void);
	virtual ~CGfxUniformVec4(void);


public:
	void SetValue(float x, float y, float z, float w);
	void Apply(void);

public:
	GLuint GetSize(void) const;
	GLuint GetBuffer(void) const;


private:
	bool m_bDirty;
	glm::vec4 m_value;
	CGfxUniformBuffer m_uniformBuffer;
};
