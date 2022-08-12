#pragma once
#include <thread>

typedef bool Pollen;
class Flower
{
public:
	Flower(size_t aTimerMin,
		   size_t aTimerMax);
	~Flower();

	static Pollen GetPollen();
	static void StopProduction();

protected:
	std::thread mProductionThread;
	size_t      mMinTimeToProduce;
	size_t      mMaxTimeToProduce;
	size_t      mId;

	size_t GetRandomNumber(size_t aMin, size_t aMax);
	void tProduce();
};
