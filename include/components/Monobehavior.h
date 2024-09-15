#pragma once

#include "components/IComponent.h"

class Monobehavior : public IComponent {
public:
	virtual ~Monobehavior() = default;

	virtual void awake(){}

	virtual void start(){}

	virtual void update(){}
};