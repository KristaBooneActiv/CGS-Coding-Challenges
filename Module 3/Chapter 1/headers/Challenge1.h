#pragma once
#include "IChallenge.h"

class Challenge1 : public IChallenge
{
public:
	Challenge1() = default;
	~Challenge1() = default;

	std::string details() const override;
	void run() override;

protected:
	void runMultiProfile(int aNumElements,
		                 int aNumTimes,
		                 bool aPrintDebugFlag);

	uint32_t runSingleProfile(int aNumElements,
		                      bool aPrintDebugFlag);

	void fillArray(int aArray[], int aNumElements);
	int  partition(int aArray[], int aLow, int aHigh);
	void printArray(int aArray[], int aNumElements);
	void quickSort(int aArray[], int aLow, int aHigh);

};