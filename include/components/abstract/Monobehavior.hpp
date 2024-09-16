#pragma once

#include "components/abstract/IComponent.hpp"

class Monobehavior : public IComponent {
public:
	virtual ~Monobehavior() = default;

	virtual void awake(){}

	virtual void start(){}

	virtual void update(){}
};