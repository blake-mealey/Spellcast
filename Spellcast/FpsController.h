#pragma once

#include "Component.h"
#include "Listener.h"
#include "KeyboardEvent.h"
#include "MouseMovedEvent.h"

#include <json/json.hpp>

class FpsControllerDesc : public ComponentDesc {
public:
	FpsControllerDesc();
	explicit FpsControllerDesc(nlohmann::json& a_data);
	void Create(Entity* a_entity) override;

	float m_moveSpeed;
	float m_cameraSpeed;
};

class FpsController : public Component,
                         public Listener<KeyboardEvent>,
                         public Listener<MouseMovedEvent> {
friend SlotMap<FpsController>;
public:
	~FpsController() override = default;
	FpsController();
	static component_type GetType();
	static component_index GetTypeIndex();

	bool Init(const FpsControllerDesc* a_desc);

	void On(const KeyboardEvent& a_event) override;
	void On(const MouseMovedEvent& a_event) override;

private:
	float m_moveSpeed;
	float m_cameraSpeed;

	bool m_locked;
};
