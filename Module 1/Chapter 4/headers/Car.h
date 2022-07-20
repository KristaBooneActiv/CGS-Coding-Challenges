#pragma once
#include <string>

class Car
{
public:
	Car() = default;
	Car(int aYear,
		int aMiles,
		const std::string& aBrand);
	~Car() = default;

	void Display();
	void Drive(int aMiles);

protected:
	int mYear {0};
	int mMiles {0};
	std::string mBrand {"Unspecified"};
};