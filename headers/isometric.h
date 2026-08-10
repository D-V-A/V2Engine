#pragma once

#include "types/vector2f.h"

//returns base point on the map surface 
Vector2f WorldToScreen(const Vector2f& worldPosition, const Vector2f& size /*tile size*/, const Vector2f& origin);

//returns top left point to start drawing
Vector2f GetTopLeft(const Vector2f& anchor, const Vector2f& size, const Vector2f& pivot);