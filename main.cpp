#include <cmath>
#include <iostream>
#include <conio.h>
#include <string.h>
#include <windows.h>
/*
\begin{pmatrix}i&j&k\end{pmatrix}\begin{pmatrix}1&0&0\\ \:\:\:\:0&cos\left(X\right)&-sin\left(X\right)\\ \:\:\:\:0&sin\left(X\right)&cos\left(X\right)\end{pmatrix}\begin{pmatrix}cos\left(Y\right)&0&sin\left(Y\right)\\ \:\:\:\:0&1&0\\ \:\:\:\:-sin\left(Y\right)&0&cos\left(Y\right)\end{pmatrix}\begin{pmatrix}cos\left(Z\right)&-sin\left(Z\right)&0\\ \:\:\:\:sin\left(Z\right)&cos\left(Z\right)&0\\ \:\:\:\:0&0&1\end{pmatrix}
*/
const int CUBE_WIDTH = 10;
const int SCREEN_WIDTH = 110;
const int SCREEN_HEIGHT = 44;
const double INCREMENT_SPEED = 0.6;

const char BACKGROUND_CHAR = ' ';
const char MAIN_CHAR = '#';

float Xadvance,YAdvance,ZAdvance;

struct Vec3 {
    float x,y,z;
};

struct ScreenPosition {
    int x, y, idx;
};

Vec3 threeDCoordinate;
Vec3 threeDTranslation;
ScreenPosition ScreenPosition;

float zBuffer[SCREEN_WIDTH*SCREEN_HEIGHT*4];
char screenBuffer[SCREEN_WIDTH*SCREEN_HEIGHT];

float calculate_x_rotation(int x, int y, int z) {
    return y * sin(Xadvance) * sin(YAdvance) * cos(ZAdvance) - z * cos(Xadvance) * sin(YAdvance) * cos(ZAdvance) +
           y * cos(Xadvance) * sin(ZAdvance) + z * sin(Xadvance) * sin(ZAdvance) + x * cos(YAdvance) * cos(ZAdvance);
}

float calculate_y_rotation(int i, int j, int k) {
    return j * cos(Xadvance) * cos(ZAdvance) + k * sin(Xadvance) * cos(ZAdvance) -
           j * sin(Xadvance) * sin(YAdvance) * sin(ZAdvance) + k * cos(Xadvance) * sin(YAdvance) * sin(ZAdvance) -
           i * cos(YAdvance) * sin(ZAdvance);}

float calculate_z_rotation(int i, int j, int k) {
  return k * cos(Xadvance) * cos(YAdvance) - j * sin(Xadvance) * cos(YAdvance) + i * sin(YAdvance);
}

void calculate_new_xyz_rotation_for_pixel(float cube_x, float cube_y, float cube_Z) {
    threeDCoordinate = {
        calculate_x_rotation(cube_x, cube_y, cube_Z),
        calculate_y_rotation(cube_x, cube_y, cube_Z),
        calculate_z_rotation(cube_x, cube_y, cube_Z) + 60
    };
}
void calculate_new_xyz_translation_for_pixel(Vec3 transform) {
    threeDCoordinate.x += transform.x;
    threeDCoordinate.y += transform.y;
    threeDCoordinate.z += transform.z;
}

void transform_pixel_to_to_screen_position(float DistanceFromZ, int screen_width, int screen_height) {
    int x = static_cast<int>(screen_width / 2 + 40 * DistanceFromZ * threeDCoordinate.x);
    int y = static_cast<int>(screen_height / 2 + 40 * DistanceFromZ * threeDCoordinate.y);
    int idx = x + y * screen_width;
    ScreenPosition = {
        x,
        y,
        idx
    };

}

void calculate_for_surface(float cube_x, float cube_y, float cube_Z, const char str) {
    calculate_new_xyz_rotation_for_pixel(cube_x, cube_y, cube_Z);
    calculate_new_xyz_translation_for_pixel(threeDTranslation);
    transform_pixel_to_to_screen_position(1/threeDCoordinate.z, SCREEN_WIDTH, SCREEN_HEIGHT);

    if(ScreenPosition.idx >= 0 && ScreenPosition.idx < (SCREEN_WIDTH*SCREEN_HEIGHT)) {
        if(1/threeDCoordinate.z > zBuffer[ScreenPosition.idx]) {
            zBuffer[ScreenPosition.idx] = 1/threeDCoordinate.z; //Every frame we set back to 0, so it just needs to be greater than 0.
            screenBuffer[ScreenPosition.idx] = str;
        }
    }
}

void advance_by_unit_vector(float x, float y, float z) {
    Xadvance += x;
    YAdvance += y;
    ZAdvance += z;
}

int main()
{
    while(1) {
        //reset buffers
        memset(screenBuffer, BACKGROUND_CHAR, sizeof(screenBuffer));
        memset(zBuffer, 0, sizeof(zBuffer));

        //Cube Math
        for(float cubeX = -CUBE_WIDTH; cubeX < CUBE_WIDTH; cubeX += INCREMENT_SPEED) { //ForEach "pixel" from left to right
            for(float cubeY = -CUBE_WIDTH; cubeY < CUBE_WIDTH; cubeY += INCREMENT_SPEED) { //ForEach "pixel" from top to bottom
                calculate_for_surface(cubeX, cubeY, -CUBE_WIDTH, '#' ); //Perform calculations for each surface of the cube
                calculate_for_surface(-cubeX, cubeY, CUBE_WIDTH, '.' );
                calculate_for_surface(CUBE_WIDTH, cubeY, cubeX, '$' );
                calculate_for_surface(-CUBE_WIDTH, cubeY, -cubeX, '%' );
                calculate_for_surface(cubeX, CUBE_WIDTH, cubeY, '@' );
                calculate_for_surface(cubeX, -CUBE_WIDTH, -cubeY, '&' );
            }
        }
        std::cout << "\x1b[H";
        for(int k = 0; k < SCREEN_HEIGHT*SCREEN_WIDTH; k++) {
            putchar(k % SCREEN_WIDTH ? screenBuffer[k] : 10); //If k % SCREEN_WIDTH is not 0, print the character, else print a new line.
        }
        advance_by_unit_vector(0.1,0.1,0.1);
        threeDTranslation = { (float)sin(Xadvance),(float)sin(Xadvance),0.0f};
        Sleep(100);
    }
    system("pause");
}



