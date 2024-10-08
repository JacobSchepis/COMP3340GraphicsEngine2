#pragma once

#include "components/abstract/Monobehavior.hpp"
#include "components/Transform.h"

#include <memory>

class CameraController : public Monobehavior {
public:
	CameraController();

	void start() override;

	void update() override;

private:
	Transform* transform;

	void applyMovement();
	void applyRotation();
};

