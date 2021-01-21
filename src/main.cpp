#include "main.hpp"
#include <iostream>
#include "Matrix.hpp"

int main(int argc, char* argv[])
{
	
	// ink::Matrix<2,3> A = {
	// 	2, 3, 5,
	// 	7, 2, 4
	// };
	
	// ink::Matrix<3,2> C = {
	// 	 1, 6,
	// 	 7, 2,
	// 	 0, -5
	// };
	
	auto I = ink::Matrix<3,3, int>{
		1, 1, 0,
		1, 1, 1,
		1, 0, 1
	};
	
	//std::cout << (f).fmt() << std::endl;
	
	return 0;
}
