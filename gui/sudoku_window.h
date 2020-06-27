/*
 * =====================================================================================
 *
 *       Filename:  sudoku_window.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/25/2020 09:35:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */
#pragma once
#include <string>
#include <iostream>
#include <fstream>

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QGridLayout>
#include <QIntValidator>
#include <QObject>

#include "sudoku_table.h"

enum mode { READ_ONLY, READ_WRITE };

class SudokuMenuBar : public QMenuBar {
	public:
		SudokuMenuBar(QWidget* parent = nullptr) : QMenuBar(parent) {	}
	
};

class SudokuCell : public QLineEdit {
	public:
		mode cellmode;

		void set_mode(mode cellmode_){
			cellmode = cellmode_;
			switch(cellmode){
				case READ_ONLY:
					setEnabled(false);
					break;
				case READ_WRITE:
					setEnabled(true);
					break;
			}
		}	

		SudokuCell(QWidget* parent = nullptr, int length = 1, mode cellmode = READ_WRITE) : QLineEdit(parent) {
			setValidator(new QIntValidator(1, 9, this));
			setMaximumSize(40, 40);
			setAlignment(Qt::AlignCenter);
			setTextMargins(0,0,0,0);
			setMaxLength(length);
			
			set_mode(cellmode);
		}

		void set_display_value(int val) {
			setText(QString::number(val));		
		}

		void enable(){set_mode(READ_WRITE);}
		void disable(){set_mode(READ_ONLY);}

};

class SudokuAbout : public QTextEdit {
	public:
		SudokuAbout(QWidget* parent = nullptr) : QTextEdit(parent) {
			

		}
};

class SudokuWindow : public QMainWindow {

	Q_OBJECT	
	SudokuMenuBar* menu;
	QGridLayout* grid_layout;	

	// dimensions of the grid layout
	int dim;
	// Path and basename (eg. ./out.su)
	std::string filename;

	// points to a table which acts as the model
	SudokuTable* table;

	// Whether or not the table can be written to
	mode tablemode;

	public:
		SudokuWindow(int dimension = 9, mode tablemode = READ_WRITE) : QMainWindow(), dim(dimension) {

			// Create MenuBar
			menu = new SudokuMenuBar;
			QMenu* file_menu = menu->addMenu("File");
			QAction* open = file_menu->addAction("Open");		
			QAction* save = file_menu->addAction("Save");		
			QAction* save_as = file_menu->addAction("Save As");		

			// Save buttons are not clickable until a table is open		
			save->setEnabled(false);
			save_as->setEnabled(false);

			// Set save button names
			save->setObjectName("save");
			save_as->setObjectName("save_as");

			// Add about 
			QAction* about = menu->addAction("About");

			// Add menu to window
			setMenuBar(menu);
			
			// File Signals
			connect(open, SIGNAL(triggered(bool)), this, SLOT(open_file()));	
			connect(save, SIGNAL(triggered(bool)), this, SLOT(save_file()));	
			connect(save_as, SIGNAL(triggered(bool)), this, SLOT(save_as_file()));	

			// About Signal
			connect(about, SIGNAL(triggered(bool)), this, SLOT(open_about()));

			// Grid Layout
			QWidget* cell_box = new QWidget();
			grid_layout = new QGridLayout();		
			grid_layout->setSpacing(0);
			grid_layout->setContentsMargins(0, 0, 0, 0);
			
			// Grid Layout Creation
			QWidget* cell;
			for (int row = 0; row < dim; row++){
				for (int col = 0; col < dim; col++){
					cell = new SudokuCell;
					grid_layout->addWidget(cell, row, col);	
					cell->setObjectName(QString::fromStdString(std::to_string(row*dim+col)));
				//	connect(cell, SIGNAL(textEdited(const QString&), this));
				}
			}

			// Set Central Widget layout
			setCentralWidget(cell_box);
			centralWidget()->setLayout(grid_layout);

			// TODO add scaling
			setFixedSize(500, 500);
		}
	
		SudokuWindow(SudokuTable* table_, mode tablemode = READ_WRITE) : SudokuWindow(table_->get_dimension(), tablemode) {
			attach_table(table_);
		}


	public slots:
		void open_file() {
			QString file = QFileDialog::getOpenFileName(nullptr, "Open a Sudoku File", ".");
			if (!file.isNull()) {
				std::ifstream ifs(file.toStdString().c_str());
				attach_table(new SudokuTable(ifs));
				enable_saving();
			}
		}

		void save_as_file() {
			QString file = QFileDialog::getSaveFileName(nullptr, "Save as", "./out.su");
			filename = file.toStdString();
			// CALL SOME FILE SAVING ROUTINE HERE
		}

		void save_file() {
			if(filename.empty()) {
				save_as_file();
			}	
			// CALL SOME FILE SAVING ROUTINE HERE
		}

		void open_about() {
			SudokuAbout* about = new SudokuAbout; 
			about->setReadOnly(true);
			about->setText("Sage Farrenholz (C) 2020");		
			about->setFixedSize(200,100);
			about->setWindowTitle("About");
			about->show();
		}
		void enable_saving() {
			findChild<QAction*>("save")->setEnabled(true);
			findChild<QAction*>("save_as")->setEnabled(true);
		}
			
		void set_cell(int idx, int val){
			findChild<SudokuCell*>(QString::number(idx))->set_display_value(val);
		}
	private:
		void attach_table(SudokuTable* table_){
			table = table_;
			SudokuCell* cell;
			for (int i = 0; i < table->get_dimension() * table->get_dimension(); i++){
				if (!table->get(i)) continue;	
				cell = findChild<SudokuCell*>(QString::number(i));
				cell->set_display_value(table->get(i));
				cell->disable();

			}

		}
};


