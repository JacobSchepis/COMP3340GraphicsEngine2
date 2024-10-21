#include "scripts/DayNightLightCycle.h"

#include "util/Time.h"

#include <iostream>

void DayNightLightCycle::start() {
	light = parent->getComponent<Light>();
}

void DayNightLightCycle::update() {
    updateTime();

    glm::vec3 sunDirection = calculateSunDirection();
    glm::vec3 lightColor = calculateLightColor();
    glm::vec3 skyColor = calculateSkyColor();

    // Apply these to your shaders, light, and skybox system

    parent->getComponent<Light>()->direction = sunDirection;
    parent->getComponent<Light>()->ambient = lightColor;

    //setDirectionalLight(sunDirection, lightColor);
    //setSkyboxColor(skyColor);
}



void DayNightLightCycle::updateTime() {
    timeOfDay += daySpeed * Time::deltaTime;  // Increment time based on delta time
    if (timeOfDay > 1.0f) {
        timeOfDay = 0.0f;  // Loop back to start of the day
    }
}



glm::vec3 DayNightLightCycle::calculateSunDirection() {
	float sunAngle = 0.74f * 360.0f;  // Convert time to angle
	return glm::vec3(cos(glm::radians(sunAngle)), sin(glm::radians(sunAngle)), 0.0f);
}

glm::vec3 DayNightLightCycle::calculateLightColor() {
    if (timeOfDay < 0.25f || timeOfDay > 0.75f) {
        return glm::vec3(0.0f, 0.0f, 0.1f);  // Night (dark blue tint)
    }
    else if (timeOfDay < 0.5f) {
        return glm::vec3(1.0f, 0.0f, 0.0f);  // Morning (orange tint)
    }
    else {
        return glm::vec3(0.0f, 1.0f, 0.0f);  // Noon (white light)
    }
}

glm::vec3 DayNightLightCycle::calculateSkyColor() {
    if (timeOfDay < 0.25f || timeOfDay > 0.75f) {
        return glm::vec3(0.0f, 0.0f, 0.2f);  // Night sky (dark)
    }
    else if (timeOfDay < 0.5f) {
        return glm::vec3(0.7f, 0.5f, 0.3f);  // Dawn sky (orange)
    }
    else {
        return glm::vec3(0.5f, 0.7f, 1.0f);  // Daytime sky (blue)
    }
}

