#include "util/Time.h"

// Define static variables
Uint32 Time::lastTime = SDL_GetTicks();  // Initialize the lastTime when the program starts
float Time::deltaTime = 0.0f;
