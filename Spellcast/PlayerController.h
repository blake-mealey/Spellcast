#pragma once

#include "Component.h"
#include "Listener.h"
#include "KeyboardEvent.h"
#include "MouseMovedEvent.h"

#include <json/json.hpp>

class PlayerControllerDesc : public ComponentDesc {
public:
	PlayerControllerDesc();
	explicit PlayerControllerDesc(nlohmann::json& a_data);
	void Create(Entity* a_entity) override;

	float m_speed;
};

class PlayerController : public Component,
                         public Listener<KeyboardEvent>,
                         public Listener<MouseMovedEvent> {
friend SlotMap<PlayerController>;
public:
	~PlayerController() override = default;
	PlayerController();
	static component_type GetType();
	static component_index GetTypeIndex();

	bool Init(const PlayerControllerDesc* a_desc);

	void On(const KeyboardEvent& a_event) override;
	void On(const MouseMovedEvent& a_event) override;

private:
	float m_speed;
};
