/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/25/2020 09:18:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author: Sage Farrenholz (), 
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <QApplication>
#include <QPushButton>
#include <QWidget>

#include "sudoku_window.h"

int main(int argc, char **argv){
	QApplication app(argc, argv);
	SudokuWindow sw;
	sw.show();
	return app.exec();	
}
