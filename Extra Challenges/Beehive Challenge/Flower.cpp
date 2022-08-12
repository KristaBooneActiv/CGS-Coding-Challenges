#include <mutex>
#include <random>

#include "Flower.h"
#include "StandardOutputForThreads.h"

namespace
{
	class IDGenerator
	{
	public:
		static size_t Get() { return ++mCount; }

	protected:
		static size_t mCount;
	};
	size_t IDGenerator::mCount = 0;
}

static std::mutex sProductAccess;    // Mutex for locking access to the product
static bool sStopProduction = false; // Flag to stop production 
static bool sProduct = false;        // Start with no product available

Flower::Flower(size_t aTimerMin, size_t aTimerMax)
	: mMinTimeToProduce(aTimerMin)
	, mMaxTimeToProduce(aTimerMax)
	, mId(IDGenerator::Get())
{
	mProductionThread = std::thread(&Flower::tProduce, this);
}

Flower::~Flower()
{
	sStopProduction = true;
	mProductionThread.join();
}

void Flower::StopProduction()
{
	sStopProduction = true;
}

Pollen Flower::GetPollen()
{
	std::lock_guard<std::mutex> accessLock(sProductAccess);
	if (sProduct)
	{
		sProduct = false;
		return true;
	}
	else
	{
		return false;
	}
}

void Flower::tProduce()
{
	while (!sStopProduction)
	{
		// Sleep for a random amount of time
		size_t sleepTime_s = GetRandomNumber(mMinTimeToProduce, mMaxTimeToProduce);

		std::stringstream ss;
		ss << "Flower  " << mId << " | Sleeping for " << sleepTime_s << "s" << std::endl;
		WriteOutputChunk(ss.str());

		std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime_s * 1000));

		ss.clear();
		ss << "Flower  " << mId << " | Producing pollen" << std::endl;
		WriteOutputChunk(ss.str());

		std::lock_guard<std::mutex> lock(sProductAccess);
		sProduct = true;
	}
}

size_t Flower::GetRandomNumber(size_t aMin, size_t aMax)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6((int)mMinTimeToProduce, (int)mMaxTimeToProduce);

	// Sleep for a random amount of time
	return dist6(rng);
}
