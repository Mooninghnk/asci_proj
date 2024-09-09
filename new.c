#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h> // For usleep

#define WIDTH 60
#define HEIGHT 40

typedef struct {
    float x, y, z;
} Vertex;

Vertex cubeVertices[8] = {
    {-1, -1, -1},
    { 1, -1, -1},
    { 1,  1, -1},
    {-1,  1, -1},
    {-1, -1,  1},
    { 1, -1,  1},
    { 1,  1,  1},
    {-1,  1,  1}
};

void initializeGrid(char grid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid[i][j] = ' ';
        }
    }
}

void printGrid(char grid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            putchar(grid[i][j]);
        }
        putchar('\n');
    }
}

void rotateX(Vertex* v, float angle) {
    float y = v->y * cos(angle) - v->z * sin(angle);
    float z = v->y * sin(angle) + v->z * cos(angle);
    v->y = y;
    v->z = z;
}

void rotateY(Vertex* v, float angle) {
    float x = v->x * cos(angle) + v->z * sin(angle);
    float z = -v->x * sin(angle) + v->z * cos(angle);
    v->x = x;
    v->z = z;
}

void rotateZ(Vertex* v, float angle) {
    float x = v->x * cos(angle) - v->y * sin(angle);
    float y = v->x * sin(angle) + v->y * cos(angle);
    v->x = x;
    v->y = y;
}

void project(Vertex v, int* x, int* y) {
    float distance = 2;
    float factor = distance / (distance + v.z);
    *x = (int)(v.x * factor * WIDTH / 2) + WIDTH / 2;
    *y = (int)(v.y * factor * HEIGHT / 2) + HEIGHT / 2;
}

void drawLine(char grid[HEIGHT][WIDTH], int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    while (1) {
        if (x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT) {
            grid[y1][x1] = '#';
        }
        if (x1 == x2 && y1 == y2) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x1 += sx; }
        if (e2 < dy) { err += dx; y1 += sy; }
    }
}

void drawCube(char grid[HEIGHT][WIDTH], Vertex vertices[8]) {
    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };

    for (int i = 0; i < 12; i++) {
        int x1, y1, x2, y2;
        project(vertices[edges[i][0]], &x1, &y1);
        project(vertices[edges[i][1]], &x2, &y2);
        drawLine(grid, x1, y1, x2, y2);
    }
}

int main() {
    char grid[HEIGHT][WIDTH];
    float angleX = 0, angleY = 0, angleZ = 0;

    while (1) {
        initializeGrid(grid);

        Vertex transformedVertices[8];
        for (int i = 0; i < 8; i++) {
            transformedVertices[i] = cubeVertices[i];
            rotateX(&transformedVertices[i], angleX);
            rotateY(&transformedVertices[i], angleY);
            rotateZ(&transformedVertices[i], angleZ);
        }

        drawCube(grid, transformedVertices);
        printGrid(grid);

        angleX += 0.05;
        angleY += 0.05;
        angleZ += 0.05;

        usleep(100000); // Sleep for 100 milliseconds
        printf("\033[H\033[J"); // Clear the screen
    }

    return 0;
}
