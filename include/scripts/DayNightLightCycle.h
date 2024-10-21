#pragma once

#include "components/abstract/Monobehavior.hpp"
#include "components/rendering/Light.hpp"
#include <glm/glm.hpp>

class DayNightLightCycle : public Monobehavior {
public:
	void start() override;

	void update() override;

private:
	float timeOfDay = 0.0f;  // Starts at midnight
	float daySpeed = 0.1f;  // Speed of time progression

	Light* light;

	void updateTime();

	glm::vec3 calculateSunDirection();

	glm::vec3 calculateLightColor();

	glm::vec3 calculateSkyColor();

	
};