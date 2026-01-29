#include "app.h"

void App::InitBalls()
{
	mBalls.reserve(mNumBalls);
	for (int i = 0; i < mNumBalls; ++i)
	{
		Ball ball;
		ball.radius = static_cast<float>(15 + rand() % 15);
		ball.position = glm::vec2(
			ball.radius + rand() % (gbl::WIDTH - static_cast<int>(2 * ball.radius)),
			ball.radius + rand() % (gbl::HEIGHT - static_cast<int>(2 * ball.radius))
		);
		ball.velocity = glm::vec2(
			(-500 + rand() % 1000) / 2000.f,
			(-500 + rand() % 1000) / 2000.f
		);
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

				constexpr float momentumKeep = 0.95f;
				mBalls[i].velocity = momentumKeep * (t1v * t + n2v * norm);
				mBalls[j].velocity = momentumKeep * (t2v * t + n1v * norm);
			}
		}

		if (mIsMouseBallActive) {
			if (mBalls[i].collidesWith(mMouseBall))
			{
				glm::vec2 dir = glm::normalize(mBalls[i].position - mMouseBall.position);
				mBalls[i].velocity += dir * 0.1f * dt;
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
{
	InitBalls();
	mMouseBall.radius = 30.f;
	mMouseBall.color[0] = 255;
	mMouseBall.color[1] = 0;
	mMouseBall.color[2] = 0;
}

void App::Update(const float dt)
{
	UpdateMouseBall();
	for (Ball& ball : mBalls)
	{
		ball.Update(dt);
	}
	HandleCollisions(dt);
}

void App::Render(SDL_Renderer* aRenderer) const
{
	for (const Ball& ball : mBalls)
	{
		ball.Render(aRenderer);
	}
	if (mIsMouseBallActive)
	{
		mMouseBall.Render(aRenderer);
	}
}
