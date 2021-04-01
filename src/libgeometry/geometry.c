#include "geometry.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum LengthsOfFigures { LENGTH_OF_CIRCLE = 6, LENGTH_OF_TRIANGLE = 8 };

typedef enum CollisionStatus { INTERSECT = 0, DONTINTERSECT = -1 } CollStatus;

static float calculate_length(float x2, float x1, float y2, float y1)
{
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

static void calculate_length_of_sides(Shape shape, float* side)
{
    int b = 0;
    for (int i = 0; i < 3; i++) {
        side[i] = calculate_length(
                shape.data.triangle.x[b + 1],
                shape.data.triangle.x[b],
                shape.data.triangle.y[b + 1],
                shape.data.triangle.y[b]);
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

static CollStatus check_intersect_c(Shape shape1, Shape shape2)
{
    float sum_radius = shape1.data.circle.radius1 + shape2.data.circle.radius1;
    float d = calculate_length(
            shape2.data.circle.x1,
            shape1.data.circle.x1,
            shape2.data.circle.y1,
            shape1.data.circle.y1);
    if (d <= sum_radius) {
        return INTERSECT;
    }
    return DONTINTERSECT;
}

// Пересекаются ли 2 отрезка?
static float find_vector_product(float ax, float ay, float bx, float by)
{ // ax,ay — координаты a bx,by — координаты b
    return ax * by - bx * ay;
}

static CollStatus find_lines_cross(
        float x1,
        float y1,
        float x2,
        float y2,
        float x3,
        float y3,
        float x4,
        float y4)
{ // Пересекаются ли отрезки?
    float v[4];
    v[0] = find_vector_product(x4 - x3, y4 - y3, x1 - x3, y1 - y3);
    v[1] = find_vector_product(x4 - x3, y4 - y3, x2 - x3, y2 - y3);
    v[2] = find_vector_product(x2 - x1, y2 - y1, x3 - x1, y3 - y1);
    v[3] = find_vector_product(x2 - x1, y2 - y1, x4 - x1, y4 - y1);
    if ((v[0] * v[1] <= 0)
        && (v[2] * v[3] <= 0)) { // {v1v2<0 и v3v4<0, отрезки пересекаются}
        return INTERSECT;
    } else {
        return DONTINTERSECT;
    }
}

static CollStatus find_shape_inside(
        float x1,
        float y1,
        float x2,
        float y2,
        float x3,
        float y3,
        float x0,
        float y0)
{
    float v1, v2, v3;
    v1 = find_vector_product((x1 - x0), (y1 - y0), (x2 - x1), (y2 - y1));
    v2 = find_vector_product((x2 - x0), (y2 - y0), (x3 - x2), (y3 - y2));
    v3 = find_vector_product((x3 - x0), (y3 - y0), (x1 - x3), (y1 - y3));
    if (((v1 > 0) && (v2 > 0) && (v3 > 0))
        || ((v1 < 0) && (v2 < 0) && (v3 < 0))) {
        return INTERSECT;
    }
    return DONTINTERSECT;
}

// FIX MEEEEEE пересечение треугольников
static CollStatus check_intersect_t(Shape shape1, Shape shape2)
{
    CollStatus status[2] = {DONTINTERSECT, DONTINTERSECT};
    for (int i = 0; i < 3; i++) {
        for (int k = 0; k < 2; k++) {
            status[k] = find_lines_cross(
                    shape1.data.triangle.x[i],
                    shape1.data.triangle.y[i],
                    shape1.data.triangle.x[i + 1],
                    shape1.data.triangle.y[i + 1],
                    shape2.data.triangle.x[k],
                    shape2.data.triangle.y[k],
                    shape2.data.triangle.x[k + 1],
                    shape2.data.triangle.y[k + 1]);
        }
        if (status[0] == INTERSECT || status[1] == INTERSECT) {
            return INTERSECT;
        }
    }
    Shape choice1 = shape1;
    Shape choice2 = shape2;
    for (int k = 0; k < 2; k++) {
        if (k == 1) {
            choice1 = shape2;
            choice2 = shape1;
        }
        status[k] = find_shape_inside(
                choice1.data.triangle.x[0],
                choice1.data.triangle.y[0],
                choice1.data.triangle.x[1],
                choice1.data.triangle.y[1],
                choice1.data.triangle.x[2],
                choice1.data.triangle.y[2],
                choice2.data.triangle.x[0],
                choice2.data.triangle.y[0]);
    }
    if (status[0] == INTERSECT || status[1] == INTERSECT) {
        return INTERSECT;
    }
    return DONTINTERSECT;
};

static float
find_distance(float x1, float x2, float y1, float y2, float x0, float y0)
{
    float side_length = calculate_length(x2, x1, y2, y1);
    Shape triangle;

    float coords[8] = {x0, y0, x1, y1, x2, y2, x0, y0};
    int b = 0;
    for (int d = 0; d < 4; d++) {
        triangle.data.triangle.x[d] = coords[b];
        b++;
        triangle.data.triangle.y[d] = coords[b];
        b++;
    }
    float area = triangle_area(triangle);
    return (2.0 * area) / side_length;
};

static CollStatus check_intersect_c_t(Shape circle, Shape triangle)
{
    // 1 расстояние от центра до одной из вершин <= radius
    /*float center_to_vertex[3];
    for (int i = 0; i < 3; i++) {
        center_to_vertex[i] = calculate_length(
                triangle.data.triangle.x[i],
                circle.data.circle.x1,
                triangle.data.triangle.y[i],
                circle.data.circle.y1);
        if (center_to_vertex[i] <= circle.data.circle.radius1) {
            return INTERSECT;
        }
    }*/
    // нормаль к прямой Ах + Ву = 0
    // вектор с координатами (-B, A)
    //
    // 2 расстояние от центра окружности до одной из 3 прямых <= radius
    // проверить принадлежит ли ближайшая точка отрезку
    for (int i = 0; i < 3; i++) {
        float distance = find_distance(
                triangle.data.triangle.x[i],
                triangle.data.triangle.x[i + 1],
                triangle.data.triangle.y[i],
                triangle.data.triangle.y[i + 1],
                circle.data.circle.x1,
                circle.data.circle.y1);
        if (distance <= circle.data.circle.radius1) {
            float circle_start
                    = circle.data.circle.x1 - circle.data.circle.radius1;
            float circle_end
                    = circle.data.circle.x1 + circle.data.circle.radius1;
            float line_start, line_end;
            if (triangle.data.triangle.x[i] < triangle.data.triangle.x[i + 1]) {
                line_start = triangle.data.triangle.x[i];
                line_end = triangle.data.triangle.x[i + 1];
            } else {
                line_start = triangle.data.triangle.x[i + 1];
                line_end = triangle.data.triangle.x[i];
            }
            if ((circle_start <= line_end) && (circle_end >= line_start)) {
                return INTERSECT;
            }
        }
    }
    // 3 центр окружности лежит внутри треугольника
    CollStatus status = find_shape_inside(
            triangle.data.triangle.x[0],
            triangle.data.triangle.y[0],
            triangle.data.triangle.x[1],
            triangle.data.triangle.y[1],
            triangle.data.triangle.x[2],
            triangle.data.triangle.y[2],
            circle.data.circle.x1,
            circle.data.circle.y1);
    if (status == INTERSECT) {
        return INTERSECT;
    }
    return DONTINTERSECT;
}

void find_collisions(
        Shape* shape,
        int figure_counter,
        int collision[figure_counter][figure_counter - 1])
{
    int status;

    for (int i = 0; i < figure_counter; i++) {
        int cols = 0;
        for (int m = 0; m < figure_counter; m++) {
            if (m != i) {
                if (shape[i].figure == CIRCLE && shape[m].figure == CIRCLE) {
                    status = check_intersect_c(shape[i], shape[m]);
                    if (status == INTERSECT) {
                        collision[i][cols] = m;
                        cols++;
                    }
                } else if (
                        shape[i].figure == CIRCLE
                        && shape[m].figure == TRIANGLE) {
                    status = check_intersect_c_t(shape[i], shape[m]);
                    if (status == INTERSECT) {
                        collision[i][cols] = m;
                        cols++;
                    }
                } else if (
                        shape[i].figure == TRIANGLE
                        && shape[m].figure == CIRCLE) {
                    status = check_intersect_c_t(shape[m], shape[i]);
                    if (status == INTERSECT) {
                        collision[i][cols] = m;
                        cols++;
                    }
                } else if (
                        shape[i].figure == TRIANGLE
                        && shape[m].figure == TRIANGLE) {
                    status = check_intersect_t(shape[i], shape[m]);
                    if (status == INTERSECT) {
                        collision[i][cols] = m;
                        cols++;
                    }
                }
            }
        }
    }
};

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
