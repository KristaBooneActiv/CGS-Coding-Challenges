#pragma once
#include <string>

class IChallenge
{
public:
	virtual std::string details() const = 0;
	virtual void run() = 0;
};