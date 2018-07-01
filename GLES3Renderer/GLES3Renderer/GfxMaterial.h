#pragma once
#include <map>
#include <vector>
#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"
#include "GfxProgram.h"
#include "GfxTexture2D.h"
#include "GfxTexture2DArray.h"
#include "GfxTextureCubeMap.h"
#include "GfxUniformVec1.h"
#include "GfxUniformVec2.h"
#include "GfxUniformVec3.h"
#include "GfxUniformVec4.h"
#include "GfxUniformMat4.h"


class CGfxMaterial
{
	friend class CGfxRenderer;


public:
	CGfxMaterial(void);
	virtual ~CGfxMaterial(void);


public:
	void Bind(void) const;

private:
	void BindPipeline(void) const;
	void BindUniforms(CGfxProgram *pProgram) const;
	void BindTextures(CGfxProgram *pProgram, GLuint indexUnit) const;

public:
	bool Create(const char *szFileName);
	void Destroy(void);

private:
	bool Load(const char *szFileName);
	bool LoadBase(TiXmlNode *pMaterialNode);
	bool LoadProgram(TiXmlNode *pMaterialNode);
	bool LoadTexture2D(TiXmlNode *pMaterialNode);
	bool LoadTexture2DArray(TiXmlNode *pMaterialNode);
	bool LoadTextureCubeMap(TiXmlNode *pMaterialNode);
	bool LoadUniformVec1(TiXmlNode *pMaterialNode);
	bool LoadUniformVec2(TiXmlNode *pMaterialNode);
	bool LoadUniformVec3(TiXmlNode *pMaterialNode);
	bool LoadUniformVec4(TiXmlNode *pMaterialNode);

public:
	void SetEnableCullFace(bool bEnable, GLenum frontFace);
	void SetEnableDepthTest(bool bEnable, GLenum depthFunc);
	void SetEnableDepthWrite(bool bEnable);
	void SetEnableBlend(bool bEnable, GLenum srcFactor, GLenum dstFactor);
	void SetEnablePolygonOffset(bool bEnable, GLfloat factor, GLfloat units);
	void SetEnableDepthMask(bool bEnable);
	void SetEnableColorMask(bool bEnableRed, bool bEnableGreen, bool bEnableBlue, bool bEnableAlpha);

public:
	CGfxProgram* GetProgram(void);

	CGfxTexture2D* GetTexture2D(const char *szName);
	CGfxTexture2DArray* GetTexture2DArray(const char *szName);
	CGfxTextureCubeMap* GetTextureCubeMap(const char *szName);

	CGfxUniformVec1* GetUniformVec1(const char *szName);
	CGfxUniformVec2* GetUniformVec2(const char *szName);
	CGfxUniformVec3* GetUniformVec3(const char *szName);
	CGfxUniformVec4* GetUniformVec4(const char *szName);
	CGfxUniformMat4* GetUniformMat4(const char *szName);

	GLuint GetTextureUnits(void) const;


private:
	bool m_bEnableCullFace;
	bool m_bEnableDepthTest;
	bool m_bEnableDepthWrite;
	bool m_bEnableBlend;
	bool m_bEnablePolygonOffset;
	bool m_bEnableDepthMask;
	bool m_bEnableColorMaskRed;
	bool m_bEnableColorMaskGreen;
	bool m_bEnableColorMaskBlue;
	bool m_bEnableColorMaskAlpha;
	GLenum m_frontFace;
	GLenum m_depthFunc;
	GLenum m_srcBlendFactor;
	GLenum m_dstBlendFactor;
	GLfloat m_polygonOffsetFactor;
	GLfloat m_polygonOffsetUnits;

private:
	std::map<GLuint, CGfxTexture2D*> m_pTexture2ds;
	std::map<GLuint, CGfxTexture2DArray*> m_pTexture2dArrays;
	std::map<GLuint, CGfxTextureCubeMap*> m_pTextureCubeMaps;

	std::map<GLuint, CGfxUniformVec1*> m_pUniformVec1s;
	std::map<GLuint, CGfxUniformVec2*> m_pUniformVec2s;
	std::map<GLuint, CGfxUniformVec3*> m_pUniformVec3s;
	std::map<GLuint, CGfxUniformVec4*> m_pUniformVec4s;
	std::map<GLuint, CGfxUniformMat4*> m_pUniformMat4s;

private:
	CGfxProgram *m_pProgram;
};