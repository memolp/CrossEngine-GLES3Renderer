#include "stdio.h"
#include "stdlib.h"
#include "GfxCamera.h"
#include "GfxRenderer.h"


CGfxCamera::CGfxCamera(void)
	: m_pFrameBuffer(NULL)
	, m_pCommandBuffer(NULL)

	, m_bEnableClearDepth(true)
	, m_bEnableClearColor(true)
	, m_clearDepth(1.0f)
	, m_clearColorRed(0.0f)
	, m_clearColorGreen(0.0f)
	, m_clearColorBlue(0.0f)
	, m_clearColorAlpha(0.0f)
{
	m_pCommandBuffer = new CGfxCommandBuffer(true);
}

CGfxCamera::~CGfxCamera(void)
{
	ClearQueue();
	delete m_pCommandBuffer;
}

void CGfxCamera::SetFrameBuffer(CGfxFrameBuffer *pFrameBuffer)
{
	m_pFrameBuffer = pFrameBuffer;
}

const CGfxFrameBuffer* CGfxCamera::GetFrameBuffer(void) const
{
	return m_pFrameBuffer;
}

void CGfxCamera::SetEnableClearDepth(bool bEnable)
{
	m_bEnableClearDepth = bEnable;
}

void CGfxCamera::SetClearDepth(float depth)
{
	m_clearDepth = depth;
}

void CGfxCamera::SetEnableClearColor(bool bEnable)
{
	m_bEnableClearColor = bEnable;
}

void CGfxCamera::SetClearColor(float red, float green, float blue, float alpha)
{
	m_clearColorRed = red;
	m_clearColorGreen = green;
	m_clearColorBlue = blue;
	m_clearColorAlpha = alpha;
}

void CGfxCamera::SetViewport(float x, float y, float width, float height)
{
	m_camera.setViewport(x, y, width, height);
}

void CGfxCamera::SetPerspective(float fovy, float aspect, float zNear, float zFar)
{
	m_camera.setPerspective(fovy, aspect, zNear, zFar);
}

void CGfxCamera::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_camera.setOrtho(left, right, bottom, top, zNear, zFar);
}

void CGfxCamera::SetLookat(const glm::vec3 &position, const glm::vec3 &direction, const glm::vec3 &up)
{
	m_camera.setLookat(position, position + direction, up);
}

const float* CGfxCamera::GetProjectionMatrix(void) const
{
	return (float *)&m_camera.mtxProjection;
}

const float* CGfxCamera::GetViewMatrix(void) const
{
	return (float *)&m_camera.mtxView;
}

glm::vec3 CGfxCamera::WorldToScreen(const glm::vec3 &world)
{
	return m_camera.worldToScreen(world);
}

glm::vec3 CGfxCamera::ScreenToWorld(const glm::vec3 &screen)
{
	return m_camera.screenToWorld(screen);
}

bool CGfxCamera::IsVisible(const glm::vec3 &vertex)
{
	return m_camera.visible(vertex);
}

bool CGfxCamera::IsVisible(const glm::aabb &aabb)
{
	return m_camera.visible(aabb);
}

bool CGfxCamera::IsVisible(const glm::sphere &sphere)
{
	return m_camera.visible(sphere);
}

void CGfxCamera::AddQueue(GLuint material, CGfxMesh *pMesh, const glm::mat4 &mtxTransform)
{
	if (CGfxMaterial *pMaterial = CGfxRenderer::GetInstance()->GetMaterial(material)) {
		if (m_meshs[pMesh] == NULL) {
			m_meshs[pMesh] = pMesh;

			glm::vec4 position = mtxTransform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			glm::vec3 distance = glm::vec3(position.x - m_camera.position.x, position.y - m_camera.position.y, position.z - m_camera.position.z);
			GLuint length = (GLuint)glm::length(distance);

			if (pMaterial->IsEnableBlend()) {
				m_queueTransparent[material][UINT_MAX - length].push_back(pMesh);
			}
			else {
				m_queueOpaque[material][length].push_back(pMesh);
			}
		}

		pMesh->AddInstance(mtxTransform);
	}
}

void CGfxCamera::ClearQueue(void)
{
	for (const auto &itMesh : m_meshs) {
		itMesh.second->ClearInstance();
	}

	m_meshs.clear();
	m_queueOpaque.clear();
	m_queueTransparent.clear();
}

void CGfxCamera::CmdDraw(void)
{
	CGfxRenderer::GetInstance()->CmdBeginPass(m_pCommandBuffer, m_pFrameBuffer);
	{
		CGfxRenderer::GetInstance()->CmdSetScissor(m_pCommandBuffer, 0, 0, m_pFrameBuffer->GetWidth(), m_pFrameBuffer->GetHeight());
		CGfxRenderer::GetInstance()->CmdSetViewport(m_pCommandBuffer, 0, 0, m_pFrameBuffer->GetWidth(), m_pFrameBuffer->GetHeight());

		if (m_bEnableClearDepth) {
			CGfxRenderer::GetInstance()->CmdClearDepth(m_pCommandBuffer, m_clearDepth);
		}

		if (m_bEnableClearColor) {
			CGfxRenderer::GetInstance()->CmdClearColor(m_pCommandBuffer, m_clearColorRed, m_clearColorGreen, m_clearColorBlue, m_clearColorAlpha);
		}

		for (const auto &itMaterialQueue : m_queueOpaque) {
			CGfxRenderer::GetInstance()->CmdSetMaterial(m_pCommandBuffer, itMaterialQueue.first);

			for (const auto &itDistanceQueue : itMaterialQueue.second) {
				for (size_t index = 0; index < itDistanceQueue.second.size(); index++) {
					CGfxRenderer::GetInstance()->CmdDrawInstance(m_pCommandBuffer, itDistanceQueue.second[index]);
				}
			}
		}
	}
	CGfxRenderer::GetInstance()->CmdEndPass(m_pCommandBuffer);
}
