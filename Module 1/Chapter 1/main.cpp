#include <iostream>

int main()
{
	const std::string ln = "--------------------------------------------------------------------------------------------\n";

	std::cout
		<< ln
		<< "Hello, CGS Training Mentor!\n"
		<< "What grade would you like to assign this student? (type 'A', 'B', 'C', 'D', or 'F'): ";

	std::string throwAway;
	std::cin >> throwAway;

	std::cout << "\nYou selected 'A'! The student thanks you." << std::endl;
	std::cout << ln;

	return 0;
}