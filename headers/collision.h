#pragma once

#include"types/rect.h"

//is object inside borders?
inline bool IsInside(const Rect& obj, const Rect& borders)
{
	return	obj.position.x + obj.size.x < borders.position.x + borders.size.x &&
			obj.position.x > borders.position.x &&
			obj.position.y + obj.size.y < borders.position.y + borders.size.y &&
			obj.position.y > borders.position.y;
}

//is object outside borders?
inline bool IsOutside(const Rect& obj, const Rect& borders)
{
	return	obj.position.x + obj.size.x < borders.position.x		||
			obj.position.x > borders.position.x + borders.size.x	||
			obj.position.y + obj.size.y < borders.position.y		||
			obj.position.y > borders.position.y + borders.size.y;
}