#include <ctest.h>
#include <libgeometry/geometry.h>
#define EMPTY 11

CTEST(determine_figure, circle_figure)
{
    char str[] = {"circle (23 5, 3)\n"};
    char* cursor_start = str;
    char* cursor_end = str;

    int real = determine_figure(&cursor_start, &cursor_end);

    int exp = CIRCLE;

    ASSERT_EQUAL(exp, real);
}

CTEST(determine_figure, triangle_figure)
{
    char str[] = {"triangle ((23 5, 25 10, 14 9, 23 5))\n"};
    char* cursor_start = str;
    char* cursor_end = str;

    int real = determine_figure(&cursor_start, &cursor_end);

    int exp = TRIANGLE;

    ASSERT_EQUAL(exp, real);
}

CTEST(determine_figure, incorrect_figure_1)
{
    char str[] = {"c (23 5, 3)\n"};
    char* cursor_start = str;
    char* cursor_end = str;

    int real = determine_figure(&cursor_start, &cursor_end);

    int exp = UNKNOWN;

    ASSERT_EQUAL(exp, real);
}

CTEST(determine_figure, incorrect_figure_2)
{
    char str[] = {"8circle (23 5, 3)\n"};
    char* cursor_start = str;
    char* cursor_end = str;

    int real = determine_figure(&cursor_start, &cursor_end);

    int exp = UNKNOWN;

    ASSERT_EQUAL(exp, real);
}

CTEST(determine_figure, incorrect_figure_3)
{
    char str[] = {"triannngle ((23 5, 25 10, 14 9, 23 5))\n"};
    char* cursor_start = str;
    char* cursor_end = str;

    int real = determine_figure(&cursor_start, &cursor_end);

    int exp = UNKNOWN;

    ASSERT_EQUAL(exp, real);
}

CTEST(parse_circle, correct)
{
    char str[] = {"(23 5, 3)\n"};
    char* cursor_start = str;
    char* cursor_end = str;
    int figure_counter = 0;
    Shape circle[1];

    int real
            = parse_circle(&cursor_start, &cursor_end, &figure_counter, circle);

    int exp = SUCCESS;

    ASSERT_EQUAL(exp, real);
}

CTEST(parse_circle, miss_left_bracket)
{
    char str[] = {"23 5, 3)\n"};
    char* cursor_start = str;
    char* cursor_end = str;
    int figure_counter = 0;
    Shape circle[1];

    int real
            = parse_circle(&cursor_start, &cursor_end, &figure_counter, circle);

    int exp = LBRACKET;

    ASSERT_EQUAL(exp, real);
}

CTEST(parse_circle, wrong_coord_x)
{
    char str[] = {"(x23 5, 3)\n"};
    char* cursor_start = str;
    char* cursor_end = str;
    int figure_counter = 0;
    Shape circle[1];

    int real
            = parse_circle(&cursor_start, &cursor_end, &figure_counter, circle);

    int exp = X1;

    ASSERT_EQUAL(exp, real);
}

CTEST(parse_circle, wrong_coord_y)
{
    char str[] = {"(23 x5, 3)\n"};
    char* cursor_start = str;
    char* cursor_end = str;
    int figure_counter = 0;
    Shape circle[1];

    int real
            = parse_circle(&cursor_start, &cursor_end, &figure_counter, circle);

    int exp = Y1;

    ASSERT_EQUAL(exp, real);
}

CTEST(parse_circle, miss_comma)
{
    char str[] = {"(23 5 3)\n"};
    char* cursor_start = str;
    char* cursor_end = str;
    int figure_counter = 0;
    Shape circle[1];

    int real
            = parse_circle(&cursor_start, &cursor_end, &figure_counter, circle);

    int exp = COMMA;

    ASSERT_EQUAL(exp, real);
}

CTEST(parse_circle, wrong_radius)
{
    char str[] = {"(23 5, x3)\n"};
    char* cursor_start = str;
    char* cursor_end = str;
    int figure_counter = 0;
    Shape circle[1];

    int real
            = parse_circle(&cursor_start, &cursor_end, &figure_counter, circle);

    int exp = RADIUS1;

    ASSERT_EQUAL(exp, real);
}

CTEST(parse_circle, miss_right_bracket)
{
    char str[] = {"(23 5, 3\n"};
    char* cursor_start = str;
    char* cursor_end = str;
    int figure_counter = 0;
    Shape circle[1];

    int real
            = parse_circle(&cursor_start, &cursor_end, &figure_counter, circle);

    int exp = RBRACKET;

    ASSERT_EQUAL(exp, real);
}

CTEST(parse_circle, extra_token)
{
    char str[] = {"(23 5, 3)  extraa\n"};
    char* cursor_start = str;
    char* cursor_end = str;
    int figure_counter = 0;
    Shape circle[1];

    int real
            = parse_circle(&cursor_start, &cursor_end, &figure_counter, circle);

    int exp = EXTRATOKEN;

    ASSERT_EQUAL(exp, real);
}

CTEST(parse_triangle, correct)
{
    char str[] = {"((23 5, 25 10, 14 9, 23 5))\n"};
    char* cursor_start = str;
    char* cursor_end = str;
    int figure_counter = 0;
    Shape triangle[1];

    int real = parse_triangle(
            &cursor_start, &cursor_end, &figure_counter, triangle);

    int exp = SUCCESS;

    ASSERT_EQUAL(exp, real);
}

CTEST(parse_triangle, miss_left_bracket)
{
    char str[] = {"(23 5, 25 10, 14 9, 23 5))\n"};
    char* cursor_start = str;
    char* cursor_end = str;
    int figure_counter = 0;
    Shape triangle[1];

    int real = parse_triangle(
            &cursor_start, &cursor_end, &figure_counter, triangle);

    int exp = LBRACKET;

    ASSERT_EQUAL(exp, real);
}

CTEST(parse_triangle, wrong_coord_x)
{
    char str[] = {"((23 5, x25 10, 14 9, 23 5))\n"};
    char* cursor_start = str;
    char* cursor_end = str;
    int figure_counter = 0;
    Shape triangle[1];

    int real = parse_triangle(
            &cursor_start, &cursor_end, &figure_counter, triangle);

    int exp = X2;

    ASSERT_EQUAL(exp, real);
}

CTEST(parse_triangle, wrong_coord_y)
{
    char str[] = {"((23 5, 25 10, 14 x9, 23 5))\n"};
    char* cursor_start = str;
    char* cursor_end = str;
    int figure_counter = 0;
    Shape triangle[1];

    int real = parse_triangle(
            &cursor_start, &cursor_end, &figure_counter, triangle);

    int exp = Y3;

    ASSERT_EQUAL(exp, real);
}

CTEST(parse_triangle, miss_comma)
{
    char str[] = {"((23 5, 25 10 14 9, 23 5))\n"};
    char* cursor_start = str;
    char* cursor_end = str;
    int figure_counter = 0;
    Shape triangle[1];

    int real = parse_triangle(
            &cursor_start, &cursor_end, &figure_counter, triangle);

    int exp = COMMA;

    ASSERT_EQUAL(exp, real);
}

CTEST(parse_triangle, match_points)
{
    char str[] = {"((23 5, 25 10, 14 9, 26 8))\n"};
    char* cursor_start = str;
    char* cursor_end = str;
    int figure_counter = 0;
    Shape triangle[1];

    int real = parse_triangle(
            &cursor_start, &cursor_end, &figure_counter, triangle);

    int exp = DONTMATCH;

    ASSERT_EQUAL(exp, real);
}

CTEST(parse_triangle, miss_right_bracket)
{
    char str[] = {"((23 5, 25 10, 14 9, 23 5)\n"};
    char* cursor_start = str;
    char* cursor_end = str;
    int figure_counter = 0;
    Shape triangle[1];

    int real = parse_triangle(
            &cursor_start, &cursor_end, &figure_counter, triangle);

    int exp = RBRACKET;

    ASSERT_EQUAL(exp, real);
}

CTEST(parse_triangle, extra_token)
{
    char str[] = {"((23 5, 25 10, 14 9, 23 5))  extraa\n"};
    char* cursor_start = str;
    char* cursor_end = str;
    int figure_counter = 0;
    Shape triangle[1];

    int real = parse_triangle(
            &cursor_start, &cursor_end, &figure_counter, triangle);

    int exp = EXTRATOKEN;

    ASSERT_EQUAL(exp, real);
}

CTEST(find_collisions, tt_intersect_1_touch)
{
    int figure_counter = 0;
    Shape shape[2];
    float coords[] = {1, 1, 3, 4, 3, 1, 1, 1};
    float coords2[] = {3, 1, 5, 4, 5, 1, 3, 1};

    shape[figure_counter].figure = TRIANGLE;
    int b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords[b];
        b++;
    }
    figure_counter++;

    shape[figure_counter].figure = TRIANGLE;
    b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords2[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords2[b];
        b++;
    }
    figure_counter++;

    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = EMPTY;
        }
    }
    find_collisions(shape, figure_counter, collision);

    int real = collision[0][0];
    int real2 = collision[1][0];

    int exp = 1;
    int exp2 = 0;

    ASSERT_EQUAL(exp, real);
    ASSERT_EQUAL(exp2, real2);
}

CTEST(find_collisions, tt_intersect_2_one_vertex_inside)
{
    int figure_counter = 0;
    Shape shape[2];
    float coords[] = {8, 2, 6, 5, 8, 5, 8, 2};
    float coords2[] = {6, 1, 9, 4, 9, 1, 6, 1};

    shape[figure_counter].figure = TRIANGLE;
    int b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords[b];
        b++;
    }
    figure_counter++;

    shape[figure_counter].figure = TRIANGLE;
    b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords2[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords2[b];
        b++;
    }
    figure_counter++;

    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = EMPTY;
        }
    }
    find_collisions(shape, figure_counter, collision);

    int real = collision[0][0];
    int real2 = collision[1][0];

    int exp = 1;
    int exp2 = 0;

    ASSERT_EQUAL(exp, real);
    ASSERT_EQUAL(exp2, real2);
}

CTEST(find_collisions, tt_intersect_3_inside_collision_big_t_first)
{
    int figure_counter = 0;
    Shape shape[2];
    float coords[] = {11, 0, 13, 5, 15, 0, 11, 0};
    float coords2[] = {12, 1, 13, 3, 13, 1, 12, 1};

    shape[figure_counter].figure = TRIANGLE;
    int b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords[b];
        b++;
    }
    figure_counter++;

    shape[figure_counter].figure = TRIANGLE;
    b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords2[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords2[b];
        b++;
    }
    figure_counter++;

    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = EMPTY;
        }
    }
    find_collisions(shape, figure_counter, collision);

    int real = collision[0][0];
    int real2 = collision[1][0];

    int exp = 1;
    int exp2 = 0;

    ASSERT_EQUAL(exp, real);
    ASSERT_EQUAL(exp2, real2);
}

CTEST(find_collisions, tt_intersect_4_inside_collision_small_t_first)
{
    int figure_counter = 0;
    Shape shape[2];
    float coords[] = {12, 1, 13, 3, 13, 1, 12, 1};
    float coords2[] = {11, 0, 13, 5, 15, 0, 11, 0};

    shape[figure_counter].figure = TRIANGLE;
    int b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords[b];
        b++;
    }
    figure_counter++;

    shape[figure_counter].figure = TRIANGLE;
    b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords2[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords2[b];
        b++;
    }
    figure_counter++;

    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = EMPTY;
        }
    }
    find_collisions(shape, figure_counter, collision);

    int real = collision[0][0];
    int real2 = collision[1][0];

    int exp = 1;
    int exp2 = 0;

    ASSERT_EQUAL(exp, real);
    ASSERT_EQUAL(exp2, real2);
}

CTEST(find_collisions, tt_intersect_5_without_vertices_inside)
{
    int figure_counter = 0;
    Shape shape[2];
    float coords[] = {2, 0, 2, 4, 4, 2, 2, 0};
    float coords2[] = {3, 0, 1, 2, 3, 4, 3, 0};

    shape[figure_counter].figure = TRIANGLE;
    int b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords[b];
        b++;
    }
    figure_counter++;

    shape[figure_counter].figure = TRIANGLE;
    b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords2[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords2[b];
        b++;
    }
    figure_counter++;

    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = EMPTY;
        }
    }
    find_collisions(shape, figure_counter, collision);

    int real = collision[0][0];
    int real2 = collision[1][0];

    int exp = 1;
    int exp2 = 0;

    ASSERT_EQUAL(exp, real);
    ASSERT_EQUAL(exp2, real2);
}

CTEST(find_collisions, tt_dont_intersect_6)
{
    int figure_counter = 0;
    Shape shape[2];
    float coords[] = {1, 1, 3, 4, 3, 1, 1, 1};
    float coords2[] = {6, 3, 6, 6, 8, 3, 6, 3};

    shape[figure_counter].figure = TRIANGLE;
    int b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords[b];
        b++;
    }
    figure_counter++;

    shape[figure_counter].figure = TRIANGLE;
    b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords2[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords2[b];
        b++;
    }
    figure_counter++;

    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = EMPTY;
        }
    }
    find_collisions(shape, figure_counter, collision);

    int real = collision[0][0];
    int real2 = collision[1][0];

    int exp = EMPTY;
    int exp2 = EMPTY;

    ASSERT_EQUAL(exp, real);
    ASSERT_EQUAL(exp2, real2);
}

CTEST(find_collisions, cc_intersect_1)
{
    int figure_counter = 0;
    Shape shape[2];
    float data[] = {4, 2, 1, 5, 2, 1};

    int b = 0;
    for (figure_counter = 0; figure_counter < 2; figure_counter++) {
        shape[figure_counter].figure = CIRCLE;
        shape[figure_counter].data.circle.x1 = data[b];
        b++;
        shape[figure_counter].data.circle.y1 = data[b];
        b++;
        shape[figure_counter].data.circle.radius1 = data[b];
        b++;
    }

    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = EMPTY;
        }
    }
    find_collisions(shape, figure_counter, collision);

    int real = collision[0][0];
    int real2 = collision[1][0];

    int exp = 1;
    int exp2 = 0;

    ASSERT_EQUAL(exp, real);
    ASSERT_EQUAL(exp2, real2);
}

CTEST(find_collisions, cc_intersect_2_touch)
{
    int figure_counter = 0;
    Shape shape[2];
    float data[] = {4, 2, 1, 6, 2, 1};

    int b = 0;
    for (figure_counter = 0; figure_counter < 2; figure_counter++) {
        shape[figure_counter].figure = CIRCLE;
        shape[figure_counter].data.circle.x1 = data[b];
        b++;
        shape[figure_counter].data.circle.y1 = data[b];
        b++;
        shape[figure_counter].data.circle.radius1 = data[b];
        b++;
    }

    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = EMPTY;
        }
    }
    find_collisions(shape, figure_counter, collision);

    int real = collision[0][0];
    int real2 = collision[1][0];

    int exp = 1;
    int exp2 = 0;

    ASSERT_EQUAL(exp, real);
    ASSERT_EQUAL(exp2, real2);
}

CTEST(find_collisions, cc_dont_intersect_3)
{
    int figure_counter = 0;
    Shape shape[2];
    float data[] = {4, 2, 1, 1, 4, 1};

    int b = 0;
    for (figure_counter = 0; figure_counter < 2; figure_counter++) {
        shape[figure_counter].figure = CIRCLE;
        shape[figure_counter].data.circle.x1 = data[b];
        b++;
        shape[figure_counter].data.circle.y1 = data[b];
        b++;
        shape[figure_counter].data.circle.radius1 = data[b];
        b++;
    }

    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = EMPTY;
        }
    }
    find_collisions(shape, figure_counter, collision);

    int real = collision[0][0];
    int real2 = collision[1][0];

    int exp = EMPTY;
    int exp2 = EMPTY;

    ASSERT_EQUAL(exp, real);
    ASSERT_EQUAL(exp2, real2);
}

CTEST(find_collisions, ct_intersect_1_t_inside_c)
{
    int figure_counter = 0;
    Shape shape[2];
    float data[] = {2, 3, 3};
    float coords[] = {1, 2, 3, 5, 3, 2, 1, 2};

    int b = 0;
    shape[figure_counter].figure = CIRCLE;
    shape[figure_counter].data.circle.x1 = data[b];
    b++;
    shape[figure_counter].data.circle.y1 = data[b];
    b++;
    shape[figure_counter].data.circle.radius1 = data[b];
    b++;
    figure_counter++;

    shape[figure_counter].figure = TRIANGLE;
    b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords[b];
        b++;
    }
    figure_counter++;

    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = EMPTY;
        }
    }

    find_collisions(shape, figure_counter, collision);

    int real = collision[0][0];
    int real2 = collision[1][0];

    int exp = 1;
    int exp2 = 0;

    ASSERT_EQUAL(exp, real);
    ASSERT_EQUAL(exp2, real2);
}

CTEST(find_collisions, ct_intersect_2_one_vertex_inside)
{
    int figure_counter = 0;
    Shape shape[2];
    float data[] = {9, 4, 2};
    float coords[] = {6, 3, 6, 6, 8, 3, 6, 3};

    int b = 0;
    shape[figure_counter].figure = CIRCLE;
    shape[figure_counter].data.circle.x1 = data[b];
    b++;
    shape[figure_counter].data.circle.y1 = data[b];
    b++;
    shape[figure_counter].data.circle.radius1 = data[b];
    b++;
    figure_counter++;

    shape[figure_counter].figure = TRIANGLE;
    b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords[b];
        b++;
    }
    figure_counter++;

    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = EMPTY;
        }
    }

    find_collisions(shape, figure_counter, collision);

    int real = collision[0][0];
    int real2 = collision[1][0];

    int exp = 1;
    int exp2 = 0;

    ASSERT_EQUAL(exp, real);
    ASSERT_EQUAL(exp2, real2);
}

CTEST(find_collisions, ct_intersect_3_no_vertices_inside)
{
    int figure_counter = 0;
    Shape shape[2];
    float data[] = {15, 5, 2};
    float coords[] = {11, 1, 13, 6, 15, 1, 11, 1};

    int b = 0;
    shape[figure_counter].figure = CIRCLE;
    shape[figure_counter].data.circle.x1 = data[b];
    b++;
    shape[figure_counter].data.circle.y1 = data[b];
    b++;
    shape[figure_counter].data.circle.radius1 = data[b];
    b++;
    figure_counter++;

    shape[figure_counter].figure = TRIANGLE;
    b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords[b];
        b++;
    }
    figure_counter++;

    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = EMPTY;
        }
    }

    find_collisions(shape, figure_counter, collision);

    int real = collision[0][0];
    int real2 = collision[1][0];

    int exp = 1;
    int exp2 = 0;

    ASSERT_EQUAL(exp, real);
    ASSERT_EQUAL(exp2, real2);
}

CTEST(find_collisions, ct_intersect_4_c_inside_t)
{
    int figure_counter = 0;
    Shape shape[2];
    float data[] = {9, 4, 2};
    float coords[] = {5, 1, 9, 10, 13, 2, 5, 1};

    int b = 0;
    shape[figure_counter].figure = CIRCLE;
    shape[figure_counter].data.circle.x1 = data[b];
    b++;
    shape[figure_counter].data.circle.y1 = data[b];
    b++;
    shape[figure_counter].data.circle.radius1 = data[b];
    b++;
    figure_counter++;

    shape[figure_counter].figure = TRIANGLE;
    b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords[b];
        b++;
    }
    figure_counter++;

    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = EMPTY;
        }
    }

    find_collisions(shape, figure_counter, collision);

    int real = collision[0][0];
    int real2 = collision[1][0];

    int exp = 1;
    int exp2 = 0;

    ASSERT_EQUAL(exp, real);
    ASSERT_EQUAL(exp2, real2);
}

CTEST(find_collisions, ct_error_intersect_5)
{
    int figure_counter = 0;
    Shape shape[2];
    float data[] = {2, 3, 3};
    float coords[] = {4.5, 0, 6, 2, 6, 0, 4.5, 0};

    int b = 0;
    shape[figure_counter].figure = CIRCLE;
    shape[figure_counter].data.circle.x1 = data[b];
    b++;
    shape[figure_counter].data.circle.y1 = data[b];
    b++;
    shape[figure_counter].data.circle.radius1 = data[b];
    b++;
    figure_counter++;

    shape[figure_counter].figure = TRIANGLE;
    b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords[b];
        b++;
    }
    figure_counter++;

    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = EMPTY;
        }
    }

    find_collisions(shape, figure_counter, collision);

    int real = collision[0][0];
    int real2 = collision[1][0];

    int exp = 1;
    int exp2 = 0;

    ASSERT_EQUAL(exp, real);
    ASSERT_EQUAL(exp2, real2);
}

CTEST(find_collisions, ct_dont_intersect_6)
{
    int figure_counter = 0;
    Shape shape[2];
    float data[] = {2, 3, 3};
    float coords[] = {11, 1, 13, 6, 15, 1, 11, 1};

    int b = 0;
    shape[figure_counter].figure = CIRCLE;
    shape[figure_counter].data.circle.x1 = data[b];
    b++;
    shape[figure_counter].data.circle.y1 = data[b];
    b++;
    shape[figure_counter].data.circle.radius1 = data[b];
    b++;
    figure_counter++;

    shape[figure_counter].figure = TRIANGLE;
    b = 0;
    for (int d = 0; d < 4; d++) {
        shape[figure_counter].data.triangle.x[d] = coords[b];
        b++;
        shape[figure_counter].data.triangle.y[d] = coords[b];
        b++;
    }
    figure_counter++;

    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = EMPTY;
        }
    }

    find_collisions(shape, figure_counter, collision);

    int real = collision[0][0];
    int real2 = collision[1][0];

    int exp = EMPTY;
    int exp2 = EMPTY;

    ASSERT_EQUAL(exp, real);
    ASSERT_EQUAL(exp2, real2);
}
