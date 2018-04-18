#pragma once

#include "ComponentType.h"

class Component {
public:
	Component();
	virtual ~Component();

	virtual void SetEnabled(bool a_enabled);
	bool IsEnabled() const;

	virtual component_type GetType() const;

protected:
	bool m_enabled;
};
