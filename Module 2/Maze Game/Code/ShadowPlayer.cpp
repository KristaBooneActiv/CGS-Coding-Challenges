#include <iostream>
#include <windows.h>
#include "ShadowPlayer.h"

ShadowPlayer::ShadowPlayer(int x, int y)
	: PlacableActor(x, y, '@', ActorColor::LightGrey_on_DarkGrey)
{ /* no-op */ }

ActorType ShadowPlayer::GetType()
{
	return ActorType::ShadowPlayer;
}
