// Создаем простую СУБД на основе связанного списка.
// Добавляем новые функции для сохранения связного списка в файл и его загрузки

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>

#define CLS system("cls")
#define FFLUSH while(getchar()!='\n')
#define NEXT current=current->next

struct order
{
    int number;
    int sum;
    struct order * next;
};

struct order * first;
struct order * newNode;
struct order * current;

char * filename = "list.dat"; /* Имя файла куда будем сохранять список*/


int showMenu(void);             // Прототип функции вывода на экран меню
void addNode(void);             // Прототип функции добавления нового узла списка
void removeNode(void);          // Прототип функции удаления узла списка
void showList(void);            // Прототип функции вывода на экран текущего узла
struct order * memAlloc(void);  // Выделение памяти для нового узла
void getInput(int * result, int min, int max, char * welcome);  // Прием вводимых данных пользователем

// Доп. функции загрузки и сохранения списка
void loadList(void);
void saveList(void);

int main()
{
    // Подключаем локаль
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int choice = '\0';      // Инициализация кнопки вывода кнопкой ENTER
    first = NULL;

    loadList();             // Загружаем список при запуске программы

    while(choice != 'Q')    // Кнопка выхода
    {
        choice = showMenu();
        switch (choice)
        {
        case 'S':           // Вывод на экран узел списка
            CLS;
            showList();
            break;
        case 'A':           // Добавление нового узла списка
            CLS;
            addNode();
            break;
        case 'R':           // Удаление узла списка
            CLS;
            removeNode();
            break;
        case 'Q':
            puts("Выход...");
            saveList();     // Сохранение на диск списка перед выходом
            system("pause");
            break;

        default:
            CLS;
            puts("Неправельный ввод");
            break;
        }
    }
}

// Функция вывода на экран меню
int showMenu(void)
{
    int ch = '\0';
    printf("A - Добавить узел списка\nR - Удалить узел списка\nS - Показать список\nQ - Выход\n");
    ch = getch();
    return(toupper(ch));
}

// Выделение памяти для нового узла списка
struct order * memAlloc(void)
{
    struct order * temp;
    temp = (struct order *)malloc(sizeof(struct order));
    if(temp == NULL)                                // Проверка выделения памяти
    {
        puts("Ошибка выделения памяти!!! Выход...\n");   // Если нет то выводим ошибку
        system("pause");
        exit(1);
    }
    return temp;
}

// Функция добавления нового узла
void addNode(void)
{
    if(first == NULL)
    {
        first = memAlloc();
        current = first;
    }
    else
    {
        current = first;
        while(current->next != NULL)
            current = current->next;
        newNode = memAlloc();           // Временная переменная
        current->next = newNode;
        current = newNode;
    }
    current->next = NULL;

    getInput(&current->number, 0, 1000000, "Enter order number: \n");
    getInput(&current->sum, 0, 4000000, "Enter sum of order: \n");
}

// Вывод на экран содержимого файла
void showList(void)
{
    int count = 1;
    // Проверка на наличие
    if(first == NULL)
    {
        puts("Список пуст\n");
        return;
    }
    puts("Список: \n");

    current = first;
    while(current != NULL)
    {
        printf("Узел #%d: Заказ #%d, Сумма %d\n",
               count, current->number, current->sum);
        current = current->next;
        count++;
    }
}

// Удаление элемента узла
void removeNode(void)
{
    struct order * prev;
    int rem = 0;
    int num;

    if(first == NULL)
    {
        puts("Нельзя удалить! Список пуст.\n");
        return;
    }
    showList();

    getInput(&rem, 1, 1000000, "Выберите номер узла для удаления: \n");

    num = 1;
    current = first;
    prev = NULL;
    while(num != rem)
    {
        prev = current;
        current = current->next;
        num++;
        if(current == NULL)
        {
            puts("Узел не найден.\n");
            return;
        }
    }
    if(prev == NULL)
        first = current->next;
    else
        prev->next=current->next;
    printf("Узел #%d удален\n", rem);
    free(current);
}

// Получение данных ввода
void getInput(int * result, int min, int max, char * welcome)
{
    *result = -1;
    while(*result < min || *result > max)
    {
        printf("%s", welcome);
        scanf("%d", result);
        FFLUSH;
    }
}

// Функция загрузки и сохранения нашего списка
void loadList(void)
{
	FILE * fptr;                  // Указатель на наш файл
	struct order loaded;          // Структура которая будет загружена из нашего файла
	int records = 0;              // Количество загруженные элементов

	fptr = fopen(filename, "rb"); // Открытие файла в режиме чтения
	if(fptr == NULL)              // Если такого файла нет, то выходим из функции
		return;

	// Считывание  всех записей из файла по одной. Цикл возвращает кол-во считанных элементов
	while( fread(&loaded, sizeof(struct order), 1, fptr) )
	{
	    // Создаем аналог элементов нашего списка (Аналог addNode) для дальнейшей загрузки в него нашего списка
		if(first == NULL)
		{
			first = memAlloc();
			current = first;
		}
		else
		{
			current = first;
			while(current->next)
				NEXT;
			newNode = memAlloc();
			current->next = newNode;
			current = newNode;
		}

		// Заполняем текущий элемент данными загруженными из файла
		current->number = loaded.number;
		current->sum = loaded.sum;
		// Указатель на след элемент списка сохранять не нужно

		current->next = NULL;   // Закрываем список
		records++;              // Считаем количество наших элементов
	}
	fclose(fptr);
	printf("%d Запись прочитан из \'%s\'\n", records, filename);
}

// Функция сохранения нашего списка в файл
void saveList(void)
{
	int records = 0;    // Количество записанных элементов
	FILE * fptr;        // Файл для сохранения списка

    fptr = fopen(filename, "wb");   // Открытие файла в режиме бинарной записи/дозаписи
	if(fptr == NULL)
	{
		puts("Невозможно создать файл для сохранения списка!\n");
		return;
	}

    // Записываем элементы в файл по одному начиная с первого элемента
    current = first;                // Если список пуст (current == NULL), цикл не запуститься
    while(current)
    {
        fwrite(current, sizeof(struct order), 1, fptr);
        NEXT;
        records++;
    }

    fclose(fptr);
    printf("%d Записей записаны в \'%s\'\n", records, filename);
}
