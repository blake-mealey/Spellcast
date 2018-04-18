#pragma once

#include "ComponentType.h"

class Component {
public:
	Component();
	virtual ~Component();

	virtual void SetEnabled(bool a_enabled);
	bool IsEnabled() const;

	static component_type GetType();
	static component_index GetTypeIndex();

protected:
	bool m_enabled;
};
