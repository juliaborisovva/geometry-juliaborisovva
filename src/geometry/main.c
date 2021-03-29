#include <libgeometry/geometry.h>
#include <stdio.h>

int main()
{
    char input[70];
    char* cursor_start;
    char* cursor_end;
    Type figure;
    int figure_counter = 0;
    int max_figure_value = 10;
    Shape shape[max_figure_value];

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

        switch (figure) {
        case CIRCLE:
            parse_circle(&cursor_start, &cursor_end, &figure_counter, shape);
            break;
        case TRIANGLE:
            parse_triangle(&cursor_start, &cursor_end, &figure_counter, shape);
            break;
        case UNKNOWN:
            printf("Error: expected \"Circle\" | \"Triangle\"\n\n");
            break;
        }
    }

    // поиск коллизий, магическое число
    int collision[figure_counter][figure_counter - 1];
    for (int r = 0; r < figure_counter; r++) {
        for (int k = 0; k < (figure_counter - 1); k++) {
            collision[r][k] = 11;
        }
    }

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
                // сделать х и у в массивах, а не как отдельные единицы++
                printf("%.1lf %.1lf, ",
                       shape[i].data.triangle.x[m],
                       shape[i].data.triangle.y[m]);
            }
            printf("))");
            printf("\nperimeter = %.1lf", shape[i].data.triangle.perimeter);
            printf("\narea = %.1lf", shape[i].data.triangle.area);
        }
        printf("\nIntersects with:\n");

        //пересечение и исправить магическое число 11

        for (int d = 0; d < (figure_counter - 1); d++) {
            if (collision[i][d] != 11) {
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
