//============================================================================
// Name        : Tetris.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Tetris...
//============================================================================
#ifndef TETRIS_CPP_
#define TETRIS_CPP_
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include<string>
#include <cstdlib>
#include"util.h"
#include <iostream>
#include <fstream>
using namespace std;
const int B_S = 60;
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
int score = 0;
int timer = 0;
int snake_length = 5;
int pX[60] = { 20, 19, 18, 17, 16 }, pY[60] = { 20, 20, 20, 20, 20 };
int board[B_S][B_S];
int hurdleX, hurdleY;
int pFY;
int pFX;
bool snakedie = false;
enum Direction { UP, DOWN, LEFT, RIGHT };
Direction snakeDirection = RIGHT;
int fX[5], fY[5];
int hT = 0;  
const int hCI = 15000; 
int pFT = 0;
int headX, headY = 0;
void SetCanvasSize(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1); 
    //glOrtho(0,B_S,B_S, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void WSTF(int score)
{
    ofstream file("scores.txt", std::ios::app); // Open the file in append mode
    if (file.is_open())
    {
        file << score << std::endl; // Write the score to the file
        file.close(); // Close the file
    }
    else
    {
       cout << "Unable to open the score file." << std::endl;
    }
}
int FindHighestScore()
{
    ifstream file("scores.txt");
    if (file.is_open())
    {
        int highestScore = 0;
        int score;
        while (file >> score)
        {
            if (score > highestScore)
            {
                highestScore = score;
            }
        }
        file.close(); // Close the file
        return highestScore;
    }
    else
    {
       cout << "Unable to open the score file." << std::endl;
        return 0;
    }
}
void gH()
{
    // Reset all existing hurdles
    for (int i = 0; i <B_S; i++)
    {
        for (int j = 0; j <B_S; j++)
        {
            if (board[i][j] == 4)
                board[i][j] = 0;
        }
    }

    // Generate random position for the vertical line of hurdles
     hurdleX = rand() % (B_S); // Random X position
     hurdleY = rand() % (B_S); // Random Y position
}
void gPFT()
{
    for (int i = 0; i <B_S; i++)
    {
        for (int j = 0; j <B_S; j++)
        {
            if (board[i][j] == 20)
            {
                board[i][j] = 0;
            }
        }
    }
        pFX = rand() %B_S;
        pFY = rand() %B_S;

}
void dS()
{
    int prevX = pX[0];
    int prevY = pY[0];
    for (int i = 1; i < snake_length; i++)
    {
        int tempX = pX[i];
        int tempY = pY[i];
        pX[i] = prevX;
        pY[i] = prevY;
        prevX = tempX;
        prevY = tempY;
    }

     headX = pX[0]; // Store the X coordinate of the head
     headY = pY[0]; // Store the Y coordinate of the head

    if (snakeDirection == UP && snakeDirection != DOWN)
    {
        headY--;
        if (headY < 0)
            headY =B_S - 1;
    }
    else if (snakeDirection == DOWN && snakeDirection != UP)
    {
        headY++;
        if (headY >=B_S)
            headY = 0;
    }
    else if (snakeDirection == RIGHT && snakeDirection != LEFT)
    {
        headX++;
        if (headX >=B_S)
            headX = 0;
    }
    else if (snakeDirection == LEFT && snakeDirection != RIGHT)
    {
        headX--;
        if (headX < 0)
            headX =B_S - 1;
    }

    if (headX == pFX && headY == pFY)
    {
        snake_length++;
        score += 20;
        gPFT();
    }

    

    if (board[headY][headX] == 4)
    {
        // Game over logic
        cout << "Game Over - Snake hit a hurdle" << std::endl;
        WSTF(score); // Write the score to the file
        exit(0);
    }

    pX[0] = headX; // Update the X coordinate of the head in the array
    pY[0] = headY; // Update the Y coordinate of the head in the array

    // Update the board array with the snake's positions
    for (int i = 0; i <B_S; i++)
    {
        for (int j = 0; j <B_S; j++)
        {
            board[i][j] = 0; // Reset the board to empty spaces
        }
    }

    for (int i = 0; i < snake_length; i++)
    {
        int x = pX[i];
        int y = pY[i];
        if (i == 0)
            board[y][x] = 1; // Set the board position to indicate the snake's head
        else
            board[y][x] = 3; // Set the board position to indicate the snake's body
    }

    
}
void gF()
{
   for (int i = 0; i <B_S; i++)
    {
        for (int j = 0; j <B_S; j++)
        {
            if (board[i][j] == 2)
                board[i][j] = 0;
        }
    }
    fX[0] = rand() %B_S;
    fY[0] = rand() %B_S;
    for (int i = 1; i < 5; i++)
    {
        while (true)
        {
            fX[i] = rand() %B_S;
            fY[i] = rand() %B_S;

            bool validP = true;

         
            for (int j = 0; j < i; j++)
            {
                if (fX[i] == fX[j] && fY[i] == fY[j])
                {
                    validP = false;
                    break;
                }
            }

            // Check for overlapping positions along rows, columns, and diagonals
            for (int j = 0; j < i; j++)
            {
                if (fX[i] == fX[j] || fY[i] == fY[j] || abs(fX[i] - fX[j]) == abs(fY[i] - fY[j]))
                {
                    validP= false;
                    break;
                }
            }

            if (validP)
                break;
        }
    }
}
void Display() {
    glClearColor(1, 0, 0.0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    int X1 = -10;
    int Y2 = 600;
    for (int i = 0; i < 60; i++) {
        for (int j = 0; j < 60; j++) {
            X1 = X1 + 10;
            if (X1 >= 0 && X1 <= 600 && Y2 >= 0 && Y2 <= 600) {
                if (board[i][j] == 1) {
                    DrawSquare(X1, Y2, 10, colors[BLUE]);
                }
                if (board[i][j] == 3) {
                    DrawSquare(X1, Y2, 10, colors[GREEN]);
                }

                else if (board[i][j] == 2) {
                    DrawSquare(X1, Y2, 10, colors[BLACK]);
                }
                else if (board[i][j] == 20) {
                    DrawSquare(X1, Y2, 20, colors[YELLOW]);
                }
                else if (board[i][j] == 4) {
                    DrawSquare(X1, Y2, 10, colors[BROWN]);
                }
            }
        }

        X1 = 60;
        Y2 = Y2 - 10;
    }

    glColor3f(1.0f, 3.0f, 1.0f);
    glRasterPos2f(250, 100); // Adjusted position for score text
    std::string scoreT = "Score: " + std::to_string(score);
    for (char c : scoreT)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    glRasterPos2f(250, 590); // Adjusted position for timer text
    std::string timerT = "Timer: " + std::to_string(timer / 1000) + "s";
    for (char c : timerT)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    glutSwapBuffers();
}
void update(int value)
{
    dS();
    for (int i = 0; i <B_S; i++)
    {
        for (int j = 0; j <B_S; j++)
        {
            board[i][j] = 0;
        }
    }
    pFT += 100;  // Increase the power food timer by 100 milliseconds
    if (pFT >= 30000)  // Check if 30 seconds have passed
    {
        pFT = 0;  // Reset the power food timer
        gPFT();  // Generate a new position for the power food
    }
    hT += 100;  // Increase the timer by 100 milliseconds
    if (hT >= hCI)
    {
        hT = 0;  // Reset the hurdle timer
        gH();  // Change the hurdle position
    }
    for (int i = 0; i < snake_length; i++)
    {
        int x = pX[i];
        int y = pY[i];
        if (i == 0)
            board[y][x] = 1; // Set the board position to indicate the snake's head
        else
            board[y][x] = 3; // Set the board position to indicate the snake's body
    }
    
    for (int i = hurdleY; i > 0; i--)
    {

        if (board[i][hurdleX] != 20 && 2 && 1)
        {
            board[i][hurdleX] = 4;
        }
    }
    for (int i = 0; i < 5; i++)
    {
        if (board[fY[i]][fX[i]] != 4 && 20 && 1)
        {
            board[fY[i]][fX[i]] = 2;

        }
        if (board[pFY][pFX] != 20)
        {
            board[pFY][pFX] = 20;
        }
    }
        bool isFoodCollision = false;
        for (int i = 0; i < 5; i++)
        {
            if (pX[0] == fX[i] && pY[0] == fY[i])
            {
                // Snake ate the food
                isFoodCollision = true;
                snake_length++;
                score += 5; // Increase the score by 5
                gF();
                break;
            }
        }
        if (pX[0] == pFX && pY[0] == pFY)
        {
            snake_length++;
            score += 20;
            gPFT();
        }
        timer += 100; // Increase the timer by 100 milliseconds
   
    glutPostRedisplay();
    glutTimerFunc(100, update, 0);
}
void handleKeypress(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
        snakeDirection = UP;
        break;
    case 's':
        snakeDirection = DOWN;
        break;
    case 'd':
        snakeDirection = RIGHT;
        break;
    case 'a':
        snakeDirection = LEFT;
        break;
    }
}
int main(int argc, char** argv)
{
   
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   // glutInitWindowPosition(50, 50);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Snake Game");
    SetCanvasSize(600, 600);
    glutDisplayFunc(Display);
    glutTimerFunc(100, update, 0);
    glutKeyboardFunc(handleKeypress);
    glewInit();
    gF();
    gH();
    gPFT();
  //  glewinit();
    glutMainLoop();

    return 0;
}
#endif /* Snake Game */



