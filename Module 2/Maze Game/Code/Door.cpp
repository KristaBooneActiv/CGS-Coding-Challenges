#include <iostream>
#include <windows.h>
#include "Door.h"

Door::Door(int x, int y, ActorColor color, ActorColor closedColor)
	: PlacableActor(x, y, '|', color)
	, m_isOpen(false)
	, m_closedColor(closedColor)
{

}

ActorColor Door::GetColor() const
{
	if (m_isOpen)
	{
		return m_color;
	}
	else
	{
		return m_closedColor;
	}
}
