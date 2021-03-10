#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum LengthsOfFigures { LENGTH_OF_CIRCLE = 6, LENGTH_OF_TRIANGLE = 8 };

typedef enum TypesOfFigures { UNKNOWN, CIRCLE, TRIANGLE } Type;

typedef enum ErrorStatus { SUCCESS = 0, FAILURE = -1 } ErrStatus;

void skip_space(char** cursor_start)
{
    while (**cursor_start == ' ') {
        (*cursor_start)++;
    }
}

// typedef struct

ErrStatus check_punctuation_symbols(char** cursor_start, char symbol)
{
    if (**cursor_start == symbol) {
        (*cursor_start)++;
        return SUCCESS;
    } else {
        printf("Error: expected \"%c\"\n\n", symbol);
        return FAILURE;
    }
}

ErrStatus check_left_brackets(Type figure, char** cursor_start)
{
    ErrStatus implementation = check_punctuation_symbols(cursor_start, '(');
    if (figure == TRIANGLE && implementation == SUCCESS) {
        implementation = check_punctuation_symbols(cursor_start, '(');
    }
    return implementation;
}

ErrStatus check_extra_token(char** cursor_start)
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
void select_type(char** cursor_start, char** cursor_end)
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

ErrStatus parse_circle(char** cursor_start, char** cursor_end, int* counter)
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
    (*counter)++;
    printf("%d Тип фигуры: circle\n", *counter);
    printf("x1 = %.1lf y1 = %.1lf \nradius1 = %.1lf\n \n", x1, y1, radius1);
    return SUCCESS;
}

ErrStatus parse_triangle(char** cursor_start, char** cursor_end, int* counter)
{
    ErrStatus implementation2;
    float coords[8];
    int number = 0;

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
            number++;
        }
        if (*cursor_start == *cursor_end) {
            printf("Error: expected float %c%d\n\n", letter, number);
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
    (*counter)++;
    printf("%d Тип фигуры: triangle\n", *counter);
    printf("x1 = %.1lf y1 = %.1lf\nx2 = %.1lf y2 = %.1lf\nx3 = %.1lf "
           "y3 = %.1lf\nx4 = %.1lf y4 = %.1lf\n\n",
           coords[0],
           coords[1],
           coords[2],
           coords[3],
           coords[4],
           coords[5],
           coords[6],
           coords[7]);
    return SUCCESS;
}

int main()
{
    char input[70];
    char* cursor_start;
    char* cursor_end;
    Type figure;
    int counter = 0;

    printf("Напишите то, что хотите проанализировать:\n");
    while ((fgets(input, 70, stdin))) {
        figure = UNKNOWN;
        fputs(input, stdout); // если надо вывести введенные данные
        cursor_start = input;
        cursor_end = input;

        skip_space(&cursor_start);
        figure = determine_figure(&cursor_start, &cursor_end);
        cursor_start = cursor_end;
        skip_space(&cursor_start);

        switch (figure) {
        case CIRCLE:
            parse_circle(&cursor_start, &cursor_end, &counter);
            break;
        case TRIANGLE:
            parse_triangle(&cursor_start, &cursor_end, &counter);
            break;
        case UNKNOWN:
            printf("Error: expected \"circle\" | \"triangle\"\n\n");
            break;
        }
    }
    return 0;
}
