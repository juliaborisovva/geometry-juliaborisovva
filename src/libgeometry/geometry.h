#pragma once
#include <stdio.h>

enum LengthsOfFigures { LENGTH_OF_CIRCLE = 6, LENGTH_OF_TRIANGLE = 8 };

typedef enum TypesOfFigures { UNKNOWN, CIRCLE, TRIANGLE } Type;

typedef enum ErrorStatus { SUCCESS = 0, FAILURE = -1 } ErrStatus;

typedef struct TriangleData {
    float x1;
    float y1;
    float x2;
    float y2;
    float x3;
    float y3;
    float x4;
    float y4;
    float perimeter;
    float area;
} Triangle;

typedef struct CircleData {
    float x1;
    float y1;
    float radius1;
    float perimeter;
    float area;
} Circle;

void skip_space(char** cursor_start);

Type determine_figure(char** cursor_start, char** cursor_end);

ErrStatus parse_circle(
        char** cursor_start,
        char** cursor_end,
        int* counter,
        Circle* Circles,
        int* num);

ErrStatus parse_triangle(
        char** cursor_start,
        char** cursor_end,
        int* counter,
        Triangle* Triangles,
        int* num);
