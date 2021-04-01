#include <libgeometry/geometry.h>
#include <stdio.h>
#define EMPTY 11

int main()
{
    char input[70];
    char* cursor_start;
    char* cursor_end;
    Type figure;
    int figure_counter = 0;
    int max_figure_value = 10;
    Shape shape[max_figure_value];
    ErrStatus status;

    printf("Write the figure to analyze:\n");
    while ((fgets(input, 70, stdin))) {
        if (figure_counter == max_figure_value) {
            printf("The number of figures is exceeded. Maximum: 10\n");
            break;
        }
        figure = UNKNOWN;
        fputs(input, stdout); // если надо вывести введенные данные
        cursor_start = input;
        cursor_end = input;

        figure = determine_figure(&cursor_start, &cursor_end);
        cursor_start = cursor_end;
        status = 0;

        switch (figure) {
        case CIRCLE:
            status = parse_circle(
                    &cursor_start, &cursor_end, &figure_counter, shape);
            break;
        case TRIANGLE:
            status = parse_triangle(
                    &cursor_start, &cursor_end, &figure_counter, shape);
            break;
        case UNKNOWN:
            printf("Error: expected \"Circle\" | \"Triangle\"\n\n");
            break;
        }

        switch (status) {
        case LBRACKET:
            printf("Error: expected \"(\"\n\n");
            break;
        case X1:
            printf("Error: expected float x1\n\n");
            break;
        case Y1:
            printf("Error: expected float y1\n\n");
            break;
        case X2:
            printf("Error: expected float x2\n\n");
            break;
        case Y2:
            printf("Error: expected float y2\n\n");
            break;
        case X3:
            printf("Error: expected float x3\n\n");
            break;
        case Y3:
            printf("Error: expected float y3\n\n");
            break;
        case X4:
            printf("Error: expected float x4\n\n");
            break;
        case Y4:
            printf("Error: expected float y4\n\n");
            break;
        case COMMA:
            printf("Error: expected \",\"\n\n");
            break;
        case RADIUS1:
            printf("Error: expected float radius\n\n");
            break;
        case RBRACKET:
            printf("Error: expected \")\"\n\n");
            break;
        case EXTRATOKEN:
            printf("Error: expected nothing after data string\n\n");
            break;
        case DONTMATCH:
            printf("Error: point 1 and point 4 don't match\n\n");
            break;
        default:
            break;
        }
    }

    // поиск коллизий
    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = EMPTY;
        }
    }
    find_collisions(shape, figure_counter, collision);
    printf("\n\n");
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            printf("%d, ", collision[r][k]);
        }
        printf("\n");
    }
    printf("\n\n");

    // вывод данных, печать
    printf("\n\n");
    for (int i = 0; i < figure_counter; i++) {
        if (shape[i].figure == CIRCLE) {
            printf("%d. circle ( %.1lf %.1lf, %.1lf )",
                   i + 1,
                   shape[i].data.circle.x1,
                   shape[i].data.circle.y1,
                   shape[i].data.circle.radius1);
            printf("\nperimeter = %.1lf", shape[i].data.circle.perimeter);
            printf("\narea = %.1lf", shape[i].data.circle.area);
        } else if (shape[i].figure == TRIANGLE) {
            printf("%d triangle (( ", i + 1);
            for (int m = 0; m < 4; m++) {
                printf("%.1lf %.1lf, ",
                       shape[i].data.triangle.x[m],
                       shape[i].data.triangle.y[m]);
            }
            printf("))");
            printf("\nperimeter = %.1lf", shape[i].data.triangle.perimeter);
            printf("\narea = %.1lf", shape[i].data.triangle.area);
        }
        printf("\nIntersects with:\n");

        //пересечение
        for (int d = 0; d < (figure_counter - 1); d++) {
            if (collision[i][d] != EMPTY) {
                if (shape[collision[i][d]].figure == CIRCLE) {
                    printf("  • circle %d\n", collision[i][d] + 1);
                }
                if (shape[collision[i][d]].figure == TRIANGLE) {
                    printf("  • triangle %d\n", collision[i][d] + 1);
                }
            }
        }
        printf("\n");
    }
    return 0;
}
