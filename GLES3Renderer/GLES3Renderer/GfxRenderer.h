#pragma once
#include <map>
#include "glew.h"
#include "GfxMesh.h"
#include "GfxMaterial.h"
#include "GfxFrameBuffer.h"
#include "GfxUniformTime.h"
#include "GfxUniformScreen.h"
#include "GfxUniformZBuffer.h"
#include "GfxUniformProjection.h"
#include "GfxUniformCamera.h"
#include "GfxUniformFog.h"
#include "GfxUniformTransform.h"
#include "GfxUniformAmbientLight.h"
#include "GfxUniformPointLight.h"
#include "GfxUniformDirectLight.h"
#include "GfxUniformShadowLight.h"


#define ENGINE_TIME_NAME                "Time"
#define ENGINE_SCREEN_NAME              "Screen"
#define ENGINE_ZBUFFER_NAME             "ZBuffer"
#define ENGINE_PROJECTION_NAME          "Projection"
#define ENGINE_CAMERA_NAME              "Camera"
#define ENGINE_TRANSFORM_NAME           "Transform"
#define ENGINE_FOG_NAME                 "Fog"
#define ENGINE_AMBIENT_LIGHT_NAME       "AmbientLight"
#define ENGINE_POINT_LIGHT_NAME         "PointLight"
#define ENGINE_DIRECT_LIGHT_NAME        "DirectLight"
#define ENGINE_SHADOW_LIGHT_NAME        "ShadowLight"


class CGfxRenderer
{
public:
	static CGfxRenderer* GetInstance(void);
	static void Create(const char *szShaderPath, const char *szTexturePath, const char *szMaterialPath);
	static void Destroy(void);


private:
	CGfxRenderer(const char *szShaderPath, const char *szTexturePath, const char *szMaterialPath);
	virtual ~CGfxRenderer(void);


public:
	const char* GetShaderFullPath(const char *szFileName, char *szFullPath) const;
	const char* GetTextureFullPath(const char *szFileName, char *szFullPath) const;
	const char* GetMaterialFullPath(const char *szFileName, char *szFullPath) const;

public:
	void SetScissor(int x, int y, int width, int height);
	void SetViewport(int x, int y, int width, int height);

	void SetFrameBuffer(GLuint fbo);
	void SetInputTexture(const char *szName, GLuint texture);

public:
	void SetTime(float t, float dt);

public:
	void SetCameraPerspective(float fovy, float aspect, float zNear, float zFar);
	void SetCameraOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetCameraLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);
	void SetCameraProjectionMatrix(const float *mtxProjection);
	void SetCameraViewMatrix(const float *mtxView);

public:
	void SetFogColor(float r, float g, float b);
	void SetFogHeightDensity(float startHeight, float endHeight, float density);
	void SetFogDistanceDensity(float startDistance, float endDistance, float density);

public:
	void SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9]);
	void SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz);

	void SetPointLightColor(float red, float green, float blue);
	void SetPointLightPosition(float posx, float posy, float posz);
	void SetPointLightAttenuation(float linear, float square, float constant);

	void SetDirectLightColor(float red, float green, float blue);
	void SetDirectLightDirection(float dirx, float diry, float dirz);

public:
	void SetShadowLightOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetShadowLightLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

public:
	bool LoadMaterial(const char *szFileName, GLuint materialid);
	CGfxMaterial* GetMaterial(GLuint id) const;

public:
	void Clear(float red, float green, float blue, float alpha, float depth);
	void DrawInstance(GLuint material, CGfxMesh *pMesh);
	void DrawElements(GLuint material, CGfxMesh *pMesh, const CGfxUniformTransform *pUniformTransform);
	void DrawScreen(GLuint material);

private:
	void BindMaterial(CGfxMaterial *pMaterial);


private:
	char m_szShaderPath[260];
	char m_szTexturePath[260];
	char m_szMaterialPath[260];

private:
	CGfxMesh m_meshScreen;

private:
	CGfxUniformTime m_uniformTime;
	CGfxUniformScreen m_uniformScreen;
	CGfxUniformZBuffer m_uniformZBuffer;
	CGfxUniformProjection m_uniformProjection;
	CGfxUniformCamera m_uniformCamera;
	CGfxUniformFog m_uniformFog;
	CGfxUniformAmbientLight m_uniformAmbientLight;
	CGfxUniformPointLight m_uniformPointLight;
	CGfxUniformDirectLight m_uniformDirectLight;
	CGfxUniformShadowLight m_uniformShadowLight;

private:
	GLuint m_material;
	std::map<GLuint, CGfxMaterial*> m_pMaterials;

private:
	CGfxMaterial *m_pGlobalMaterial;

private:
	static CGfxRenderer *pInstance;
};