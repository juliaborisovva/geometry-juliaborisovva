#pragma once

typedef enum TypesOfFigures { UNKNOWN, CIRCLE, TRIANGLE } Type;

typedef enum ErrorStatus { SUCCESS = 0, FAILURE = -1 } ErrStatus;

typedef struct {
    float x[4];
    float y[4];
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

typedef struct {
    Type figure;
    union {
        Circle circle;
        Triangle triangle;
    } data;
} Shape;

void find_collisions(
        Shape* shape,
        int figure_counter,
        int collision[figure_counter][figure_counter - 1]);

Type determine_figure(char** cursor_start, char** cursor_end);

ErrStatus parse_circle(
        char** cursor_start,
        char** cursor_end,
        int* figure_counter,
        Shape* shape);

ErrStatus parse_triangle(
        char** cursor_start,
        char** cursor_end,
        int* figure_counter,
        Shape* shape);
