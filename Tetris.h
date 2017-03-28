/*

    File: tetris.h
    Author: Zane Christenson
    Date: 6/10/16
    Version: 1.0.3

*/

#ifndef TETRIS_H
#define TETRIS_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Text.hpp>

#include <iostream>     //cout
#include <vector>       //vector
#include <string>       //string
#include <sstream>      //ostringstream
#include <cmath>        //abs
#include <time.h>       //rand, srand


// Constants
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600


// Global Block Location
int blockRow;
int blockCol;
bool gameRunning;
bool paused;
int score;

sf::Text scoreString;

// Global Board
std::vector<int> row(10, 0);
std::vector< std::vector<int> > board(20, row);


// Function Declaration Prototypes

void printTable();
void newBlock();
void spawnNewBlock();
void blockDrop();
void moveLeft();
void moveRight();
void rotateBlock();
void removeRow(int);
void checkForCompleteRow();
void gameOver();
void newGame();
void incrementScore();




#endif // TETRIS_H
