default:
	g++ --std=c++17 -c sudoku_table.cpp 
	cp sudoku_table.o gui
clean:
	rm *.o
