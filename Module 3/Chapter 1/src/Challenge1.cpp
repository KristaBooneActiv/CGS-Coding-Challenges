#include <random>
#include "Challenge1.h"
#include "Utilities.h"

std::string Challenge1::details() const
{
	return "Profiles the QuickSort sorting algorithm on a user-defined sized array of random numbers.";
}

void Challenge1::run()
{
	std::cout << "Profiling the QuickSort sorting algorithm..." << std::endl;
	std::cout << "========================================================================\n";

	const bool cDEBUG_PRINT = false;
	const int cNUM_ITER = 1000;
	runMultiProfile(10, cNUM_ITER, cDEBUG_PRINT);
	runMultiProfile(100, cNUM_ITER, cDEBUG_PRINT);
	runMultiProfile(1000, cNUM_ITER, cDEBUG_PRINT);
	runMultiProfile(10000, cNUM_ITER, cDEBUG_PRINT);
}

void Challenge1::runMultiProfile(int aNumElements, int aNumTimes, bool aPrintDebugFlag)
{
	std::cout << "Averaging performace of " << aNumTimes
		<< " runs with " << aNumElements << " elements..." << std::endl;

	uint32_t avg = 0;
	for (size_t i = 0; i < aNumTimes; ++i)
	{
		avg += runSingleProfile(aNumElements, aPrintDebugFlag);
	}
	avg /= aNumTimes;

	std::cout << "Runtime Average: " << avg/(1000.) << "ms" << std::endl << std::endl;
}

uint32_t Challenge1::runSingleProfile(int aNumElements, bool aPrintDebugFlag)
{
	// Create and fill array with aNumElements number of random numbers.
	int* arr = new int[aNumElements];
	fillArray(arr, aNumElements);

	if (aPrintDebugFlag)
	{
		std::cout << "Before sort: ";
		printArray(arr, aNumElements);
		std::cout << std::endl;
	}

	// Run sorting algorithm, and return time it takes to execute.
	auto startTimeNS = util::getNanos();
	quickSort(arr, 0, aNumElements-1);
	auto endTimeNS = util::getNanos();

	if (aPrintDebugFlag)
	{
		std::cout << "After sort: ";
		printArray(arr, aNumElements);
		std::cout << std::endl;
	}

	delete[] arr;
	return endTimeNS - startTimeNS;
}

void Challenge1::quickSort(int aArray[], int aLow, int aHigh)
{
	// If the lower index is less than the higher index
	if (aLow < aHigh)
	{
		// Pick a pivot index (aHigh), put it in the correct
		// position in the array, then move smaller elements
		// to the left, and bigger elements to the right.
		int p = partition(aArray, aLow, aHigh);

		// Sort elements before the partition
		quickSort(aArray, aLow, p - 1);

		// Sort elements after the partition
		quickSort(aArray, p + 1, aHigh);
	}
}

int Challenge1::partition(int aArray[], int aLow, int aHigh)
{
	// Choose the last element as the pivot (could choose lowest, random, or some other #)
	int pivot = aArray[aHigh];
	// Index of the smaller element
	int minIdx = aLow - 1;

	for (int currIdx = aLow; currIdx < aHigh; ++currIdx)
	{
		if (aArray[currIdx] < pivot)
		{
			// If current element is smaller than the pivot, swap
			// smaller element with current element and increment minIdx.
			++minIdx;
			
			// Swap elements
			int temp = aArray[minIdx];
			aArray[minIdx] = aArray[currIdx];
			aArray[currIdx] = temp;
		}
	}

	// Swap min with pivot
	++minIdx;

	int temp = aArray[minIdx];
	aArray[minIdx] = aArray[aHigh];
	aArray[aHigh] = temp;

	return minIdx;
}

void Challenge1::printArray(int aArray[], int aNumElements)
{
	std::cout << "[ ";
	for (size_t i = 0; i < aNumElements; ++i)
	{
		std::cout << aArray[i] << " ";
	}
	std::cout << "] ";
}

void Challenge1::fillArray(int aArray[], int aNumElements)
{
	std::mt19937 rng(rand());
	std::uniform_int_distribution<int> gen(0, aNumElements);

	for (size_t i = 0; i < aNumElements; ++i)
	{
		aArray[i] = gen(rng);
	}
}