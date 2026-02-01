#include "app.h"

void App::InitSDL()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer("Particle simulation", gbl::WIDTH, gbl::HEIGHT, 0, &pWindow, &pRenderer);
	SDL_SetRenderVSync(pRenderer, 1);
	SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
}

void App::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	pIo = &ImGui::GetIO();
	pIo->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL3_InitForSDLRenderer(pWindow, pRenderer);
	ImGui_ImplSDLRenderer3_Init(pRenderer);
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
	if (pIo->WantCaptureMouse)
	{
		mIsMouseBallActive = false;
		return;
	}
	float mouseX, mouseY;
	SDL_MouseButtonFlags buttons = SDL_GetMouseState(&mouseX, &mouseY);
	glm::vec2 mousePos(mouseX, mouseY);
	mMouseBall.position = mousePos;
	mIsMouseBallActive = buttons & SDL_BUTTON_LMASK;
}

void App::UpdateGUI()
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Settings"); 
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Particle settings"); 
		ImGui::Separator();
		ImGui::SliderInt("Number of particles", &mSettings.numBalls, 1, 5000, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Ball min radius", &mSettings.ballMinRadius, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Ball max radius", &mSettings.ballMaxRadius, mSettings.ballMinRadius, 100, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Ball min velocity", &mSettings.ballMinVelocity, 1, 2000, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Ball max velocity", &mSettings.ballMaxVelocity, mSettings.ballMinVelocity, 2000, "%d", ImGuiSliderFlags_AlwaysClamp);
		if (ImGui::Button("Apply changes"))
		{
			mBalls.clear();
			InitBalls();
		}
		ImGui::Dummy(ImVec2(0.0f, 20.0f)); 
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Other settings"); 
		ImGui::Separator();
		ImGui::SliderFloat("Gravity", &mSettings.gravity, -5000.f, 5000.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderFloat("Collision damping", &mSettings.collisionDamping, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderFloat("Air resistance", &mSettings.airResistance, 0.f, 0.005f, "%.5f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderFloat("Mouse ball radius", &mSettings.mouseBallRadius, 1.f, 100.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderFloat("Mouse ball force", &mSettings.mouseBallForce, 0.f, 50000.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
		EditSDLColor("Mouse ball color", mSettings.MouseBallCol);
		EditSDLColor("Background color", mSettings.BackgroundCol);
		EditSDLColor("Balls color", mSettings.BallsCol);
	}
	ImGui::End();
}

void App::RenderGUI() const
{
	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), pRenderer);
}

App::App()
	: mMouseBall(mSettings.mouseBallRadius, mSettings.MouseBallCol, glm::vec2(0.f), glm::vec2(0.f))
{
	InitSDL();
	InitImGui();
	InitBalls();
}

App::~App()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
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
			ImGui_ImplSDL3_ProcessEvent(&event);
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
	mMouseBall.radius = mSettings.mouseBallRadius;
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
		ball.velocity.y += mSettings.gravity * deltaTime;
		ball.velocity *= 1.f - std::log(mSettings.airResistance + 1);
		ball.Update(deltaTime);
	}
	HandleCollisions(deltaTime);
	UpdateGUI();
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
	RenderGUI();
}

void App::EditSDLColor(const char* label, SDL_Color& color)
{
	float temp[3] = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f };
	if (ImGui::ColorEdit3(label, temp, ImGuiColorEditFlags_DisplayRGB))
	{
		color.r = static_cast<Uint8>(temp[0] * 255.0f);
		color.g = static_cast<Uint8>(temp[1] * 255.0f);
		color.b = static_cast<Uint8>(temp[2] * 255.0f);
	}
}
