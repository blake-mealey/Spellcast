#include "Camera.h"
#include "GraphicsSystem.h"
#include "World.h"
#include "Entity.h"
#include "Geometry.h"
#include "SkyboxRenderer.h"
#include "MeshRenderer.h"
#include "Logger.h"
#include "Uniforms.h"

#include <glm/gtc/matrix_transform.inl>

using namespace glm;
using namespace nlohmann;

CameraDesc::CameraDesc(): m_nearClippingPlane(DEFAULT_NEAR_CLIPPING_PLANE),
                          m_farClippingPlane(DEFAULT_FAR_CLIPPING_PLANE),
                          m_upVector(Geometry::UP), m_fieldOfView(radians(DEFAULT_FIELD_OF_VIEW)),
                          m_globalPosition(-Geometry::FORWARD),
                          m_viewportUnitScale(vec2(1.f)) {}

CameraDesc::CameraDesc(json& a_data): CameraDesc() {
	m_nearClippingPlane = ContentManager::FromJson(a_data, "NearClippingPlane", m_nearClippingPlane);
	m_farClippingPlane = ContentManager::FromJson(a_data, "FarClippingPlane", m_farClippingPlane);

	m_upVector = ContentManager::VecFromJson(a_data, "UpVector", m_upVector);
	m_fieldOfView = radians(ContentManager::FromJson(a_data, "FieldOfView", DEFAULT_FIELD_OF_VIEW));

	m_globalPosition = ContentManager::VecFromJson(a_data, "Position", m_globalPosition);
	m_targetGlobalPosition = ContentManager::VecFromJson(a_data, "Target", m_targetGlobalPosition);

	m_viewportUnitScale = ContentManager::VecFromJson(a_data, "ViewportUnitScale", m_viewportUnitScale);
	m_viewportUnitPosition = ContentManager::VecFromJson(a_data, "ViewportUnitPosition", m_viewportUnitPosition);

	m_viewportPixelScale = ContentManager::VecFromJson(a_data, "ViewportPixelScale", m_viewportPixelScale);
	m_viewportPixelPosition = ContentManager::VecFromJson(a_data, "ViewportPixelPosition", m_viewportPixelPosition);
}

void CameraDesc::Create(Entity* a_entity) {
	auto* camera = World::CreateAndGetComponent<Camera>();
	camera->Init(*this);
	a_entity->AddComponent(camera);
}


Camera::~Camera() {
	if (m_depthStencilBuffer) glDeleteRenderbuffers(2, &m_depthStencilBuffer);
	if (m_screenBuffer) glDeleteFramebuffers(1, &m_screenBuffer);
	if (m_screenVao) glDeleteVertexArrays(1, &m_screenVao);
	if (m_screenVbo) glDeleteBuffers(1, &m_screenVbo);
}

Camera::Camera() : m_depthStencilBuffer(0), m_screenBuffer(0), m_glowBuffer(0), m_screenVao(0), m_screenVbo(0),
                   m_blurTextures{}, m_blurTempTextures{},
                   m_nearClippingPlane(0.f),
                   m_farClippingPlane(0.f),
                   m_fieldOfView(0.f) {};

component_type Camera::GetType() {
	return Component::GetType() | (1 << GetTypeIndex());
}

component_index Camera::GetTypeIndex() {
	return ComponentTypeIndex::CAMERA;
}

bool Camera::Init(const CameraDesc& a_desc) {
	// Initialize buffers and textures
	if (!GenerateBuffers()) return false;
	if (!InitVaoAndVbo()) return false;
	if (!InitGlowBuffer()) return false;
	if (!InitScreenBuffer()) return false;

	// Initialize shaders
	if (!m_copyShader.Init()) return false;
	if (!m_blurShader.Init()) return false;

	// Copy data from description
	m_nearClippingPlane = a_desc.m_nearClippingPlane;
	m_farClippingPlane = a_desc.m_farClippingPlane;
	
	m_upVector = a_desc.m_upVector;
	m_fieldOfView = a_desc.m_fieldOfView;

	m_globalPosition = a_desc.m_globalPosition;
	m_targetGlobalPosition = a_desc.m_targetGlobalPosition;

	m_viewportUnitScale = a_desc.m_viewportUnitScale;
	m_viewportUnitPosition = a_desc.m_viewportUnitPosition;
	
	m_viewportPixelScale = a_desc.m_viewportPixelScale;
	m_viewportPixelPosition = a_desc.m_viewportPixelPosition;

	return true;
}

bool Camera::GenerateBuffers() {
	// Generate buffers
	glGenFramebuffers(2, &m_screenBuffer);
	glGenRenderbuffers(1, &m_depthStencilBuffer);
	glGenVertexArrays(1, &m_screenVao);
	glGenBuffers(1, &m_screenVbo);

	return glGetError() == GL_NO_ERROR;
}

bool Camera::InitVaoAndVbo() {
	// Initialize VAO
	glBindVertexArray(m_screenVao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_screenVbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));
	glBindVertexArray(0);

	// Initialize VBO
	const vec2 verts[4] = {
		vec2(-1, -1),
		vec2( 1, -1),
		vec2(-1,  1),
		vec2( 1,  1)
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 4, verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return glGetError() == GL_NO_ERROR;
}

bool Camera::InitGlowBuffer() {
	// Initialize glow buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_glowBuffer);

	GLenum glowBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, glowBuffers);

	for (size_t i = 0; i < BLUR_LEVEL_COUNT; ++i) {
		// const float factor = 1.f / pow(2, i);

		m_blurTextures[i].Init(GL_RGBA, 1, 1, nullptr, GL_CLAMP_TO_EDGE);
		m_blurTempTextures[i].Init(GL_RGBA, 1, 1, nullptr, GL_CLAMP_TO_EDGE);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return glGetError() == GL_NO_ERROR;
}

bool Camera::InitScreenBuffer() {
	// Initialize screen buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_screenBuffer);

	GLenum screenBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, screenBuffers);

	// Normal color buffer
	m_screenTexture.Init(GL_RGBA, 1, 1, nullptr, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_screenTexture.GetId(), 0);

	// Glow color buffer
	m_glowTexture.Init(GL_RGBA, 1, 1, nullptr, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_glowTexture.GetId(), 0);

	// Depth buffer
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1, 1);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Logger::Console()->warn("Screen framebuffer incomplete.");
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return glGetError() == GL_NO_ERROR;
}

void Camera::Render(const GraphicsSystem& a_context) {
	// Compute current viewport
	const vec2& windowDims = a_context.GetWindowDims();
	const vec2 lastViewportPosition = m_viewportPosition;
	const vec2 lastViewportScale = m_viewportScale;
	m_viewportPosition = (windowDims * m_viewportUnitPosition) + m_viewportPixelPosition;
	m_viewportScale = (windowDims * m_viewportUnitScale) + m_viewportPixelScale;
	glViewport(0, 0, m_viewportScale.x, m_viewportScale.y);

	// Update texture sizes
	if (m_viewportScale != lastViewportScale) {
		const int width = m_viewportScale.x, height = m_viewportScale.y;

		m_screenTexture.UpdateDimensions(width, height);
		m_glowTexture.UpdateDimensions(width, height);

		glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

		for (size_t i = 0; i < BLUR_LEVEL_COUNT; ++i) {
			const float factor = 1.f / pow(2, i);

			m_blurTextures->UpdateDimensions(width, height);
			m_blurTempTextures->UpdateDimensions(width, height);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	// Compute current aspect ratio
	const float aspectRatio = m_viewportScale.x / m_viewportScale.y;

	// Compute view and projection matrices
	const mat4 viewMatrix = lookAt(m_globalPosition, m_targetGlobalPosition, m_upVector);
	const mat4 projectionMatrix = perspective(m_fieldOfView, aspectRatio, m_nearClippingPlane, m_farClippingPlane);

	// Render to the screen buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_screenBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render meshes
	for (auto it = World::BeginComponents<MeshRenderer>(); it != World::EndComponents<MeshRenderer>(); ++it) {
		it->Render(viewMatrix, projectionMatrix);
	}

	// Render skyboxes
	glDisable(GL_CULL_FACE);
	for (auto it = World::BeginComponents<SkyboxRenderer>(); it != World::EndComponents<SkyboxRenderer>(); ++it) {
		it->Render(viewMatrix, projectionMatrix);
	}
	glEnable(GL_CULL_FACE);

	// Rotate entities... for reasons
	for (auto it = World::BeginEntities(); it != World::EndEntities(); ++it) {
		it->GetTransform().Rotate(Geometry::UP, 0.01f);
	}

	// Post-processing effects
	glBindFramebuffer(GL_FRAMEBUFFER, m_glowBuffer);
	glBindVertexArray(m_screenVao);

	m_copyShader.Enable();
	m_glowTexture.Bind(ALBEDO_TEXTURE_UNIT);

	// Copy the glow buffer to each of the level buffers
	for (size_t i = 0; i < BLUR_LEVEL_COUNT; ++i) {
		const float factor = 1.f / pow(2, i);
		glViewport(0, 0, m_viewportScale.x * factor, m_viewportScale.y * factor);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_blurTextures[i].GetId(), 0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	m_blurShader.Enable();

	for (size_t i = 0; i < BLUR_LEVEL_COUNT; ++i) {
		// Get the relevant buffers
		const Texture& buffer = m_blurTextures[i];
		const Texture& blurBuffer = m_blurTempTextures[i];

		// Set the viewport
		const float factor = 1.f / pow(2, i);
		glViewport(0, 0, m_viewportScale.x * factor, m_viewportScale.y * factor);

		// Calculate the blur offsets
		const float xOffset = 1.2f / (m_viewportScale.x * factor);
		const float yOffset = 1.2f / (m_viewportScale.y * factor);

		// Blur on the x-axis
		buffer.Bind(ALBEDO_TEXTURE_UNIT);
		m_blurShader.SetOffset(vec2(xOffset, 0.f));
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurBuffer.GetId(), 0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// Blur on the y-axis
		blurBuffer.Bind(ALBEDO_TEXTURE_UNIT);
		m_blurShader.SetOffset(vec2(0.f, yOffset));
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer.GetId(), 0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	// Copy the screen buffer to the window
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(m_screenVao);

	m_copyShader.Enable();
	m_screenTexture.Bind(ALBEDO_TEXTURE_UNIT);

	glViewport(m_viewportPosition.x, m_viewportPosition.y, m_viewportScale.x, m_viewportScale.y);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);



	glDepthMask(GL_FALSE);
	glBlendFunc(GL_ONE, GL_ONE);

	for (size_t i = 0; i < BLUR_LEVEL_COUNT; ++i) {
		m_blurTextures[i].Bind(ALBEDO_TEXTURE_UNIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);
}

void Camera::SetViewportUnitScale(const vec2& a_scale) {
	m_viewportUnitScale = a_scale;
}

void Camera::SetViewportUnitPosition(const vec2& a_position) {
	m_viewportUnitPosition = a_position;
}