#include "ball.h"

Ball::Ball(float aRadius, SDL_Color& color, glm::vec2 aPosition, glm::vec2 aVelocity)
    : radius{ aRadius }
    , color{ color }
    , position{ aPosition }
	, velocity{ aVelocity }
{
}

void Ball::Update(const float dt)
{
	position += velocity * dt;
	if (position.x - radius < 0.f)
	{
		position.x = radius;
		velocity.x = -velocity.x;
	}
    if (position.x + radius > gbl::WIDTH)
    {
        position.x = gbl::WIDTH - radius;
        velocity.x = -velocity.x;
    }
	if (position.y - radius < 0.f)
	{
		position.y = radius;
		velocity.y = -velocity.y;
	}
    if (position.y + radius > gbl::HEIGHT)
    {
		position.y = gbl::HEIGHT - radius;
        velocity.y = -velocity.y;
    }
}

void Ball::Render(SDL_Renderer* aRenderer) const
{
    int segments = 20;
    std::vector<SDL_Vertex> verts;
    verts.reserve(segments + 2); 
    SDL_FColor fColor = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };
    float cx = position.x;
	float cy = position.y;
    verts.push_back({ {cx, cy}, fColor, {0, 0} });
    for (int i = 0; i <= segments; i++)
    {
        float angle = (float)i / (float)segments * 2.0f * 3.1415926f;
        float x = cx + std::cos(angle) * radius;
        float y = cy + std::sin(angle) * radius;

        verts.push_back({ {x, y}, fColor, {0, 0} });
    }
    std::vector<int> indices;
    for (int i = 1; i <= segments; i++)
    {
        indices.push_back(0);
        indices.push_back(i); 
        indices.push_back(i + 1);
    }
    SDL_RenderGeometry(aRenderer, nullptr, verts.data(), verts.size(), indices.data(), indices.size());
}

const bool Ball::collidesWith(const Ball& aOther) const
{
    glm::vec2 diff = position - aOther.position;
	float len = diff.x * diff.x + diff.y * diff.y;
	float radiiSum = radius + aOther.radius;
    if (len < radiiSum*radiiSum)
    {
        return true;
	}
	return false;
}
