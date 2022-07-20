#include "IChallenge.h"
#include "Utilities.h"

#include <iostream>
#include <memory>
#include <optional>
#include <vector>

class ChallengeRunner
{
public:
	ChallengeRunner() = default;
	~ChallengeRunner() = default;

	void addChallenge(std::unique_ptr<IChallenge>&& aChallenge)
	{
		mChallenges.push_back(std::move(aChallenge));
	}

	std::optional<size_t> getChallengeNumber()
	{
		std::optional<size_t> result;
		std::string userOption;
		std::cin >> userOption;

		if (util::isNumber(userOption))
		{
			size_t option = std::stoi(userOption);
			if (option > 0 && option <= mChallenges.size())
			{
				result = option;
			}
		}
		return result;
	}

	void run()
	{
		bool keepGoing = true;
		while (keepGoing)
		{
			// Have the user select a challenge
			std::cout << "Enter the number of the challenge you want to run:" << std::endl;
			for (size_t i = 0; i < mChallenges.size(); ++i)
			{
				std::cout << "\t" << i + 1 << ": " << mChallenges.at(i)->details() << std::endl;
			}

			std::optional<size_t> challengeNum;
			while (!challengeNum.has_value())
			{
				challengeNum = getChallengeNumber();
				if (!challengeNum.has_value())
				{
					std::cout << "\nBad option. Try again: ";
				}
			}
			std::cin.ignore(); // ensure clear input buffer

			std::cout << "You selected " << challengeNum.value() << ". Running now..." << std::endl << std::endl;
			mChallenges.at(challengeNum.value() - 1)->run();

			std::cout << "\nRun another challenge? Select 'y' for yes or any other key for no: ";
			char keepGoingChar;
			std::cin >> keepGoingChar;
			std::cout << std::endl;

			if (std::tolower(keepGoingChar) != 'y')
			{
				keepGoing = false;
			}
		}
	}

protected:
	std::vector<std::unique_ptr<IChallenge>> mChallenges;
};