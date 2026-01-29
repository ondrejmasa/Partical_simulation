# pragma once
#include <SDL3/SDL.h>
#include "ball.h"
#include <vector>
#include <glm/geometric.hpp>

class App {
private:
	const int mNumBalls = 70;
	std::vector<Ball> mBalls;
	Ball mMouseBall;
	bool mIsMouseBallActive = false;
	void InitBalls();
	void HandleCollisions(const float dt);
	void UpdateMouseBall();
public:
	App();
	void Update(const float dt);
	void Render(SDL_Renderer* aRenderer) const;
};