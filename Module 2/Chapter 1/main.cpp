// Challenge:
//
// Write a program that asks the user to enter integers to be stored
// in the variables 'a' and 'b'. Then create two integer pointers
// named ptrA and ptrB. Assign the values of 'a' and 'b' to ptrA
// and ptrB, and display them.
#include <iostream>

#include "Utilities.h"

int getValidInteger()
{
	std::optional<int> n;
	while (true)
	{
		n = util::getNumberFromUser<int>();
		if (n.has_value())
		{
			break;
		}
		std::cout << "\nThat's not an integer! Try again: ";
	}
	return n.value();
}

void printPtrData(int* aPtr, char aObjName)
{
	std::cout
		<< "Here is " << aObjName << "Ptr: " << aPtr
		<< " which is set to the address of " << aObjName
		<< ".\nAccessing " << aObjName << " through the pointer, I see that "
		<< aObjName << "=" << *aPtr << std::endl << std::endl;

}

int main()
{
	std::cout << "Hello there! Please type an integer for 'a': ";
	int a = getValidInteger();

	std::cout << "\nGreat! Please type an integer for 'b': ";
	int b = getValidInteger();

	std::cout << "\nThank you! I'm going to create a pointer to these integers and print them out for you...\n" << std::endl;
	int* aPtr = &a;
	int* bPtr = &b;

	printPtrData(aPtr, 'a');
	printPtrData(bPtr, 'b');
}