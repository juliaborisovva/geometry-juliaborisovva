#include "geometry.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum LengthsOfFigures { LENGTH_OF_CIRCLE = 6, LENGTH_OF_TRIANGLE = 8 };

static float calculate_length(float x2, float x1, float y2, float y1)
{
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

static void skip_space(char** cursor_start)
{
    while (**cursor_start == ' ') {
        (*cursor_start)++;
    }
}

static ErrStatus check_punctuation_symbols(char** cursor_start, char symbol)
{
    if (**cursor_start == symbol) {
        (*cursor_start)++;
        return SUCCESS;
    } else {
        printf("Error: expected \"%c\"\n\n", symbol);
        return FAILURE;
    }
}

static ErrStatus check_extra_token(char** cursor_start)
{
    while (**cursor_start != '\0') {
        if ((isalnum(**cursor_start) != 0) || (ispunct(**cursor_start) != 0)) {
            printf("Error: expected "
                   "nothing after "
                   "data string\n\n");
            **cursor_start = '\0';
            return FAILURE;
        } else {
            (*cursor_start)++;
        }
    }
    return SUCCESS;
}

/*если встречается буква, то пока она встречается
двигаем указатель end пока не увидим другой символ*/
static void select_type(char** cursor_start, char** cursor_end)
{
    skip_space(cursor_start);
    *cursor_end = *cursor_start;
    while (isalpha(**cursor_end) != 0) {
        (*cursor_end)++;
    }
}

/*Если введенный тип совпал с известными нам типами фигур, то печатаем
тип и выставляем переменной figure соответствующее значение, иначе -
вывести ошибку*/
Type determine_figure(char** cursor_start, char** cursor_end)
{
    char type_circle[] = {"Circle"};
    char type_triangle[] = {"Triangle"};
    select_type(cursor_start, cursor_end);
    size_t length_of_type = *cursor_end - *cursor_start;
    if (strncasecmp(*cursor_start, type_circle, LENGTH_OF_CIRCLE) == 0
        && length_of_type == LENGTH_OF_CIRCLE) {
        return CIRCLE;
    } else if (
            strncasecmp(*cursor_start, type_triangle, LENGTH_OF_TRIANGLE) == 0
            && length_of_type == LENGTH_OF_TRIANGLE) {
        return TRIANGLE;
    } else {
        return UNKNOWN;
    }
}

static float circle_perimeter(Shape shape)
{
    return 2 * M_PI * shape.data.circle.radius1;
}

static float circle_area(Shape shape)
{
    return M_PI * pow(shape.data.circle.radius1, 2);
}

static void calculate_length_of_sides(Shape shape, float* side)
{
    int b = 0;
    for (int i = 0; i < 3; i++) {
        side[i] = calculate_length(
                shape.data.triangle.x[b + 1],
                shape.data.triangle.x[b + 0],
                shape.data.triangle.y[b + 1],
                shape.data.triangle.y[b + 0]);
        b += 1;
    }
}

static float triangle_perimeter(Shape shape)
{
    int sides_value = 3;
    float side[sides_value];
    calculate_length_of_sides(shape, side);
    float perimeter = 0;
    for (int i = 0; i < sides_value; i++) {
        perimeter += side[i];
    }
    return perimeter;
}

static float triangle_area(Shape shape)
{
    int sides_value = 3;
    float side[sides_value];
    calculate_length_of_sides(shape, side);
    float perimeter = triangle_perimeter(shape);
    float p = perimeter / 2.0;
    float area = sqrt(p * (p - side[0]) * (p - side[1]) * (p - side[2]));
    return area;
}

ErrStatus parse_circle(
        char** cursor_start,
        char** cursor_end,
        int* figure_counter,
        Shape* shape)
{
    ErrStatus implementation2;
    float x1, y1, radius1;

    skip_space(cursor_start);
    implementation2 = check_punctuation_symbols(cursor_start, '(');
    if (implementation2)
        return FAILURE;
    *cursor_end = *cursor_start;
    x1 = strtof(*cursor_start, cursor_end);
    if (*cursor_start == *cursor_end) {
        printf("Error: expected float x1\n\n");
        return FAILURE;
    }
    *cursor_start = *cursor_end;
    y1 = strtof(*cursor_start, cursor_end);
    if (*cursor_start == *cursor_end) {
        printf("Error: expected float y1\n\n");
        return FAILURE;
    }
    *cursor_start = *cursor_end;
    skip_space(cursor_start);
    implementation2 = check_punctuation_symbols(cursor_start, ',');
    if (implementation2)
        return FAILURE;

    radius1 = strtof(*cursor_start, cursor_end);
    if (*cursor_start == *cursor_end) {
        printf("Error: expected float radius\n\n");
        return FAILURE;
    }
    *cursor_start = *cursor_end;
    skip_space(cursor_start);
    implementation2 = check_punctuation_symbols(cursor_start, ')');
    if (implementation2)
        return FAILURE;
    implementation2 = check_extra_token(cursor_start);
    if (implementation2)
        return FAILURE;
    shape[*figure_counter].figure = CIRCLE;
    shape[*figure_counter].data.circle.x1 = x1;
    shape[*figure_counter].data.circle.y1 = y1;
    shape[*figure_counter].data.circle.radius1 = radius1;

    shape[*figure_counter].data.circle.perimeter
            = circle_perimeter(shape[*figure_counter]);
    shape[*figure_counter].data.circle.area
            = circle_area(shape[*figure_counter]);
    (*figure_counter)++;

    return SUCCESS;
}

ErrStatus parse_triangle(
        char** cursor_start,
        char** cursor_end,
        int* figure_counter,
        Shape* shape)
{
    ErrStatus implementation2;
    float coords[8];
    int digit = 0;

    skip_space(cursor_start);
    implementation2 = check_punctuation_symbols(cursor_start, '(');
    if (implementation2)
        return FAILURE;
    implementation2 = check_punctuation_symbols(cursor_start, '(');
    if (implementation2)
        return FAILURE;
    *cursor_end = *cursor_start;

    for (int i = 0; i < 8; i++) {
        coords[i] = strtof(*cursor_start, cursor_end);
        char letter = 'y';
        if (i % 2 == 0) {
            letter = 'x';
            digit++;
        }
        if (*cursor_start == *cursor_end) {
            printf("Error: expected float %c%d\n\n", letter, digit);
            return FAILURE;
        }
        *cursor_start = *cursor_end;

        if ((i != 7) && (i % 2 == 1)) {
            skip_space(cursor_start);
            implementation2 = check_punctuation_symbols(cursor_start, ',');
            if (implementation2)
                return FAILURE;
        }
    }
    skip_space(cursor_start);
    implementation2 = check_punctuation_symbols(cursor_start, ')');
    if (implementation2)
        return FAILURE;
    implementation2 = check_punctuation_symbols(cursor_start, ')');
    if (implementation2)
        return FAILURE;
    implementation2 = check_extra_token(cursor_start);
    if (implementation2)
        return FAILURE;
    if ((coords[0] != coords[6]) && (coords[1] != coords[7])) {
        printf("Error: point 1 and point 4 don't match\n\n");
        return FAILURE;
    }

    shape[*figure_counter].figure = TRIANGLE;

    int b = 0;
    for (int d = 0; d < 4; d++) {
        shape[*figure_counter].data.triangle.x[d] = coords[b];
        b++;
        shape[*figure_counter].data.triangle.y[d] = coords[b];
        b++;
    }

    shape[*figure_counter].data.triangle.perimeter
            = triangle_perimeter(shape[*figure_counter]);
    shape[*figure_counter].data.triangle.area
            = triangle_area(shape[*figure_counter]);
    (*figure_counter)++;

    return SUCCESS;
}
