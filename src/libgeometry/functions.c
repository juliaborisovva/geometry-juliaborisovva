#include "geometry.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum LengthsOfFigures { LENGTH_OF_CIRCLE = 6, LENGTH_OF_TRIANGLE = 8 };

void skip_space(char** cursor_start)
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

static float circle_perimeter(Circle circle)
{
    return 2 * M_PI * circle.radius1;
}

static float circle_area(Circle circle)
{
    return M_PI * pow(circle.radius1, 2);
}

static void length_of_sides(Triangle triangle, float* sides)
{
    sides[0]
            = sqrt(pow((triangle.x2 - triangle.x1), 2)
                   + pow((triangle.y2 - triangle.y1), 2));
    sides[1]
            = sqrt(pow((triangle.x3 - triangle.x2), 2)
                   + pow((triangle.y3 - triangle.y2), 2));
    sides[2]
            = sqrt(pow((triangle.x4 - triangle.x3), 2)
                   + pow((triangle.y4 - triangle.y3), 2));
}

static float triangle_perimeter(Triangle triangle)
{
    int sides_value = 3;
    float sides[sides_value];
    length_of_sides(triangle, sides);
    float perimeter = 0;
    for (int i = 0; i < 3; i++) {
        perimeter += sides[i];
    }
    return perimeter;
}

static float triangle_area(Triangle triangle)
{
    int sides_value = 3;
    float sides[sides_value];
    length_of_sides(triangle, sides);
    float perimeter = triangle_perimeter(triangle);
    float p = perimeter / 2.0;
    float area = sqrt(p * (p - sides[0]) * (p - sides[1]) * (p - sides[2]));
    return area;
}

ErrStatus parse_circle(
        char** cursor_start,
        char** cursor_end,
        int* counter,
        Circle* circle,
        int* num)
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
    //убрать избыточные ветвления, создать функцию, которая возвращает нам тру
    //или фолс, и сразу писать ретёрн ()
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
    circle[*counter].x1 = x1;
    circle[*counter].y1 = y1;
    circle[*counter].radius1 = radius1;
    (*num)++;
    printf("%d Тип фигуры: Circle\n", *num);
    printf("x1 = %.1lf y1 = %.1lf \nradius1 = %.1lf\n \n",
           circle[*counter].x1,
           circle[*counter].y1,
           circle[*counter].radius1);
    circle[*counter].perimeter = circle_perimeter(circle[*counter]);
    circle[*counter].area = circle_area(circle[*counter]);
    printf("Perimeter = %.2lf\n", circle[*counter].perimeter);
    printf("Area = %.2lf\n\n", circle[*counter].area);
    (*counter)++;
    return SUCCESS;
}

ErrStatus parse_triangle(
        char** cursor_start,
        char** cursor_end,
        int* counter,
        Triangle* triangle,
        int* num)
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
    triangle[*counter].x1 = coords[0];
    triangle[*counter].y1 = coords[1];
    triangle[*counter].x2 = coords[2];
    triangle[*counter].y2 = coords[3];
    triangle[*counter].x3 = coords[4];
    triangle[*counter].y3 = coords[5];
    triangle[*counter].x4 = coords[6];
    triangle[*counter].y4 = coords[7];
    (*num)++;
    printf("%d Тип фигуры: Triangle\n", *num);
    printf("x1 = %.1lf y1 = %.1lf\nx2 = %.1lf y2 = %.1lf\nx3 = %.1lf "
           "y3 = %.1lf\nx4 = %.1lf y4 = %.1lf\n\n",
           triangle[*counter].x1,
           triangle[*counter].y1,
           triangle[*counter].x2,
           triangle[*counter].y2,
           triangle[*counter].x3,
           triangle[*counter].y3,
           triangle[*counter].x4,
           triangle[*counter].y4);

    triangle[*counter].perimeter = triangle_perimeter(triangle[*counter]);
    triangle[*counter].area = triangle_area(triangle[*counter]);
    printf("Perimeter = %.2lf\n", triangle[*counter].perimeter);
    printf("Area = %.2lf\n\n", triangle[*counter].area);

    (*counter)++;

    return SUCCESS;
}
