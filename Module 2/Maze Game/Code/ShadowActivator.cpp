#include <iostream>
#include <windows.h>

#include "ShadowActivator.h"

ShadowActivator::ShadowActivator(int x, int y)
	: PlacableActor(x, y, ActorColor::LightGrey_on_DarkGrey)
{ /* no-op */ }

void ShadowActivator::Draw()
{
	// Todo: move this up into the base class
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, (int)m_color);

	std::cout << "*";
	SetConsoleTextAttribute(console, (int)ActorColor::Regular);
}
