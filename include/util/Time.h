#pragma once

#include <SDL/SDL.h>

class Time {
public:
    static float deltaTime;  // Time between the current and previous frame in seconds

    static void update() {
        // Get the current time in milliseconds
        Uint32 currentTime = SDL_GetTicks();

        // Calculate the delta time (in seconds)
        deltaTime = (currentTime - lastTime) / 1000.0f;

        // Store current time as last time for the next frame
        lastTime = currentTime;
    }

private:
    static Uint32 lastTime;  // Time at the start of the previous frame
};
