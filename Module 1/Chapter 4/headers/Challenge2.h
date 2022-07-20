#pragma once
#include "IChallenge.h"

class Challenge2 : public IChallenge
{
public:
	std::string details() const override;
	void run() override;
};