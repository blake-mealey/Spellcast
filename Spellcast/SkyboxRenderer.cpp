#include "SkyboxRenderer.h"
#include "Uniforms.h"
#include "Mesh.h"
#include "Logger.h"

using namespace std;

SkyboxRendererDesc::SkyboxRendererDesc(): m_cubeMap(nullptr), m_mesh(nullptr), m_sunStrip(nullptr) {}

SkyboxRendererDesc::SkyboxRendererDesc(nlohmann::json& a_data) : SkyboxRendererDesc() {
	m_cubeMap = ContentManager::GetCubeMap("Skyboxes/" + ContentManager::FromJson<string>(a_data, "Path"));
	m_mesh = ContentManager::GetMesh(ContentManager::FromJson<string>(a_data, "Mesh", "Cube.obj"));
	m_sunStrip = ContentManager::GetTexture(ContentManager::FromJson<string>(a_data, "Sun", "SunStrip.png"));
}

void SkyboxRendererDesc::Create(Entity* a_entity) {
	auto* renderer = World::CreateAndGetComponent<SkyboxRenderer>();
	if (!renderer->Init(*this)) Logger::Console()->warn("SkyboxRenderer init failed.");
	a_entity->AddComponent(renderer);
}




SkyboxRenderer::SkyboxRenderer(): m_cubeMap(nullptr), m_mesh(nullptr), m_sunStrip(nullptr) {}

component_type SkyboxRenderer::GetType() {
	return Component::GetType() | (1 << GetTypeIndex());
}

component_index SkyboxRenderer::GetTypeIndex() {
	return ComponentTypeIndex::SKYBOX_RENDERER;
}

bool SkyboxRenderer::Init(const SkyboxRendererDesc& a_desc) {
	m_cubeMap = a_desc.m_cubeMap;
	m_mesh = a_desc.m_mesh;
	m_sunStrip = a_desc.m_sunStrip;

	return m_shader.Init();
}

void SkyboxRenderer::Render(const glm::mat4& a_viewMatrix, const glm::mat4& a_projectionMatrix) const {
	m_shader.Enable();
	
	m_shader.SetViewProjectionMatrix(a_projectionMatrix * glm::mat4(glm::mat3(a_viewMatrix)));
	m_sunStrip->Bind(SUN_STRIP_TEXTURE_UNIT);
	m_cubeMap->Bind(SKYBOX_TEXTURE_UNIT);

	m_mesh->RenderBasic();
}
