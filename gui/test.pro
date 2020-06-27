TEMPLATE = app
TARGET = Sudoku 
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += test.cpp sudoku_table.cpp
HEADERS += sudoku_window.h
