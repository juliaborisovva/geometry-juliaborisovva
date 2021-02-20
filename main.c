#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct circle {
    float x;
    float y;
    float radius;
} circle;

int main()
{
    float x1, y1 = 0;
    float radius1 = 0;
    char input[70];
    char* cursor = input;
    char* cursor2 = input;
    size_t n = 0;
    char typec[] = {"circle"};
    char typet[] = {"triangle"};
    char typep[] = {"polygon"};
    int flag = 0;
    int flag2 = 1;

    printf("Напишите то, что хотите проанализировать:\n");

    //собираем информацию о фигуре
    fgets(input, 70, stdin);
    // fputs(input, stdout);
    //    while (cursor != input + strlen(input)) {

    //пока встречается пробел, пропускаем его
    while (*cursor == ' ') {
        cursor++;
        cursor2++;
    }

    //если встречается буква, то двигаем второй указатель пока не встретит
    //пробел
    if (isalpha(*cursor) != 0) {
        // while (*cursor2 == ' ') {
        //    cursor2++;
        //}
        while ((isalpha(*cursor2) != 0) && (cursor2 != input + strlen(input))) {
            cursor2++;
        }
    }

    //посчитаем длину введенного типа фигуры
    n = cursor2 - cursor;

    // printf("значение курсора 1 = %d\n", *cursor);
    // printf("значение курсора 2 = %d\n", *cursor2);
    // printf("размер введенного типа фигуры %lu\n", n);

    //Если введенный тип совпал с известными нам типами фигур, то печатаем тип и
    //выставляем переменной flag соответствующее значение, иначе - вывести
    //ошибку
    if (strncasecmp(cursor, typec, n) == 0) {
        printf("Тип фигуры: circle\n");
        flag = 1;
    } else if (strncasecmp(cursor, typet, n) == 0) {
        printf("Тип фигуры: triangle\n");
        flag = 2;
    } else if (strncasecmp(cursor, typep, n) == 0) {
        printf("Тип фигуры: polygon\n");
        flag = 3;
    } else {
        printf("Error: expected \"circle\" | \"triangle\" | \"polygon\"\n");
    }

    //если flag не изменился (==0), то есть была выведена ошибка, то последующие
    //действия не запустятся. Если flag принял новое ненулевое значение, то
    //присваиваем первому курсору положение второго курсора
    if (flag != 0) {
        cursor = cursor2;

        //пропускаем пробелы
        while (*cursor == ' ') { //проверить если я не пишу продолжение что
                                 //пишет программа, не повторяется ли до
                                 //бесконечности? может добавить доп условие &&
                                 // while cursor != input + strlen(input)
            cursor++;
            cursor2++;
        }

        //ожидаем встретить скобку, если не встречаем - выводим ошибку и
        //переменная flag2 будет с изначальным значением
        if (*cursor == '(') {
            cursor++;
            cursor2++;
            flag2 = 0;
        } else {
            printf("Error: expected \"(\"\n");
        }

        //если переменная flag2 имеет значение 0, значит ошибки на предыдущем
        //этапе не было, выполняем действия
        if (flag2 == 0) {
            while (*cursor == ' ') {
                cursor++;
                cursor2++;
            }

            switch (flag) {
            case 1:
                //считываем значения х1
                x1 = strtof(cursor, &cursor2);
                // printf("значение курсора 1 = %d\n", *cursor);
                // printf("значение курсора 2 = %d\n", *cursor2);
                if (cursor != cursor2) {
                    //перекидываем первый указатель на значение второго
                    cursor = cursor2;
                    //пока видим пробелы пропускаем их
                    while (*cursor == ' ') {
                        cursor++;
                        cursor2++;
                    }
                    //считываем значение у1, перекидываем  указатель на значение
                    //второго
                    y1 = strtof(cursor, &cursor2);
                    // printf("значение курсора 1 = %d\n", *cursor);
                    // printf("значение курсора 2 = %d\n", *cursor2);
                    if (cursor != cursor2) {
                        cursor = cursor2;
                        //пока видим пробелы пропускаем их
                        while (*cursor == ' ') {
                            cursor++;
                            cursor2++;
                        }
                        //если встретили запятую, то пропустим и дадим
                        //переменной flag2 новое значение, иначе ошибка
                        if (*cursor == ',') {
                            cursor++;
                            cursor2++;
                            flag2 = 2;
                        } else {
                            printf("Error: expected \",\"");
                        }
                        //проверяем была ли ошибка на предыдущем этапе
                        if (flag2 == 2) {
                            while (*cursor == ' ') {
                                cursor++;
                                cursor2++;
                            }
                            radius1 = strtof(cursor, &cursor2);
                            if (cursor != cursor2) {
                                cursor = cursor2;
                                //пока пробел, пропускаем
                                while (*cursor
                                       == ' ') { //дописать доп условие, чтоб
                                                 //если человек не написал
                                                 //конец, то не повторялось до
                                                 //бесконечности
                                    cursor++;
                                    cursor2++;
                                }

                                //если встречается скобка, то меняем значение
                                // flag2, и пропускаем, иначе ошибка
                                if (*cursor == ')') {
                                    cursor++;
                                    cursor2++;
                                    flag2 = 3;
                                } else {
                                    printf("Error: expected \")\"\n");
                                }
                                if (flag2 == 3) {
                                    printf("x1 = %.1lf y1 = %.1lf\nradius1 = "
                                           "%.1lf\n",
                                           x1,
                                           y1,
                                           radius1);
                                }
                            } else {
                                printf("Error: expected float radius\n");
                            }
                        }
                    } else {
                        printf("Error: expected float y\n");
                    }
                } else {
                    printf("Error: expected float x\n");
                }
                // printf("cursor 1 = %d\ncursor 2 = %d\n", *cursor, *cursor2);

                break;
            case 2:

                break;
            case 3:

                break;
            }
            //    size_t z = strlen(input);
            //    int i = 1;
            //    char* begin = input;
            //    for (int i = 0; i <= z; i++) {
            //	begin++;
            //    }
            //    printf("длина = %lu\n", z);
            //    fputs(input, stdout);
            //	printf("\n.%c.", input[z]);
            //	printf("\n.%c.", *cursor);

            while (*cursor != '\0') {
                if ((isalnum(*cursor) != 0) || (ispunct(*cursor) != 0)) {
                    printf("Error: expected nothing after data string\n");
                    *cursor = '\0';
                } else {
                    cursor++;
                }
            }
        }
    }
    //ненужно пока
    //      for(int i=0; i<strlen(figure); i++)
    //            {
    //                  if(figure[i] == str[i])
    //                {
    //                      flag++;
    //            }
    //  }
    //        if(flag==strlen(figure))
    //      {
    //    circle.x=x1;
    //  circle.y=y1;
    //  circle.radius=radius1;
    //    }
    //  else (printf("\nError: expected \"circle\"\n"));
    // printf("фигура= %s x= %.1f y= %.1f rad= %.1f\n",figure, circle.x,
    // circle.y, circle.radius);
    // printf("%s", figure);
    //      printf("тип фигуры = %s\n икс = %.1f\n игрик = %.1f\n радиус =
    //      %.1f", figure, x1, y1, radius1); printf("\nтип фигуры = %s",
    //      figure);
    return 0;
}
