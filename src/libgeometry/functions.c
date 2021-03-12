#include "geometry.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static enum LengthsOfFigures { LENGTH_OF_CIRCLE = 6, LENGTH_OF_TRIANGLE = 8 };

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
    char type_circle[] = {"circle"};
    char type_triangle[] = {"triangle"};
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

static float circle_perimeter(Circle Circles)
{
    Circles.perimeter = 2 * M_PI * Circles.radius1;
    return Circles.perimeter;
}

static float circle_area(Circle Circles)
{
    Circles.area = M_PI * pow(Circles.radius1, 2);
    return Circles.area;
}

static void length_of_sides(Triangle Triangles, float* sides)
{
    sides[0]
            = sqrt(pow((Triangles.x2 - Triangles.x1), 2)
                   + pow((Triangles.y2 - Triangles.y1), 2));
    sides[1]
            = sqrt(pow((Triangles.x3 - Triangles.x2), 2)
                   + pow((Triangles.y3 - Triangles.y2), 2));
    sides[2]
            = sqrt(pow((Triangles.x4 - Triangles.x3), 2)
                   + pow((Triangles.y4 - Triangles.y3), 2));
}

static float triangle_perimeter(Triangle Triangles)
{
    int sides_value = 3;
    float sides[sides_value];
    length_of_sides(Triangles, sides);
    float perimeter = 0;
    for (int i = 0; i < 3; i++) {
        perimeter += sides[i];
    }
    return perimeter;
}

static float triangle_area(Triangle Triangles)
{
    int sides_value = 3;
    float sides[sides_value];
    length_of_sides(Triangles, sides);
    float perimeter = triangle_perimeter(Triangles);
    float p = perimeter / 2.0;
    float area = sqrt(p * (p - sides[0]) * (p - sides[1]) * (p - sides[2]));
    return area;
}

ErrStatus parse_circle(
        char** cursor_start,
        char** cursor_end,
        int* counter,
        Circle* Circles,
        int* num)
{
    ErrStatus implementation2;
    float x1, y1, radius1;

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
    Circles[*counter].x1 = x1;
    Circles[*counter].y1 = y1;
    Circles[*counter].radius1 = radius1;
    (*num)++;
    printf("%d Тип фигуры: circle\n", *num);
    printf("x1 = %.1lf y1 = %.1lf \nradius1 = %.1lf\n \n",
           Circles[*counter].x1,
           Circles[*counter].y1,
           Circles[*counter].radius1);
    Circles[*counter].perimeter = circle_perimeter(Circles[*counter]);
    Circles[*counter].area = circle_area(Circles[*counter]);
    printf("Perimeter = %.2lf\n", Circles[*counter].perimeter);
    printf("Area = %.2lf\n\n", Circles[*counter].area);
    (*counter)++;
    return SUCCESS;
}

ErrStatus parse_triangle(
        char** cursor_start,
        char** cursor_end,
        int* counter,
        Triangle* Triangles,
        int* num)
{
    ErrStatus implementation2;
    float coords[8];
    int digit = 0;

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
    Triangles[*counter].x1 = coords[0];
    Triangles[*counter].y1 = coords[1];
    Triangles[*counter].x2 = coords[2];
    Triangles[*counter].y2 = coords[3];
    Triangles[*counter].x3 = coords[4];
    Triangles[*counter].y3 = coords[5];
    Triangles[*counter].x4 = coords[6];
    Triangles[*counter].y4 = coords[7];
    (*num)++;
    printf("%d Тип фигуры: triangle\n", *num);
    printf("x1 = %.1lf y1 = %.1lf\nx2 = %.1lf y2 = %.1lf\nx3 = %.1lf "
           "y3 = %.1lf\nx4 = %.1lf y4 = %.1lf\n\n",
           Triangles[*counter].x1,
           Triangles[*counter].y1,
           Triangles[*counter].x2,
           Triangles[*counter].y2,
           Triangles[*counter].x3,
           Triangles[*counter].y3,
           Triangles[*counter].x4,
           Triangles[*counter].y4);

    Triangles[*counter].perimeter = triangle_perimeter(Triangles[*counter]);
    Triangles[*counter].area = triangle_area(Triangles[*counter]);
    printf("Perimeter = %.2lf\n", Triangles[*counter].perimeter);
    printf("Area = %.2lf\n\n", Triangles[*counter].area);

    (*counter)++;

    return SUCCESS;
}
