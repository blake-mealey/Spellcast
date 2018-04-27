#pragma once

class System {
public:
	virtual ~System() = default;

	virtual void Update() = 0;
};
