#include <arrayfire.h>
#include <iostream>

int main()
{
	af::array myArray{};

	std::cout << myArray.dims()[0];

	std::cin.get();
}
