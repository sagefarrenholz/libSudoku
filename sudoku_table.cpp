/*
 * 
 * Sage Farrenholz, 2020
 *
 */

#include "sudoku_table.h"

// Implementation required includes

#include <cmath>
#include <cctype>
#include <algorithm>
#include <fstream>

/* 
 * Cell Function Definitions
 */

Cell::Cell(void) : value(0) {};
Cell::Cell(const int& value) : value(value) {};		
		
int Cell::get_value(void) const { return value; };
void Cell::set_value(const int& _value) { value = _value; };	

/*
 * Shape Function Defintions
 */	
Shape::Shape (SudokuTable* table, const int& x, const int& y, const int& width, const int& height) : complete(false), x(x), y(y), w(width), h(height) {

	if (table == nullptr) {
		std::cerr << "Error: Shape \"table\" parameter cannot be NULL.";
		exit(EXIT_FAILURE);
	}

	auto dim = table->get_dimension();

	if (y + h > dim || x + w > dim) {
		std::cerr << "Error: Shape exceeds the bounds of the sudoku table.";
		exit(EXIT_FAILURE);
	} 
	
	auto& cells = table->cells;			

	// Allocate memory for values and locations arrays	
	cell_vec.reserve(w * h);
	locations.reserve(w * h);
			
	// Loop through the given table copying pointers to the values in the table into the shape
	for (int row = 0; row < height; row++){
		for (int col = 0; col < width; col++){
			cell_vec.push_back(&cells.at(x + col + dim * (y + row)));
		}
	} 	

	// Fills locations with the location values of each number pointed to by values	
	/*for (int i = 0, j = 0; i < w * h; i++, j = 0) {
		while (cell_vec.at(j)->get_value() != i && j < w * h) j++;
		locations.push_back(-1);
	}*/
}
		
// Returns the index of the given value in the shape, -1 if not found. 
int Shape::find(const int& n) const {
	return locations.at(n-1);
}

// Sets the value at the given index.
void Shape::set(const int& i, const int& n){
	
}

// Returns if this shape is complete.
bool Shape::is_complete(void) { return complete; } 


// Returns the sum of all numbers in the shape
int Shape::sum(void) { 
	int sum = 0;
	std::for_each(cell_vec.begin(), cell_vec.end(), [&](auto* cell){sum += cell->get_value();});
	return sum;
}

/*
 * Shape Implementations
 */

Square::Square(SudokuTable* table, const int& x, const int& y, const int& dim) : Shape(table, x, y, dim, dim) {}

Row::Row(SudokuTable* table, const int& y) : Shape(table, 0, y, table->get_dimension(), 1) {}

Column::Column(SudokuTable* table, const int& x) : Shape(table, x, 0, 1, table->get_dimension()) {}

/* 
 * SudokuTable Function Definitions
 */

int SudokuTable::get_dimension(void) const { return dimension; }
int SudokuTable::get_root(void) const { return root; }

// Initalize table shapes
void SudokuTable::init(void) {
	const int dim = get_dimension();

	// Allocate memory within shape vectors
	rows.reserve(dim);
	columns.reserve(dim);
	squares.reserve(dim);
	
	// Initialize rows
	for (int r = 0; r < dim; r++) rows.push_back(Row(this, r));
	
	// Initialize columns
	for (int c = 0; c < dim; c++) columns.push_back(Column(this, c));

	// Initialize squares
	for (int r = 0; r < get_root(); r++) {
		for (int c = 0; c < get_root(); c++){
			squares.push_back(Square(this, c * get_root(), r * get_root(), get_root()));
		}
	}
}

// Constructs a 9x9 table by default
SudokuTable::SudokuTable(void) : dimension(9), root(3) {
	cells.resize(81);
	init();
}
		
// Construct by dimension
SudokuTable::SudokuTable(const int& x) : dimension(x), root((int) sqrt(get_dimension())) {
	cells.resize(x * x);
	init();
}

// Construct by reading from stream
SudokuTable::SudokuTable(std::istream& is) {
	char c; int count = 0;
	is.get(c);
	while(is.good()){
		if (isdigit(c)) {
			count++;
			cells.push_back(c - 48);
		} else if (c == '.' || c == ' ' ) {
			count++;
			cells.push_back(0);
		} else if (c != '\n') {
			break;
		}
		is.get(c);	
	}
	
	double rt = sqrt(count);
	if (rt - (int) rt == 0) { 
		dimension = (int) rt;
		root = (int) sqrt(get_dimension());
	} else {
		throw "Failed to create sudoku table from stream. Dimension must have square root.";
	}
	init();
}

// Output through stream
void SudokuTable::output(std::ostream& os){
	// If dimension is 0 (unitialized table), this won't run
	int val, dim = get_dimension();
	for(int i = 0; i < dim && os.good(); i++){
		for(int j = 0; j < dim && os.good(); j++){
			val = get(i * dim + j);
			if (val > 0){
				// conversion of val to ascii encoded num 
				os.put('0' + val);
			} else {
				os.put('.');
			}
		}
		os.put('\n');
	} 
}

std::string SudokuTable::to_string(void) const { 
	std::string concat = ""; int wrap = 0; 
	std::for_each(cells.begin(), cells.end(), [&](const Cell& c){
		concat += std::to_string(c.get_value());
		if (++wrap != get_dimension()) {
			concat += " ";
		} else {
			concat += "\n";
			wrap = 0;
		}
	});
	return concat;
}
		
std::string SudokuTable::to_output(const std::string& title) const {
	std::string concat = "";
	concat += "\u250F"; // corner pipe top left
	for (int bar = 0; bar < get_dimension() * 4 - 1; bar++) { 
		if ((bar + 1) % 4 == 0) {
			if ((bar + 1) % (get_root()) == 0)  
				concat += "\u2533";
			else 
				concat += "\u252F";
		} else concat += "\u2501";
	}
	concat += "\u2513\n"; // corner pipe top right
	for (int row = 0; row < get_dimension(); row++){
		concat += "\u2503";
		for (int col = 0; col < get_dimension(); col++){
			if (cells.at(col + row * get_dimension()).get_value()) 
				concat += " " + std::to_string(cells.at(col + row * get_dimension()).get_value()) + " "; 
			else 
				concat += "   ";
			if (col < get_dimension() - 1) {
				if ((col + 1) % 3 == 0) {
					concat += "\u2503";
				} else 
					concat += "\u2502";
			}
		}
		concat += "\u2503\n";
		if ( row < get_dimension() - 1) {
			if ( (row + 1) % 3 == 0) {
				// Heavy
				concat += "\u2523";
				for (int bar = 0; bar < get_dimension() * 4 - 1; bar++) { 
					if ((bar + 1) % 4 == 0) {
						if ((bar + 1) % (get_root()) == 0)  
							concat += "\u254B";
						else 
							concat += "\u253F";
					} else concat += "\u2501";
				}
				concat += "\u252B\n"; 
			} else {
				concat += "\u2520";
				for (int bar = 0; bar < get_dimension() * 4 - 1; bar++) { 
					if ((bar + 1) % 4 == 0) {
						if ((bar + 1) % (get_root() * 4) == 0)  
							concat += "\u2542";
						else 
							concat += "\u253C";
					} else concat += "\u2500";
				}
				concat += "\u2528\n"; 
			}	
		}

	}
	concat += "\u2517"; // corner pipe top left
	for (int bar = 0; bar < get_dimension() * 4 - 1; bar++) { 
		if ((bar + 1) % 4 == 0) {
			if ((bar + 1) % (get_root()) == 0)  
				concat += "\u253B";
			else 
				concat += "\u2537";
		} else concat += "\u2501";
	}
	concat += "\u251B\n"; // corner pipe top right
	return concat;
}

void SudokuTable::set(const int& i, const int& n){
	if (n <= get_dimension()) cells.at(i) = n;
	else std::cout << "Cannot set a cell to: " << n << ". Greater than the dimension of the table." << std::endl;
}

void SudokuTable::set(const int& x, const int& y, const int& n) { set(x - 1 + (y - 1) * get_dimension(), n); }
		
int SudokuTable::get(const int& i) const { return cells.at(i).get_value(); }

bool SudokuTable::is_complete(void) { 
	// TODO impelement 
	return false; 
}

SudokuTable SudokuTable::open(const char* file){
	std::ifstream ifs{file};
	SudokuTable table{ifs};
	ifs.close();
	return table;
}
