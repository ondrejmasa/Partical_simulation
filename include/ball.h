# pragma once
#include <vector>
#include <SDL3/SDL.h>
#include <glm/vec2.hpp>
#include "global.h"

class Ball {
public:
	glm::vec2 position;
	glm::vec2 velocity;
	float radius;
	SDL_Color color;
	Ball(float aRadius, SDL_Color color, glm::vec2 aPosition, glm::vec2 aVelocity);
	void Update(const float dt);
	void Render(SDL_Renderer* aRenderer) const;
	const bool collidesWith(const Ball& aOther) const;
};