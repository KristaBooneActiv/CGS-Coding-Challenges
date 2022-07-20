#include "Challenge1.h"
#include "Utilities.h"

#include <cassert>
#include <iostream>
#include <sstream>

Challenge1::Challenge1(int numNumbersToUse)
	: mNumNumbers(numNumbersToUse)
{
}

double Challenge1::computeSum(const std::vector<double>& aNumbers) const
{
    assert(aNumbers.size() >= mNumNumbers);

    double sum = 0;
    for (size_t i = 0; i < mNumNumbers; ++i)
    {
        sum += aNumbers.at(i);
    }
    return sum;
}

double Challenge1::computeAverage(const std::vector<double>& aNumbers) const
{
    assert(aNumbers.size() >= mNumNumbers);
    auto sum = computeSum(aNumbers);
    return sum / (double)mNumNumbers;
}

void Challenge1::parseCin(std::vector<double>& aNumbers) const
{
    std::string inputStr;
    std::getline(std::cin, inputStr);

    std::stringstream ss(inputStr);
    std::string numStr;
    while (ss >> numStr)
    {
        if (util::isNumber(numStr))
        {
            aNumbers.push_back(std::stod(numStr));
        }
    }
}

void Challenge1::printNumbers(std::ostream& aOut, const std::vector<double>& aNumbers) const
{
    assert(aNumbers.size() >= mNumNumbers);
    for (size_t i = 0; i < mNumNumbers; ++i)
    {
        aOut << aNumbers.at(i) << " ";
    }
}

std::string Challenge1::details() const
{
    return "Computes the Sum and Average of " + std::to_string(mNumNumbers) + " numbers";
}

void Challenge1::run()
{
    // Prompt user for input
    std::cout << "Hello, there! Enter " << mNumNumbers
        << " numbers with spaces between (e.g. \"1 2 3\"): ";

    // Grab numbers from standard input
    std::vector<double> numbers;
    parseCin(numbers);

    // Gather additional numbers if the user submitted fewer than necessary.
    while (numbers.size() < mNumNumbers)
    {
        std::cout << "Hmm, I don't have enough numbers... Give me "
            << mNumNumbers - numbers.size() << " more: ";
        parseCin(numbers);
    }

    // Print warning if the user submitted more than necessary.
    if (numbers.size() > mNumNumbers)
    {
        std::cout << "Ope! Looks like you gave me too many numbers. I'll use the first "
            << mNumNumbers << " numbers." << std::endl;
    }

    // Regurgitate numbers back to user for clarity.
    std::cout << "\nGreat, thanks! You gave me: ";
    printNumbers(std::cout, numbers);

    // Output sum, average.
    // If using more numbers, we could optimize by storing the intermediate sum,
    // but for the sake of syntactic simplicity, we separate into solo functions.
    std::cout << "\nLet me crunch some numbers..." << std::endl << std::endl;
    std::cout << "Sum:     " << computeSum(numbers) << std::endl;
    std::cout << "Average: " << computeAverage(numbers) << std::endl;
}
