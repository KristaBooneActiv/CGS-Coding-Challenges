#include <iostream>
#include <windows.h>

#include "Key.h"

Key::Key(int x, int y, ActorColor color)
	: PlacableActor(x, y, '+', color)
{}
