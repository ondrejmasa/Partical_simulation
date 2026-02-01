#include "app.h"

void App::InitSDL()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer("Particle simulation", gbl::WIDTH, gbl::HEIGHT, 0, &pWindow, &pRenderer);
	SDL_SetRenderVSync(pRenderer, 1);
	SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
}

void App::InitBalls()
{
	mBalls.reserve(mSettings.numBalls);
	for (int i = 0; i < mSettings.numBalls; ++i)
	{
		float radius = static_cast<float>(mSettings.ballMinRadius + rand() % (mSettings.ballMaxRadius - mSettings.ballMinRadius + 1));
		glm::vec2 position = {
			radius + rand() % (gbl::WIDTH - static_cast<int>(2 * radius)),
			radius + rand() % (gbl::HEIGHT - static_cast<int>(2 * radius))
		};
		glm::vec2 velocity{
			mSettings.ballMinVelocity + rand() % (mSettings.ballMaxVelocity - mSettings.ballMinVelocity + 1),
			mSettings.ballMinVelocity + rand() % (mSettings.ballMaxVelocity - mSettings.ballMinVelocity + 1)
		};
		if (rand() % 2 == 0) velocity.x = -velocity.x;
		if (rand() % 2 == 0) velocity.y = -velocity.y;
		Ball ball(radius, mSettings.BallsCol, position, velocity);
		mBalls.push_back(ball);
	}
}

void App::HandleCollisions(const float dt)
{
	for (size_t i = 0; i < mBalls.size(); ++i)
	{
		for (size_t j = i + 1; j < mBalls.size(); ++j)
		{
			if (mBalls[i].collidesWith(mBalls[j]))
			{
				glm::vec2 dir = mBalls[i].position - mBalls[j].position;
				glm::vec2 norm = glm::normalize(dir);
				float d = (mBalls[i].radius + mBalls[j].radius) - glm::length(dir);
				mBalls[i].position += d * norm / 2.f;
				mBalls[j].position -= d * norm / 2.f;

				glm::vec2 t = glm::vec2(-norm.y, norm.x);
				float t1v = glm::dot(mBalls[i].velocity, t);
				float t2v = glm::dot(mBalls[j].velocity, t);
				float n1v = glm::dot(mBalls[i].velocity, norm);
				float n2v = glm::dot(mBalls[j].velocity, norm);

				mBalls[i].velocity = mSettings.collisionDamping * (t1v * t + n2v * norm);
				mBalls[j].velocity = mSettings.collisionDamping * (t2v * t + n1v * norm);
			}
		}

		if (mIsMouseBallActive) {
			if (mBalls[i].collidesWith(mMouseBall))
			{
				glm::vec2 dir = glm::normalize(mBalls[i].position - mMouseBall.position);
				mBalls[i].velocity += dir * mSettings.mouseBallForce * dt;
			}
		}
	}
}

void App::UpdateMouseBall()
{
	float mouseX, mouseY;
	SDL_MouseButtonFlags buttons = SDL_GetMouseState(&mouseX, &mouseY);
	glm::vec2 mousePos(mouseX, mouseY);
	mMouseBall.position = mousePos;
	mIsMouseBallActive = buttons & SDL_BUTTON_LMASK;
}

App::App()
	: mMouseBall(mSettings.mouseBallRadius, mSettings.MouseBallCol, glm::vec2(0.f), glm::vec2(0.f))
{
	InitSDL();
	InitBalls();
}

App::~App()
{
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

void App::Run()
{
	SDL_Event event;
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				quit = true;
			}
		}
		SDL_Color& col = mSettings.BackgroundCol;
		SDL_SetRenderDrawColor(pRenderer, col.r, col.g, col.b, col.a);
		SDL_RenderClear(pRenderer);
		Update();
		Render();
		SDL_RenderPresent(pRenderer);
	}
}

void App::Update()
{
	bool isFpsUpdate = mFpsCounter.Update();
	float deltaTime = mFpsCounter.GetDeltaTime();
	if (isFpsUpdate)
	{
		float fps = mFpsCounter.GetFps();
		SDL_SetWindowTitle(pWindow, ("FPS: " + std::to_string(fps)).c_str());
	}
	UpdateMouseBall();
	for (Ball& ball : mBalls)
	{
		ball.Update(deltaTime);
	}
	HandleCollisions(deltaTime);
}

void App::Render() const
{
	for (const Ball& ball : mBalls)
	{
		ball.Render(pRenderer);
	}
	if (mIsMouseBallActive)
	{
		mMouseBall.Render(pRenderer);
	}
}
