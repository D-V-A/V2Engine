#pragma once

#include"types/rect.h"

//is object inside borders?
bool IsInsideBorders(const Rect& obj, const Rect& borders)
{
	return	obj.position.x + obj.size.x / 2 < borders.position.x + borders.size.x / 2 &&
			obj.position.x - obj.size.x / 2 > borders.position.x - borders.size.x / 2 &&
			obj.position.y + obj.size.y / 2 < borders.position.y + borders.size.y / 2 &&
			obj.position.y - obj.size.y /2 > borders.position.y - borders.size.y / 2;
}

//is object outside borders?
bool IsOutsideBorders(const Rect& obj, const Rect& borders)
{
	return	obj.position.x + obj.size.x / 2 < borders.position.x - borders.size.x / 2 ||
			obj.position.x - obj.size.x / 2 > borders.position.x + borders.size.x / 2 ||
			obj.position.y + obj.size.y / 2 < borders.position.y - borders.size.y / 2 ||
			obj.position.y - obj.size.y / 2 > borders.position.y + borders.size.y / 2;
}