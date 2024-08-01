#include <cmath>
#include <iostream>
#include <conio.h>
#include <string.h>
#include <windows.h>
/*


*/
const int CUBE_WIDTH = 10;
const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 44;
const double INCREMENT_SPEED = 0.06;

const char BACKGROUND_CHAR = ' ';
const char MAIN_CHAR = '#';

float A = 0,B = 0,C = 0;

float zBuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
char screenBuffer[SCREEN_WIDTH*SCREEN_HEIGHT];

float calculateX(int j, int k, int i) {
    return j * sin(A) *sin(B) *cos(C) - k * cos(A) * sin(B) * cos(C) +
        j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(int j, int k, int i) {
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) - j * sin(A) *   sin(B) * sin(C) + k * cos(A) *   sin(B) * sin(C) - i * cos(B) * sin(C);
}

float calculateZ(int j, int k, int i) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B)+ i * sin(B);
}


float x,y,z;
void calculateForSurface(float cube_x, float cube_y, float cube_Z, const char str) {
    x = calculateX(cube_x, cube_y, cube_Z);
    y = calculateX(cube_x, cube_y, cube_Z);
    z = calculateZ(cube_x, cube_y, cube_Z) + 40; //DistanceFromViewPoint

    float OutOfZBuffer = 1/z;

    int screenX = (int)(SCREEN_WIDTH/2 + 40*OutOfZBuffer*x*2); //Tweak this 40 value. What does it do?
    int screenY = (int)(SCREEN_HEIGHT/2+ 40*OutOfZBuffer*y);

    int ScreenPositionIndex = screenX + screenY * SCREEN_WIDTH;

    if(ScreenPositionIndex >= 0 && ScreenPositionIndex < SCREEN_WIDTH*SCREEN_HEIGHT) {
        if(OutOfZBuffer > zBuffer[ScreenPositionIndex]) {
            zBuffer[ScreenPositionIndex] = OutOfZBuffer; //Every frame we set back to 0, so it just needs to be greater than 0.
            screenBuffer[ScreenPositionIndex] = str;
        }
    }
}

int main()
{
    while(1) {
        memset(zBuffer, 0, sizeof(zBuffer));
        memset(screenBuffer, BACKGROUND_CHAR, sizeof(screenBuffer));

        for(float cubeX = -CUBE_WIDTH; cubeX < CUBE_WIDTH; cubeX += INCREMENT_SPEED) {
            for(float cubeY = -CUBE_WIDTH; cubeY < CUBE_WIDTH; cubeY += INCREMENT_SPEED) {
                calculateForSurface(cubeX, cubeY, -CUBE_WIDTH, '#');
                calculateForSurface(CUBE_WIDTH, cubeY, cubeX, '.');
            }
        }
        std::cout << "\x1B[2J\x1B[H";
        for(int k = 0; k < SCREEN_HEIGHT*SCREEN_WIDTH; k++) {
            putchar(k % SCREEN_WIDTH ? screenBuffer[k] : 10);
        }
        A += 0.05;
        B += 0.05;
        Sleep(100);
    }
    system("pause");
}



