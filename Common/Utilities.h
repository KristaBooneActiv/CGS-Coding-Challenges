#pragma once
#include <chrono>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace util
{
    // Check if aString is a number.
    static const bool isNumber(const std::string& aString)
    {
        return !aString.empty() &&
            aString.find_first_not_of("0123456789.") == std::string::npos;
    }

    // Check if aString is an integer.
    static const bool isInteger(const std::string& aString)
    {
        return !aString.empty() &&
            aString.find_first_not_of("0123456789") == std::string::npos;
    }

    // Print the contents of a vector separated by a space to an output stream.
    template<typename T>
    static void printVector(std::ostream& aOut, const std::vector<T>& aVec)
    {
        for (const auto& t : aVec)
        {
            aOut << t << " ";
        }
    }

    // Get a number from the user on standard input.
    // Returns empty optional if invalid number supplied.
    template <typename T>
    static std::optional<T> getNumberFromUser()
    {
        std::string userOption;
        std::cin >> userOption;

        std::optional<T> result;
        if (isInteger(userOption))
        {
            result = std::stoi(userOption);
        }
        return result;
    }

    static std::optional<double> getDecimalNumberFromUser()
    {
        std::string userOption;
        std::cin >> userOption;

        std::optional<double> result;
        if (isNumber(userOption))
        {
            result = std::stod(userOption);
        }
        return result;
    }

    static uint32_t getNanos()
    {
        using namespace std::chrono;
        return static_cast<uint32_t>(
            duration_cast<nanoseconds>(
                system_clock::now().time_since_epoch()
                ).count()
            );
    }

}
