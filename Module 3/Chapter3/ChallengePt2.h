#pragma once
#include "IChallenge.h"

class ChallengePt2 : public IChallenge
{
public:
	std::string details() const override;
	void run() override;
};
