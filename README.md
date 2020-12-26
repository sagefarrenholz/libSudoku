# libSudoku
## A Lightweight Sudoku Backend
sudokulib provides a straightforward interface to creating and interacting with sudoku tables: 
* Import sudoku files
* Check the completeness of rows, columns, squares, or the entire table.
* Provide table output via graphical interface or stdout.
## Installing
1. Run `make`, creating the sudoku_table.o intermediate.
2. Add `#include "sudoku_table.h"` to your code.
3. Link against sudoku_table.o
## Building the GUI Application
1. Install Qt
2. After running `make` generating the sudoku_table.o, move into the gui directory and run `qmake`
3. Finally, run the generated make script from Qt. On Unix and MacOS this will just be typing `make` once more.
## Getting Started
Please view sample.cpp in sample/
