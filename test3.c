// Программа которая будет формировать буфер для отправки от ККМ в контроллер ТРК
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

// Макрос объема протокола
#define SIZE 25

// Протокол ТРК172
char result[SIZE] = {0};

struct protocol
{
    int SOH;
    int TRK_No;
    int Command;
    int STX;
    int Price;
    int Volume;
    int Status;
    int Error;
    int Code;
    int ETX;
    int CRC;
};



// Функция буфера
void MakeBuffer(struct protocol *tpointer)
{
    // Проверка ввода пользовтеля
    result[0] = tpointer->SOH;
    // Проверка ввода пользователя
    if(tpointer->TRK_No > 0 && tpointer->TRK_No < 127)
    {
        tpointer->Error = 0;
    }
    else
    {
        tpointer->Error = 1;
    }
    result[1] = tpointer->TRK_No;

    if(tpointer->Command > 0 && tpointer->Command < 15)
    {
        tpointer->Error = 0;
    }
    else
    {
        tpointer->Error = 1;
    }
    result[2] = tpointer->Command;

    if(tpointer->STX == 0 || tpointer->STX == 2)
    {
        tpointer->Error = 0;
    }
    else
    {
        tpointer->Error = 1;
    }
    result[3] = tpointer->STX;

    if(tpointer->Price > 0 && tpointer->Price < 999999)
    {
        tpointer->Error = 0;
    }
    else
    {
        tpointer->Error = 1;
    }

    result[4] = (tpointer->Price%1000000)/100000;
    result[5] = (tpointer->Price%100000)/10000;
    result[6] = (tpointer->Price%10000)/1000;
    result[7] = (tpointer->Price%1000)/100;
    result[8] = (tpointer->Price%100)/10;
    result[9] = tpointer->Price%10;

    if(tpointer->Volume > 0 && tpointer->Volume < 999999)
    {
        tpointer->Error = 0;
    }
    else
    {
        tpointer->Error = 1;
    }

    result[10] = (tpointer->Volume%1000000)/100000;
    result[11] = (tpointer->Volume%100000)/10000;
    result[12] = (tpointer->Volume%10000)/1000;
    result[13] = (tpointer->Volume%1000)/100;
    result[14] = (tpointer->Volume%100)/10;
    result[15] = tpointer->Volume%10;

    if(tpointer->Status > 0 && tpointer->Status < 65535)
    {
        tpointer->Error = 0;
    }
    else
    {
        tpointer->Error = 1;
    }

    result[16] = (tpointer->Status%10000)/1000;
    result[17] = (tpointer->Status%1000)/100;
    result[18] = (tpointer->Status%100)/10;
    result[19] = tpointer->Status%10;

    if(tpointer->Error == 0 || tpointer->Error == 1)
    {
        tpointer->Error = 0;
    }
    else
    {
        tpointer->Error = 1;
    }
    result[20] = tpointer->Error;

    if(tpointer->Code > 0 && tpointer->Code < 255)
    {
        tpointer->Error = 0;
    }
    else
    {
        tpointer->Error = 1;
    }
    result[21] = tpointer->Code;

    if(tpointer->ETX == 0 || tpointer->ETX == 3)
    {
        tpointer->Error = 0;
    }
    else
    {
        tpointer->Error = 1;
    }
    result[22] = tpointer->ETX;

    if(tpointer->CRC > 0 && tpointer->CRC < 255)
    {
        tpointer->Error = 0;
    }
    else
    {
        tpointer->Error = 1;
    }
    result[23] = tpointer->CRC;
}

FILE * myFopen(const char * name, const char * mode)
{
    FILE * fptr;
    fptr = fopen(name, mode);
    if(fptr == NULL)
    {
        printf("Не удалось открыть файл.\n");
        puts("Нажмите любую кнопку для выхода.\n");
        getch();
        exit(1);
    }
    else
    {
        printf("Файл открыт успешно в режиме записи.\n");
    }
    return fptr;
}

void myFclose(FILE * fptr)
{
    if (fclose(fptr) == 0)
    {
        puts("Файл успешно закрыт. Выход...");
    }
    else
    {
        puts("Не удалось закрыть файл. Нажмите любую кнопку для выхода...\n");
        getch();
    }
}


int main(void)
{
    // Подключаем локаль
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Инициализация протокола
    struct protocol TRK172 = {12,4,0,14,555555,666666,1234,11,0,11,14};

    // Функция записи исполняющая функцию буффера
    MakeBuffer(&TRK172);

    // Вывод на экран для проверки результата
    printf("SOH (HEX): %X\n", result[0]);
    printf("TRK_No (ASCII): %d\n", result[1]);
    printf("Command (ASCII): %d\n",result[2]);
    printf("STX (HEX): %X\n", result[3]);
    printf("Price (ASCII): %d%d%d%d%d%d\n", result[4],result[5],result[6],result[7],result[8],result[9]);
    printf("Volume (ASCII): %d%d%d%d%d%d\n", result[10],result[11],result[12],result[13],result[14],result[15]);
    printf("Status (ASCII): %d%d%d%d\n", result[16],result[17],result[18],result[19]);
    printf("Error (ASCII): %d\n", result[20]);
    printf("Code (ASCII): %d\n", result[21]);
    printf("ETX (HEX): %X\n", result[22]);
    printf("CRC (HEX): %X\n", result[23]);

    char control[SIZE] = {0};
    control[SIZE] = result[SIZE];
    result[SIZE] = result[SIZE]^control[SIZE];
    // Файл будет играть роль буфера отправки
    FILE * FILEptr;
    FILEptr = myFopen("out.txt", "w");

    // Записываем значения
    for(int j = 0; j < 10; j++)
    {
        fprintf(FILEptr, "%d", result[j]);

    }

    // Закрываем файл
    myFclose(FILEptr);

    return 0;
}
