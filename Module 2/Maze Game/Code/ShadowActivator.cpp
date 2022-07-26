#include <iostream>
#include <windows.h>

#include "ShadowActivator.h"

ShadowActivator::ShadowActivator(int x, int y)
	: PlacableActor(x, y, '*', ActorColor::LightGrey_on_DarkGrey)
{ /* no-op */ }
