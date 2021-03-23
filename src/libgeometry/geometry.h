#pragma once

typedef enum TypesOfFigures { UNKNOWN, CIRCLE, TRIANGLE } Type;

typedef enum ErrorStatus { SUCCESS = 0, FAILURE = -1 } ErrStatus;

typedef struct {
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

typedef struct {
    float x1;
    float y1;
    float radius1;
    float perimeter;
    float area;
} Circle;

Type determine_figure(char** cursor_start, char** cursor_end);

ErrStatus parse_circle(
        char** cursor_start,
        char** cursor_end,
        int* counter,
        Circle* circle,
        int* num);

ErrStatus parse_triangle(
        char** cursor_start,
        char** cursor_end,
        int* counter,
        Triangle* triangle,
        int* num);
