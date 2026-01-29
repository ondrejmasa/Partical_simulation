#include "ball.h"

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
    SDL_SetRenderDrawColor(aRenderer, color[0], color[1], color[2], color[3]);
    int r = (int)radius;
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;
    while (y >= x) {
        SDL_RenderLine(aRenderer, position.x - x, position.y - y, position.x + x, position.y - y);
        SDL_RenderLine(aRenderer, position.x - x, position.y + y, position.x + x, position.y + y);
        SDL_RenderLine(aRenderer, position.x - y, position.y - x, position.x + y, position.y - x);
        SDL_RenderLine(aRenderer, position.x - y, position.y + x, position.x + y, position.y + x);
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else {
            d = d + 4 * x + 6;
        }
    }
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
