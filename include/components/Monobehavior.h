#pragma once

#include "components/IComponent.h"

class Entity;

class Monobehavior : public IComponent {
public:
	Monobehavior(Entity* parent);
	~Monobehavior();

	virtual void awake();

	virtual void start();

	virtual void update();
};