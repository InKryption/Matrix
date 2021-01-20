#include "main.hpp"
#include <iostream>
#include "Matrix.hpp"

int main(int argc, char* argv[])
{
	
	ink::Matrix<2,3> A = {
		2, 3, 5,
		7, 2, 4
	};
	
	ink::Matrix<3,2> C = {
		 1, 6,
		 7, 2,
		 0, -5
	};
	
	std::cout << (A * C).fmt() << std::endl;
	
	return 0;
}
