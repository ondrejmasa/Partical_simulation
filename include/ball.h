# pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <glm/vec2.hpp>
#include "global.h"

class Ball {
public:
	glm::vec2 position;
	glm::vec2 velocity;
	float radius;
	Uint8 color[4]{ 255, 255, 255, 255 };
	void Update(const float dt);
	void Render(SDL_Renderer* aRenderer) const;
	const bool collidesWith(const Ball& aOther) const;
};