TEMPLATE = app
TARGET = Sudoku 
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += test.cpp 
OBJECTS += sudoku_table.o
HEADERS += sudoku_window.h
QMAKE_CXXFLAGS += -std=c++11
