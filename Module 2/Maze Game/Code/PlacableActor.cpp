#include <windows.h>
#include <iostream>
#include "PlacableActor.h"

PlacableActor::PlacableActor(int x, int y, char c, ActorColor color)
	: m_pPosition(new Point(x, y))
	, m_IsActive(true)
	, m_color(color)
	, m_drawChar(c)
{

}

PlacableActor::~PlacableActor()
{
	delete m_pPosition;
	m_pPosition = nullptr;
}

int PlacableActor::GetXPosition()
{
	return m_pPosition->x;
}

int PlacableActor::GetYPosition()
{
	return m_pPosition->y;
}

int* PlacableActor::GetXPositionPointer()
{
	return &(m_pPosition->x);
}

int* PlacableActor::GetYPositionPointer()
{
	return &(m_pPosition->y);
}

void PlacableActor::SetPosition(int x, int y)
{
	m_pPosition->x = x;
	m_pPosition->y = y;
}

void PlacableActor::Place(int x, int y)
{
	m_pPosition->x = x;
	m_pPosition->y = y;
	m_IsActive = true;
}

// =============================================================
// Refactored Draw method so that it enforces each actor that is
// drawn takes up a single character on the screen.
// 
// Benefits:
//  - PlacableActors do not draw more than assumed
//  - Reduce duplicate code calling SetConsoleTextAttribute
// =============================================================
void PlacableActor::Draw() const
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, (int)GetColor());
	std::cout << m_drawChar;
	SetConsoleTextAttribute(console, (int)ActorColor::Regular);
}
