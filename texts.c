#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SYMBOL_COUNT 3072

int texts_get_count() {
    FILE* texts_file;
    char* str;
    int texts_count = 0;
    texts_file = fopen("RU/texts.txt", "r");
    if (texts_file == NULL) {
        return -1;
    }
    str = malloc(MAX_SYMBOL_COUNT * sizeof(char));
    if (str == NULL) {
        return -1;
    }
    while (fgets(str, MAX_SYMBOL_COUNT, texts_file) != NULL) {
        if ((str[0] == '&') && (str[1] == '&')) {
            texts_count++;
        }
    }
    return texts_count;
}

char** texts_read() {
    //Инициализация переменных
    FILE* texts_file;  //Для работы с файлом
    char* str;         //Для считывания строк
    int text_number;   //Номер текста
    fpos_t pos;  //Для того, чтобы запомнить позицию в файле
    int newline_count;  //Количество переходов на новую строку
    char** text_out;  //Хранит текст под номером text_number

    //Объявление переменных
    texts_file = fopen("RU/texts.txt", "r");  //Открытие файла
    if (texts_file == NULL) {  //Проверка, открылся ли файл
        return NULL;
    }
    str = malloc(MAX_SYMBOL_COUNT *
                 sizeof(char));  //выделение памяти для хранения строк
    if (str == NULL) {  //Выделилась ли память
        return NULL;
    }
    srand(time(NULL));                         //Случайный сид
    text_number = rand() % texts_get_count();  //Открытия случайного текста
    newline_count = 0;  //Обнуление счетчика переходов на новую строку

    //Поиск нужного текста
    while (text_number) {
        fgets(str, MAX_SYMBOL_COUNT, texts_file);
        if ((str[0] == '&') && (str[1] == '&')) {
            text_number--;
        }
    }

    //Подсчет количества переходов на новую строку
    fgetpos(texts_file, &pos);
    do {
        newline_count++;
        fgets(str, MAX_SYMBOL_COUNT, texts_file);
    } while ((str[0] != '&') || (str[1] != '&'));
    fsetpos(texts_file, &pos);

    //Выделения памяти. Каждому абзацу своя переменная.
    text_out = (char**)malloc(newline_count * sizeof(char*));
    if (text_out == NULL) {
        return NULL;
    }

    //Считывание текста в память
    for (int i = 0; i < newline_count; i++) {
        text_out[i] = malloc(MAX_SYMBOL_COUNT * sizeof(char));
        fgets(text_out[i], MAX_SYMBOL_COUNT, texts_file);
    }

    //освобождение
    free(str);
    fclose(texts_file);

    //Возвращаем указатель на наш текст
    return text_out;
}

void texts_print(char** text) {
    for (int i = 0; (text[i][0] != '&') && (text[i][1] != '&'); i++) {
        for (char* j = text[i]; *j != '\0'; j++) {
            printf("%c", *j);
        }
    }
}

int main() {
    //Проверка работоспособности
    char** text;
    text = texts_read();
    texts_print(text);
    return 0;
}
