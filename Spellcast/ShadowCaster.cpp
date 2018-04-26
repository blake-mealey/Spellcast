#include "ShadowCaster.h"
#include "Logger.h"
#include "World.h"
#include "MeshRenderer.h"
#include "Uniforms.h"

using namespace glm;

const mat4 ShadowCaster::BIAS_MATRIX = mat4(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
);

ShadowCaster::~ShadowCaster() {
	if (m_shadowBuffer) glDeleteFramebuffers(1, &m_shadowBuffer);
}

ShadowCaster::ShadowCaster() : m_shadowBuffer(0), m_shadowIntensity(0), m_castsShadows(false), m_shadowMapSize(0) {}

bool ShadowCaster::InitShadowMap() {
	// Initialize shaders
	if (!m_shadowMapShader.Init()) return false;
	if (!m_shadowShader.Init()) return false;

	// Generate shadow map framebuffer
	glGenFramebuffers(1, &m_shadowBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowBuffer);

	// Add depth texture
	TextureDesc desc(m_shadowMapSize, m_shadowMapSize, nullptr);
	desc.m_internalFormat = GL_DEPTH_COMPONENT16;
	desc.m_format = GL_DEPTH_COMPONENT;
	desc.m_type = GL_FLOAT;
	desc.m_wrapS = GL_CLAMP_TO_EDGE;
	desc.m_wrapT = GL_CLAMP_TO_EDGE;
	desc.m_compareFunc = GL_GEQUAL;
	desc.m_compareMode = GL_COMPARE_REF_TO_TEXTURE;
	m_shadowMap.Init(desc);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowMap.GetId(), 0);

	// No draw buffers
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Logger::Console()->warn("Shadow map framebuffer incomplete.");
	
	return glGetError() == GL_NO_ERROR;
}

void ShadowCaster::RenderShadowMap() {
	if (!m_castsShadows) return;

	// Render to the shadow map framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowBuffer);

	// Clear the depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	m_shadowMapShader.Enable();

	glViewport(0, 0, m_shadowMapSize, m_shadowMapSize);

	ComputeDepthViewProjectionMatrix();

	for (auto it = World::BeginComponents<MeshRenderer>(); it != World::EndComponents<MeshRenderer>(); ++it) {
		const MeshRenderer& meshRenderer = *it;
		if (!meshRenderer.IsEnabled() || !meshRenderer.DoesCastShadows()) continue;
		
		const mat4& depthModelMatrix = meshRenderer.GetTransform().GetTransformationMatrix();
		m_shadowMapShader.SetDepthModelViewProjectionMatrix(m_depthViewProjectionMatrix * depthModelMatrix);
		meshRenderer.RenderBasic();
	}
}

void ShadowCaster::RenderShadows(const mat4& a_viewProjectionMatrix) const {
	if (!m_castsShadows) return;

	m_shadowShader.Enable();
	m_shadowShader.SetIntensity(m_shadowIntensity);

	m_shadowMap.Bind(SHADOW_TEXTURE_UNIT);
	const mat4 depthBiasViewProjectionMatrix = BIAS_MATRIX * m_depthViewProjectionMatrix;
	for (auto it = World::BeginComponents<MeshRenderer>(); it != World::EndComponents<MeshRenderer>(); ++it) {
		if (!it->IsEnabled() || !it->DoesReceiveShadows()) continue;
		const mat4 modelMatrix = it->GetTransform().GetTransformationMatrix();
		m_shadowShader.SetModelViewProjectionMatrix(a_viewProjectionMatrix * modelMatrix);
		m_shadowShader.SetDepthBiasModelViewProjectionMatrix(depthBiasViewProjectionMatrix * modelMatrix);
		it->RenderBasic();
	}
}

void ShadowCaster::SetCastsShadows(const bool a_castsShadows) {
	m_castsShadows = a_castsShadows;
	if (!m_shadowBuffer) InitShadowMap();
}

const Texture& ShadowCaster::GetShadowMap() const {
	return m_shadowMap;
}

const mat4& ShadowCaster::GetDepthViewProjectionMatrix() const {
	return m_depthViewProjectionMatrix;
}

bool ShadowCaster::CastsShadows() const {
	return m_castsShadows;
}
