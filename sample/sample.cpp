
#include <iostream>
#include <fstream>

#include "sudoku_table.h"

int main(int argc, char* argv[]) {

	if (argc < 2) {
		std::cerr << "Please enter a sudoku file." << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream ifs{argv[1]};
	
	SudokuTable table{ifs};

	int x, y, n;
	std::cout << table.to_output();
	std::cout << "x: ";
	std::cin >> x;
	std::cout << "y: ";
	std::cin >> y;
	std::cout << "n: ";
	std::cin >> n;

	while (x > 0 || y > 0 || n != -1){
		table.set(x,y,n);
		std::cout << table.to_output();
		std::cout << "x: ";
		std::cin >> x;
		std::cout << "y: ";
		std::cin >> y;
		std::cout << "n: ";
		std::cin >> n;
	}

	ifs.close(); 
	return EXIT_SUCCESS;
}
