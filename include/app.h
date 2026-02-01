# pragma once
#include <iostream>
#include <vector>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include <glm/geometric.hpp>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include "fpsCounter.h"
#include "global.h"
#include "ball.h"

struct Settings
{
	int numBalls = 500;
	int ballMinRadius = 7;
	int ballMaxRadius = 9;
	int ballMinVelocity = 100;
	int ballMaxVelocity = 300;
	float mouseBallForce = 25000.f;
	float mouseBallRadius = 30.f;
	float collisionDamping = 0.975f;
	float gravity = 0.f;
	float airResistance = 0.00025f;
	SDL_Color MouseBallCol = { 255, 40, 40, 255 };
	SDL_Color BackgroundCol = { 75, 170, 225, 255 };
	SDL_Color BallsCol = { 255, 75, 75, 255 };
};

class App {
private:
	Settings mSettings;
	SDL_Window* pWindow;
	SDL_Renderer* pRenderer;
	FpsCounter mFpsCounter;
	std::vector<Ball> mBalls;
	Ball mMouseBall;
	ImGuiIO* pIo;
	bool mIsMouseBallActive = false;

	void InitBalls();
	void InitSDL();
	void InitImGui();
	void HandleCollisions(const float dt);
	void UpdateMouseBall();
	void UpdateGUI();
	void RenderGUI() const;
	void Update();
	void Render() const;
	void EditSDLColor(const char* label, SDL_Color& color);
public:
	App();
	virtual ~App();
	void Run();
};