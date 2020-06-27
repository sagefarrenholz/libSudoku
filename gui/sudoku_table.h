/*
 * 
 * Sage Farrenholz, 2020
 *
 */

#pragma once

#ifndef SUDOKU_TABLE_H
#define SUDOKU_TABLE_H

#include <vector>
#include <iostream>
#include <string>

// A single cell in a sudoku table
class Cell {
	
	protected:	
		int value;

	public:
		Cell(void); 
		Cell(const int& value);
		
		int get_value(void) const; 
		void set_value(const int& _value);
};

class SudokuTable;

// Shape base class
class Shape {
	
	protected:
		// A vector of pointers to each cell in the sudoku table that this shape references. 
		std::vector<Cell*> cell_vec;
	
		// A lookup vector of the locations of each number within the values array, for instance: location 0 corresponds to location of the number 1 in values.
		std::vector<int> locations;

		// Properties of the shape: origin coordinate, width, and height.
		const int x, y, w, h;

		bool complete;

	public:
		Shape (SudokuTable* table, const int& x, const int& y, const int& width, const int& height);
	
		// Returns the index of the given value in the shape, -1 if not found. 
		int find(const int& n) const;

		// Sets the value at the given index.
		void set(const int& i, const int& n);	

		// Returns if this shape is complete.
		bool is_complete(void); 

		// Returns the sum of all numbers in the shape
		int sum(void);

};

class Square final : public Shape {
	public:
		Square(SudokuTable* table, const int& x, const int& y, const int& dim); 
};

class Row final : public Shape {
	public:
		Row(SudokuTable* table, const int& y);
};

class Column final : public Shape {
	public:
		Column(SudokuTable* table, const int& x);
};

class SudokuTable {

	friend class Shape;

	private: 
		std::vector<Cell> cells;
		int dimension, root;
	
		std::vector<Column> columns;
		std::vector<Row> rows;
		std::vector<Square> squares;

		// Get the root of the table 
		int get_root(void) const; 

		// Initialize shapes of the table 
		void init(void);
	public:
		// Constructs a 9x9 table by default
		SudokuTable(void);
		
		// Construct by dimension
		SudokuTable(const int& x);

		// Construct by reading from stream
		SudokuTable(std::istream& is);

		// Get the dimension of the table
		int get_dimension(void) const;

		// Returns a string of the values of the table. 
		// Each column is seperated by space, each row seperated by a newline.
		std::string to_string(void) const;
	
		// Returns a formatted string utilizes box-drawing unicode characters.	
		std::string to_output(const std::string& title = "") const;

		// Set a cell by index. 0 <= i (index)  < dim^2
		void set(const int& i, const int& n);

		// Set a cell by index. The top-leftmost cell is (1,1)
		void set(const int& x, const int& y, const int& n); 
	
		// Get a cell value by index. 0 corresponds to empty. 	
		int get(const int& i) const;

		// Returns a bool indicating if the table is complete.
		bool is_complete(void);
};

#endif
