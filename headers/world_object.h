#include "types/vector2f.h"
#include "types/rect.h"

#include "texture.h"

class Renderer;

class WorldObject
{
public:
	bool Initialize(Renderer& renderer, const char* texturePath);
	void Render(Renderer& renderer, const Vector2f& screenPosition) const;

	Vector2f GetPosition() const { return position; }

private:
	void CalculateRectSize();

	Texture texture;

	Rect textureRectangle{
	{0.0f, 0.0f},
	{0.0f, 0.0f} };
	Vector2f position{ 4.0f, 5.0f };
	Vector2f pivot = { 0.5f, 0.75f };//middle, lower
};