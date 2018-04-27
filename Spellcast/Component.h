#pragma once

#include "ComponentType.h"
#include "World.h"

class Entity;
class Component;

class ComponentDesc {
public:
	virtual ~ComponentDesc() = default;
	virtual void Create(Entity* a_entity) = 0;
};

class Component {
friend SlotMap<Component>;
friend Entity;
public:
	Component();
	virtual ~Component();

	component_id GetId() const;

	virtual void SetEnabled(bool a_enabled);
	bool IsEnabled() const;

	static component_type GetType();
	static component_index GetTypeIndex();
	
	Entity* GetEntity() const;

protected:
	bool m_enabled;
	component_id m_id;
	bool m_active;

	entity_id m_entity;
};
