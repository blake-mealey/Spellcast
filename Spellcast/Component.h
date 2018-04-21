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
public:
	Component();
	virtual ~Component();

	bool GetId() const;

	virtual void SetEnabled(bool a_enabled);
	bool IsEnabled() const;

	static component_type GetType();
	static component_index GetTypeIndex();

protected:
	bool m_enabled;
	component_id m_id;
	bool m_active;
};
