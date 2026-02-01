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
    //SDL_SetRenderDrawColor(aRenderer, color.r, color.g, color.b, color.a);
    //int r = (int)radius;
    //int x = 0;
    //int y = r;
    //int d = 3 - 2 * r;
    //while (y >= x) {
    //    SDL_RenderLine(aRenderer, position.x - x, position.y - y, position.x + x, position.y - y);
    //    SDL_RenderLine(aRenderer, position.x - x, position.y + y, position.x + x, position.y + y);
    //    SDL_RenderLine(aRenderer, position.x - y, position.y - x, position.x + y, position.y - x);
    //    SDL_RenderLine(aRenderer, position.x - y, position.y + x, position.x + y, position.y + x);
    //    x++;
    //    if (d > 0) {
    //        y--;
    //        d = d + 4 * (x - y) + 10;
    //    }
    //    else {
    //        d = d + 4 * x + 6;
    //    }
    //}

    // 1. Optimalizace: Èím menší kruh, tím ménì segmentù potøebuje.
    // Pro malé èástice staèí 12-16 segmentù, pro velké 30+.
    // Tím ušetøíte tisíce výpoètù.
    int segments = 20;

    // Vektor vrcholù (Vertices)
    // Potøebujeme støed + body po obvodu + 1 (aby se kruh uzavøel)
    std::vector<SDL_Vertex> verts;
    verts.reserve(segments + 2); // +2 (støed + uzavírací bod)

    // Pøevedeme barvu na float (SDL_Vertex vyžaduje 0.0-1.0 v SDL2/3 API se to mùže lišit,
    // ale v SDL3 Vertex struct obvykle bere FColor nebo Color struct.
    // Pro SDL3 Vertex používá SDL_FColor structure (r,g,b,a float).
    SDL_FColor fColor = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };
    float cx = position.x;
	float cy = position.y;
    // A. Støedový bod (Center)
    verts.push_back({ {cx, cy}, fColor, {0, 0} });

    // B. Body po obvodu
    for (int i = 0; i <= segments; i++)
    {
        float angle = (float)i / (float)segments * 2.0f * 3.1415926f;
        float x = cx + std::cos(angle) * radius;
        float y = cy + std::sin(angle) * radius;

        verts.push_back({ {x, y}, fColor, {0, 0} });
    }

    // C. Vykreslení
    // Používáme indices (pole poøadí), abychom vytvoøili "Triangle Fan"
    std::vector<int> indices;
    for (int i = 1; i <= segments; i++)
    {
        indices.push_back(0); // Støed
        indices.push_back(i); // Aktuální bod obvodu
        indices.push_back(i + 1); // Následující bod obvodu
    }

    // Odeslání na GPU
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
