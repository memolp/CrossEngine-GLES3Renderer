#include "stdio.h"
#include "stdlib.h"
#include "GfxRenderer.h"


CGfxRenderer* CGfxRenderer::pInstance = NULL;
CGfxRenderer* CGfxRenderer::GetInstance(void)
{
	return pInstance;
}

void CGfxRenderer::Create(const char *szShaderPath, const char *szTexturePath, const char *szMaterialPath)
{
	if (pInstance == NULL) {
		pInstance = new CGfxRenderer(szShaderPath, szTexturePath, szMaterialPath);
	}
}

void CGfxRenderer::Destroy(void)
{
	if (pInstance) {
		delete pInstance;
	}

	pInstance = NULL;
}

CGfxRenderer::CGfxRenderer(const char *szShaderPath, const char *szTexturePath, const char *szMaterialPath)
	: m_pGlobalMaterial(NULL)
{
	strcpy(m_szShaderPath, szShaderPath);
	strcpy(m_szTexturePath, szTexturePath);
	strcpy(m_szMaterialPath, szMaterialPath);

	struct Vertex {
		glm::vec3 position;
		glm::vec2 texcoord;
	};

	const static Vertex vertices[] = {
		{ glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
	};

	const static GLuint indices[] = {
		0, 1, 2, 2, 3, 0
	};

	m_meshScreen.CreateIndexBuffer(sizeof(indices), indices, false, GL_UNSIGNED_INT);
	m_meshScreen.CreateVertexBuffer(sizeof(vertices), vertices, false, VERTEX_ATTRIBUTE_POSITION | VERTEX_ATTRIBUTE_TEXCOORD0);

	m_material = -1;
	m_pGlobalMaterial = new CGfxMaterial;
}

CGfxRenderer::~CGfxRenderer(void)
{
	m_meshScreen.Destroy();
	delete m_pGlobalMaterial;
}

const char* CGfxRenderer::GetShaderFullPath(const char *szFileName, char *szFullPath) const
{
	sprintf(szFullPath, "%s/%s", m_szShaderPath, szFileName);
	return szFullPath;
}

const char* CGfxRenderer::GetTextureFullPath(const char *szFileName, char *szFullPath) const
{
	sprintf(szFullPath, "%s/%s", m_szTexturePath, szFileName);
	return szFullPath;
}

const char* CGfxRenderer::GetMaterialFullPath(const char *szFileName, char *szFullPath) const
{
	sprintf(szFullPath, "%s/%s", m_szMaterialPath, szFileName);
	return szFullPath;
}

void CGfxRenderer::SetScissor(int x, int y, int width, int height)
{
	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, width, height);
}

void CGfxRenderer::SetViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
	m_uniformScreen.SetScreen(1.0f * width, 1.0f * height);
}

void CGfxRenderer::SetFrameBuffer(GLuint fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void CGfxRenderer::SetInputTexture(const char *szName, GLuint texture)
{
	m_pGlobalMaterial->GetTexture2D(szName)->Create(texture, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
}

void CGfxRenderer::SetTime(float t, float dt)
{
	m_uniformTime.SetTime(t, dt);
}

void CGfxRenderer::SetCameraPerspective(float fovy, float aspect, float zNear, float zFar)
{
	m_uniformCamera.SetPerspective(fovy, aspect, zNear, zFar);
	m_uniformZBuffer.SetZBuffer(zNear, zFar);
	m_uniformProjection.SetProjection(zNear, zFar);
}

void CGfxRenderer::SetCameraOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_uniformCamera.SetOrtho(left, right, bottom, top, zNear, zFar);
	m_uniformZBuffer.SetZBuffer(zNear, zFar);
	m_uniformProjection.SetProjection(zNear, zFar);
}

void CGfxRenderer::SetCameraLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_uniformCamera.SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CGfxRenderer::SetCameraProjectionMatrix(const float *mtxProjection)
{
	m_uniformCamera.SetProjectionMatrix(mtxProjection);
}

void CGfxRenderer::SetCameraViewMatrix(const float *mtxView)
{
	m_uniformCamera.SetViewMatrix(mtxView);
}

void CGfxRenderer::SetFogColor(float r, float g, float b)
{
	m_uniformFog.SetColor(r, g, b);
}

void CGfxRenderer::SetFogHeightDensity(float startHeight, float endHeight, float density)
{
	m_uniformFog.SetHeightDensity(startHeight, endHeight, density);
}

void CGfxRenderer::SetFogDistanceDensity(float startDistance, float endDistance, float density)
{
	m_uniformFog.SetDistanceDensity(startDistance, endDistance, density);
}

void CGfxRenderer::SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
{
	m_uniformAmbientLight.SetSH(shRed, shGreen, shBlue);
}

void CGfxRenderer::SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
{
	m_uniformAmbientLight.SetRotation(angle, axisx, axisy, axisz);
}

void CGfxRenderer::SetPointLightColor(float red, float green, float blue)
{
	m_uniformPointLight.SetColor(red, green, blue);
}

void CGfxRenderer::SetPointLightPosition(float posx, float posy, float posz)
{
	m_uniformPointLight.SetPosition(posx, posy, posz);
}

void CGfxRenderer::SetPointLightAttenuation(float linear, float square, float constant)
{
	m_uniformPointLight.SetAttenuation(linear, square, constant);
}

void CGfxRenderer::SetDirectLightColor(float red, float green, float blue)
{
	m_uniformDirectLight.SetColor(red, green, blue);
}

void CGfxRenderer::SetDirectLightDirection(float dirx, float diry, float dirz)
{
	m_uniformDirectLight.SetDirection(-dirx, -diry, -dirz);
}

void CGfxRenderer::SetShadowLightOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_uniformShadowLight.SetOrtho(left, right, bottom, top, zNear, zFar);
}

void CGfxRenderer::SetShadowLightLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_uniformShadowLight.SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

bool CGfxRenderer::LoadMaterial(const char *szFileName, GLuint materialid)
{
	CGfxMaterial *pMaterial = NULL;

	try {
		int err = 0;

		if (m_pMaterials.find(materialid) != m_pMaterials.end()) {
			throw err++;
		}

		if ((pMaterial = new CGfxMaterial) == NULL) {
			throw err++;
		}

		if (pMaterial->Create(szFileName) == false) {
			throw err++;
		}

		m_pMaterials[materialid] = pMaterial;

		return true;
	}
	catch (int) {
		if (pMaterial) {
			delete pMaterial;
		}

		return false;
	}
}

CGfxMaterial* CGfxRenderer::GetMaterial(GLuint id) const
{
	const auto &itMaterial = m_pMaterials.find(id);
	return itMaterial != m_pMaterials.end() ? itMaterial->second : m_pGlobalMaterial;
}

void CGfxRenderer::Clear(float red, float green, float blue, float alpha, float depth)
{
	glClearColor(red, green, blue, alpha);
	glClearDepthf(depth);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CGfxRenderer::DrawInstance(GLuint material, CGfxMesh *pMesh)
{
	if (m_pMaterials.find(material) == m_pMaterials.end()) {
		return;
	}

	if (m_material != material) {
		m_material  = material;
		BindMaterial(m_pMaterials[material]);
	}

	pMesh->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, pMesh->GetIndexCount(), pMesh->GetIndexType(), NULL, pMesh->GetInstanceCount());
}

void CGfxRenderer::DrawElements(GLuint material, CGfxMesh *pMesh, const CGfxUniformTransform *pUniformTransform)
{
	if (m_pMaterials.find(material) == m_pMaterials.end()) {
		return;
	}

	if (m_material != material) {
		m_material  = material;
		BindMaterial(m_pMaterials[material]);
	}

	m_pMaterials[material]->GetProgram()->BindUniformBuffer(HashValue(ENGINE_TRANSFORM_NAME), pUniformTransform->GetBuffer(), pUniformTransform->GetSize());

	pMesh->Bind();
	glDrawElements(GL_TRIANGLES, pMesh->GetIndexCount(), pMesh->GetIndexType(), NULL);
}

void CGfxRenderer::DrawScreen(GLuint material)
{
	if (m_pMaterials.find(material) == m_pMaterials.end()) {
		return;
	}

	if (m_material != material) {
		m_material  = material;
		BindMaterial(m_pMaterials[material]);
	}

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	m_meshScreen.Bind();
	glDrawElements(GL_TRIANGLES, m_meshScreen.GetIndexCount(), m_meshScreen.GetIndexType(), NULL);
}

void CGfxRenderer::BindMaterial(CGfxMaterial *pMaterial)
{
	m_uniformTime.Apply();
	m_uniformScreen.Apply();
	m_uniformZBuffer.Apply();
	m_uniformProjection.Apply();
	m_uniformCamera.Apply();
	m_uniformFog.Apply();
	m_uniformAmbientLight.Apply();
	m_uniformPointLight.Apply();
	m_uniformDirectLight.Apply();
	m_uniformShadowLight.Apply();

	pMaterial->Bind();
	pMaterial->GetProgram()->BindUniformBuffer(HashValue(ENGINE_TIME_NAME), m_uniformTime.GetBuffer(), m_uniformTime.GetSize());
	pMaterial->GetProgram()->BindUniformBuffer(HashValue(ENGINE_SCREEN_NAME), m_uniformScreen.GetBuffer(), m_uniformScreen.GetSize());
	pMaterial->GetProgram()->BindUniformBuffer(HashValue(ENGINE_ZBUFFER_NAME), m_uniformZBuffer.GetBuffer(), m_uniformZBuffer.GetSize());
	pMaterial->GetProgram()->BindUniformBuffer(HashValue(ENGINE_PROJECTION_NAME), m_uniformProjection.GetBuffer(), m_uniformProjection.GetSize());
	pMaterial->GetProgram()->BindUniformBuffer(HashValue(ENGINE_CAMERA_NAME), m_uniformCamera.GetBuffer(), m_uniformCamera.GetSize());
	pMaterial->GetProgram()->BindUniformBuffer(HashValue(ENGINE_FOG_NAME), m_uniformFog.GetBuffer(), m_uniformFog.GetSize());
	pMaterial->GetProgram()->BindUniformBuffer(HashValue(ENGINE_AMBIENT_LIGHT_NAME), m_uniformAmbientLight.GetBuffer(), m_uniformAmbientLight.GetSize());
	pMaterial->GetProgram()->BindUniformBuffer(HashValue(ENGINE_POINT_LIGHT_NAME), m_uniformPointLight.GetBuffer(), m_uniformPointLight.GetSize());
	pMaterial->GetProgram()->BindUniformBuffer(HashValue(ENGINE_DIRECT_LIGHT_NAME), m_uniformDirectLight.GetBuffer(), m_uniformDirectLight.GetSize());
	pMaterial->GetProgram()->BindUniformBuffer(HashValue(ENGINE_SHADOW_LIGHT_NAME), m_uniformShadowLight.GetBuffer(), m_uniformShadowLight.GetSize());

	m_pGlobalMaterial->BindUniforms(pMaterial->GetProgram());
	m_pGlobalMaterial->BindTextures(pMaterial->GetProgram(), pMaterial->GetTextureUnits());
}