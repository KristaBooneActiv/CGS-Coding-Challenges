// Challenge:
// Write a program to find and display the biggest number (element)
// in an array of integers. Hint the size of the array is unimportant
// as long as there's a way to check that the number displayed is
// definitely the biggest number.
#pragma once
#include <algorithm>
#include <iostream>
#include "IChallenge.h"

template <typename T, size_t N>
class Challenge1 : public IChallenge
{
public:
	Challenge1(T(& aArray)[N])
	{
		std::copy(std::begin(aArray), std::end(aArray), std::begin(mArray));
	}

	std::string details() const override
	{
		return "Find and display the biggest number in an array.";
	}

	void run() override
	{
		int largest = largestElement();

		std::cout << "The largest element in the array { ";
		printArray(std::cout);
		std::cout << "} is " << largest << "!" << std::endl;
	}

protected:
	// Find largest element of mArray.
	T largestElement()
	{
		std::sort(mArray, mArray + N);
		return mArray[N - 1];
	}

	// Print out mArray.
	void printArray(std::ostream& aOut)
	{
		for (size_t i = 0; i < N; ++i)
		{
			aOut << mArray[i] << " ";
		}
	}

	// The array
	T mArray[N];
};