#pragma once
#include <string>
#include "Point.h"

enum class ActorColor
{
	Regular = 7,
	Blue = 9,
	Green = 10,
	Red = 12,
	SolidGreen = 34,
	SolidRed = 68,
	SolidBlue = 153,
	LightGrey_on_DarkGrey = 135
};

enum class ActorType
{
	Door,
	Enemy,
	Goal,
	Key,
	Money,
	Player,
	ShadowActivator,
	ShadowPlayer
};

static std::string ActorTypeToString(ActorType type)
{
	switch (type)
	{
	case ActorType::Door:
		return "Door";
	case ActorType::Enemy:
		return "Enemy";
	case ActorType::Goal:
		return "Goal";
	case ActorType::Key:
		return "Key";
	case ActorType::Money:
		return "Money";
	case ActorType::Player:
		return "Player";
	case ActorType::ShadowActivator:
		return "ShadowActivator";
	case ActorType::ShadowPlayer:
		return "ShadowPlayer";
	default:
		return "Undefined";
	}
}

class PlacableActor
{
public:
	PlacableActor(int x, int y, char c, ActorColor color = ActorColor::Regular);
	virtual ~PlacableActor();

	int GetXPosition();
	int GetYPosition();
	int* GetXPositionPointer();
	int* GetYPositionPointer();
	void SetPosition(int x, int y);

	virtual ActorColor GetColor() const { return m_color; }

	void Remove() { m_IsActive = false; }
	bool IsActive() { return m_IsActive; }
	void Place(int x, int y);
	void Draw() const;

	virtual ActorType GetType() = 0;
	virtual void Update()
	{

	}

protected:
	Point* m_pPosition;

	bool m_IsActive;
	ActorColor m_color;
	char m_drawChar;
};