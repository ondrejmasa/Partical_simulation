# pragma once
#include <iostream>
#include <vector>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include <glm/geometric.hpp>
#include "fpsCounter.h"
#include "global.h"
#include "ball.h"

struct Settings
{
	static constexpr int WIDTH = 1280;
	static constexpr int HEIGHT = 720;
	int numBalls = 10;
	int ballMinRadius = 15;
	int ballMaxRadius = 30;
	int ballMinVelocity = 100;
	int ballMaxVelocity = 500;
	float mouseBallForce = 100000.f;
	float mouseBallRadius = 30.f;
	float collisionDamping = 0.95f;
	SDL_Color MouseBallCol = { 255, 0, 0, 255 };
	SDL_Color BackgroundCol = { 0, 0, 0, 255 };
	SDL_Color BallsCol = { 255, 255, 255, 255 };
};

class App {
private:
	Settings mSettings;
	SDL_Window* pWindow;
	SDL_Renderer* pRenderer;
	FpsCounter mFpsCounter;
	std::vector<Ball> mBalls;
	Ball mMouseBall;
	bool mIsMouseBallActive = false;

	void InitBalls();
	void InitSDL();
	void HandleCollisions(const float dt);
	void UpdateMouseBall();
	void Update();
	void Render() const;
public:
	App();
	virtual ~App();
	void Run();
};