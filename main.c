#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum LengthsOfFigures { LENGTH_OF_CIRCLE = 6, LENGTH_OF_TRIANGLE = 8 };

typedef enum TypesOfFigures { UNKNOWN, CIRCLE, TRIANGLE } Type;

typedef enum ErrorStatus { SUCCESS = 0, FAILURE = -1 } Bool;

void skip_space(char** cursor_start)
{
    while (**cursor_start == ' ') {
        (*cursor_start)++;
    }
}

Bool check_punctuation_symbols(char** cursor_start, /*int* implementation,*/ char symbol)
{
    if (**cursor_start == symbol) {
        (*cursor_start)++;
        return SUCCESS;
    } else {
        printf("Error: expected \"%c\"\n\n", symbol);
        return FAILURE;
    }
}

Bool check_extra_token(char** cursor_start/*, int* implementation*/)
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
void select_type (char** cursor_start, char** cursor_end, char* input) {
    if (isalpha(**cursor_start) != 0) {
        *cursor_end = *cursor_start;
        while ((isalpha(**cursor_end) != 0)
               && (*cursor_end != input + strlen(input))) {
            (*cursor_end)++;
        }
    }
}

/*Если введенный тип совпал с известными нам типами фигур, то печатаем
тип и выставляем переменной figure соответствующее значение, иначе -
вывести ошибку*/
Type determine_figure (char** cursor_start, /*int* implementation, */size_t length_of_type, char* type_circle, char* type_triangle) {
    if (strncasecmp(*cursor_start, type_circle, LENGTH_OF_CIRCLE) == 0
        && length_of_type == LENGTH_OF_CIRCLE) {
        return CIRCLE;
    } else if (
            strncasecmp(*cursor_start, type_triangle, LENGTH_OF_TRIANGLE) == 0
            && length_of_type == LENGTH_OF_TRIANGLE) {
        return TRIANGLE;
    } else {
        printf("Error: expected \"circle\" | \"triangle\"\n\n");
        //*implementation = FAILURE;
        return UNKNOWN;
    }
}


Bool parse_circle(char **cursor_start, char **cursor_end, int* counter) {
    Bool implementation2;
    float x1, y1, radius1;
    x1 = strtof(*cursor_start, &(*cursor_end));
    if (*cursor_start == *cursor_end) {
        printf("Error: expected float x1\n\n");
        return FAILURE;
    }
    *cursor_start = *cursor_end;
    y1 = strtof(*cursor_start, &(*cursor_end));
    if (*cursor_start == *cursor_end) {
        printf("Error: expected float y1\n\n");
        return FAILURE;
    }
    *cursor_start = *cursor_end;
    skip_space(&(*cursor_start));
    implementation2 = check_punctuation_symbols(&(*cursor_start),/*&(*implementation),*/ ',');
    if (implementation2 == FAILURE) {
        return FAILURE;
    }
    radius1 = strtof(*cursor_start, &(*cursor_end));
    if (*cursor_start == *cursor_end) {
        printf("Error: expected float radius\n\n");
        return FAILURE;
    }
    *cursor_start = *cursor_end;
    skip_space(&(*cursor_start));
    implementation2 = check_punctuation_symbols(&(*cursor_start),/*&(*implementation),*/ ')');
    if (implementation2 == FAILURE) {
        return FAILURE;
    }
    implementation2 = check_extra_token(&(*cursor_start)/*, &(*implementation)*/);
    if (implementation2 == FAILURE) {
        return FAILURE;
    }
    (*counter)++;
    printf("%d Тип фигуры: circle\n", *counter);
    printf("x1 = %.1lf y1 = %.1lf \nradius1 = %.1lf\n \n", x1, y1, radius1);
    return SUCCESS;
}


int main()
{
    //float x1, y1 = 0;
    //float radius1 = 0;
    char input[70];
    char* cursor_start;
    char* cursor_end;
    size_t length_of_type = 0;
    char type_circle[] = {"circle"};
    char type_triangle[] = {"triangle"};
    Type figure;
    Bool implementation;
    int counter = 0;
    //для кейса треугольника:
    //float x2, y2, x3, y3, x4, y4 = 0;

    printf("Напишите то, что хотите проанализировать:\n");
    while ((fgets(input, 70, stdin))) {
        figure = UNKNOWN;
        implementation = SUCCESS;
        fputs(input, stdout); // если надо вывести введенные данные
        cursor_start = input;
        cursor_end = input;
        skip_space(&cursor_start);

        select_type(&cursor_start, &cursor_end, input);
        length_of_type = cursor_end - cursor_start;
        figure = determine_figure(&cursor_start, /*&implementation, */length_of_type, type_circle, type_triangle);
        if (figure == UNKNOWN) {
            continue;
        }

        cursor_start = cursor_end;
        skip_space(&cursor_start);
        implementation = check_punctuation_symbols(&cursor_start, /*&implementation, */'(');
        if (figure == TRIANGLE && implementation == SUCCESS) {
            implementation = check_punctuation_symbols(&cursor_start, /*&implementation, */'(');
        }
        if (implementation == FAILURE) {
            continue;
        }

        cursor_end = cursor_start;
        switch (figure) {
            case CIRCLE:
                parse_circle(&cursor_start, &cursor_end, &counter);
                /*if (implementation == FAILURE) {
                    printf("\n%d ОШИБКА НАЙДЕНА\n\n", implementation);
                } else {
                    printf("\n%d ОШИБКА НЕ НАЙДЕНА\n\n", implementation);
                }*/
                break;

            case TRIANGLE:
                /*x1 = strtof(cursor_start, &cursor_end);
                if (cursor_start == cursor_end) {
                    printf("Error: expected float x1\n\n");
                    continue;
                }
                cursor_start = cursor_end;
                y1 = strtof(cursor_start, &cursor_end);
                if (cursor_start == cursor_end) {
                    printf("Error: expected float y1\n\n");
                    continue;
                }
                cursor_start = cursor_end;
                skip_space(&cursor_start);
                check_punctuation_symbols(&cursor_start, &implementation, ',');
                if (implementation == FAILURE) {
                    continue;
                }

                x2 = strtof(cursor_start, &cursor_end);
                if (cursor_start == cursor_end) {
                    printf("Error: expected float x2\n\n");
                    continue;
                }
                cursor_start = cursor_end;
                y2 = strtof(cursor_start, &cursor_end);
                if (cursor_start == cursor_end) {
                    printf("Error: expected float y2\n\n");
                    continue;
                }
                cursor_start = cursor_end;
                skip_space(&cursor_start);
                check_punctuation_symbols(&cursor_start, &implementation, ',');
                if (implementation == FAILURE) {
                    continue;
                }

                x3 = strtof(cursor_start, &cursor_end);
                if (cursor_start == cursor_end) {
                    printf("Error: expected float x3\n\n");
                    continue;
                }
                cursor_start = cursor_end;
                y3 = strtof(cursor_start, &cursor_end);
                if (cursor_start == cursor_end) {
                    printf("Error: expected float y3\n\n");
                    continue;
                }
                cursor_start = cursor_end;
                skip_space(&cursor_start);
                check_punctuation_symbols(&cursor_start, &implementation, ',');
                if (implementation == FAILURE) {
                    continue;
                }

                x4 = strtof(cursor_start, &cursor_end);
                if (cursor_start == cursor_end) {
                    printf("Error: expected float x4\n\n");
                    continue;
                }
                cursor_start = cursor_end;
                y4 = strtof(cursor_start, &cursor_end);
                if (cursor_start == cursor_end) {
                    printf("Error: expected float y4\n\n");
                    continue;
                }
                cursor_start = cursor_end;
                skip_space(&cursor_start);
                check_punctuation_symbols(&cursor_start, &implementation, ')');
                if (implementation == FAILURE) {
                    continue;
                }
                check_punctuation_symbols(&cursor_start, &implementation, ')');
                if (implementation == FAILURE) {
                    continue;
                }
                check_extra_token(&cursor_start, &implementation);
                if (implementation == FAILURE) {
                    continue;
                }
                //передать данные о переменных,
                //типе фигуры, номере в массив
                //посчитать номер фигуры, возможно потом не понадобиться тут,
                //можно убрать (т.к. вывод будет после выполнения парсинга строки)
                counter++;
                printf("%d Тип фигуры: triangle\n", counter);
                printf("x1 = %.1lf y1 = %.1lf\nx2 = %.1lf y2 = %.1lf\nx3 = %.1lf "
                       "y3 = %.1lf\nx4 = %.1lf y4 = %.1lf\n\n",
                       x1,
                       y1,
                       x2,
                       y2,
                       x3,
                       y3,
                       x4,
                       y4);*/
                break;
        }
    }
    return 0;
}

