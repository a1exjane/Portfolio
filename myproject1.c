// Считывание файлов из открытой директории

#include <stdio.h>
#include <stdlib.h>     // Для MAX_PATH
#include <dirent.h>     // opendir и т.д.
#include <sys/stat.h>   // Для stat() и struct stat
#include <string.h>     // Для strcpy() и strcat()
#include <time.h>
#include <windows.h>

#define MAX_PATH 256

int main(void)
{
    // Подключаем локаль
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    char pathFull[MAX_PATH + FILENAME_MAX];                        // Полный путь к файлу
    DIR * folder;
    char pathDir[MAX_PATH] = ".";
    int chr;
    struct dirent * file;               // Отсюда берём данные о файле (имя)
    struct stat fileStat;               // Сюда записываем статистику функцией stat()
    long int total;                     // Переменная счетчик для рассчета общего обьема файлов в директории
    total = 0;

    printf("Введите путь до директории:\n");
    for(int i = 0; i < MAX_PATH-1; i++)
    {
        chr = getchar();
        if(chr != '\n')
        pathDir[i] = chr;
        else
        {
            pathDir[i] = '\0';
            break;
        }
    }
    if((folder = opendir(pathDir)) == NULL)             // Проверка удалось ли открыть папку
    {
        printf("Невозможно открыть директорию \"%s\"\n", pathDir);
        system("pause");
        exit(1);
    }
    folder = opendir(pathDir);
    printf("Открыли директорию \"%s\"\n", pathDir);

    printf("%2s %10s %30s %24s\n", "N", "Размер", "Имя файла", "Время изменения");
    while(file = readdir(folder)) {
        strcpy(pathFull, pathDir);
        strcat(pathFull,"/");
        strcat(pathFull, file->d_name);
        if (!stat(pathFull, &fileStat)) {
            printf("%2ld ",telldir(folder));
            if(S_ISDIR(fileStat.st_mode))       // Использую макрос S_ISDIR
                printf("   %10s", "<DIR>");     // Для определния, что это директория
            else
                if(fileStat.st_size != 0)       // Счетчик сумматор обьема файлов
                {
                    total = total + fileStat.st_size;
                }
                if(fileStat.st_size < 1024)      // Для удобства интерфейса добавляем функции отображения в байтах
                {
                    printf("%10ld B ", (long)fileStat.st_size);
                }
                if(fileStat.st_size > 1024)      // Для удобства интерфейса добавляем функции отображения в Кбайтах
                {
                    printf("%10ld KB", (long)fileStat.st_size/1024);
                }
                if(fileStat.st_size > 1048576)   // Для удобства интерфейса добавляем функции отображения в Мбайтах
                {
                    printf("%10ld MB", ((long)fileStat.st_size/1048576));
                }
                if(fileStat.st_size > 1073741824) // Для удобства интерфейса добавляем функции отображения в Гбайтах
                {
                    printf("%10ld GB", ((long)fileStat.st_size/1073741824));
                }

            printf(" %30s %s", file->d_name, ctime(&fileStat.st_mtime));

        }
        else {
            puts("Не удалось записать данные в структуру stat");
            exit(2);
        }
    }
    // Выводим на экран обьем всех файлов в директории
    if(total < 1024)                 // Для удобства интерфейса добавляем функции отображения в байтах
                {
                    printf("Общий обьем файлов в директории: %10ld B \n", (long)total);
                }
                if(total > 1024)     // Для удобства интерфейса добавляем функции отображения в Кбайтах
                {
                    printf("Общий обьем файлов в директории: %10ld KB\n", (long)total/1024);
                }
                if(total > 1048576)   // Для удобства интерфейса добавляем функции отображения в Мбайтах
                {
                    printf("Общий обьем файлов в директории: %10ld MB\n", ((long)total/1048576));
                }
                if(total > 1073741824)  // Для удобства интерфейса добавляем функции отображения в Гбайтах
                {
                    printf("Общий обьем файлов в директории: %10ld GB\n", ((long)total/1073741824));
                }

    if(closedir(folder) != 0)
    {
        printf("Невозможно закрыть директорию \"%s\"\n", pathDir);
        system("pause");
        exit(2);
    }
    closedir(folder);
    printf("Закрыли директорию \"%s\"\n", pathDir);
    printf("Конец программы\n");
    return 0;
}
