// ���������� ������ �� �������� ����������

#include <stdio.h>
#include <stdlib.h>     // ��� MAX_PATH
#include <dirent.h>     // opendir � �.�.
#include <sys/stat.h>   // ��� stat() � struct stat
#include <string.h>     // ��� strcpy() � strcat()
#include <time.h>
#include <windows.h>

#define MAX_PATH 256

int main(void)
{
    // ���������� ������
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    char pathFull[MAX_PATH + FILENAME_MAX];                        // ������ ���� � �����
    DIR * folder;
    char pathDir[MAX_PATH] = ".";
    int chr;
    struct dirent * file;               // ������ ���� ������ � ����� (���)
    struct stat fileStat;               // ���� ���������� ���������� �������� stat()
    long int total;                     // ���������� ������� ��� �������� ������ ������ ������ � ����������
    total = 0;

    printf("������� ���� �� ����������:\n");
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
    if((folder = opendir(pathDir)) == NULL)             // �������� ������� �� ������� �����
    {
        printf("���������� ������� ���������� \"%s\"\n", pathDir);
        system("pause");
        exit(1);
    }
    folder = opendir(pathDir);
    printf("������� ���������� \"%s\"\n", pathDir);

    printf("%2s %10s %30s %24s\n", "N", "������", "��� �����", "����� ���������");
    while(file = readdir(folder)) {
        strcpy(pathFull, pathDir);
        strcat(pathFull,"/");
        strcat(pathFull, file->d_name);
        if (!stat(pathFull, &fileStat)) {
            printf("%2ld ",telldir(folder));
            if(S_ISDIR(fileStat.st_mode))       // ��������� ������ S_ISDIR
                printf("   %10s", "<DIR>");     // ��� ����������, ��� ��� ����������
            else
                if(fileStat.st_size != 0)       // ������� �������� ������ ������
                {
                    total = total + fileStat.st_size;
                }
                if(fileStat.st_size < 1024)      // ��� �������� ���������� ��������� ������� ����������� � ������
                {
                    printf("%10ld B ", (long)fileStat.st_size);
                }
                if(fileStat.st_size > 1024)      // ��� �������� ���������� ��������� ������� ����������� � �������
                {
                    printf("%10ld KB", (long)fileStat.st_size/1024);
                }
                if(fileStat.st_size > 1048576)   // ��� �������� ���������� ��������� ������� ����������� � �������
                {
                    printf("%10ld MB", ((long)fileStat.st_size/1048576));
                }
                if(fileStat.st_size > 1073741824) // ��� �������� ���������� ��������� ������� ����������� � �������
                {
                    printf("%10ld GB", ((long)fileStat.st_size/1073741824));
                }

            printf(" %30s %s", file->d_name, ctime(&fileStat.st_mtime));

        }
        else {
            puts("�� ������� �������� ������ � ��������� stat");
            exit(2);
        }
    }
    // ������� �� ����� ����� ���� ������ � ����������
    if(total < 1024)                 // ��� �������� ���������� ��������� ������� ����������� � ������
                {
                    printf("����� ����� ������ � ����������: %10ld B \n", (long)total);
                }
                if(total > 1024)     // ��� �������� ���������� ��������� ������� ����������� � �������
                {
                    printf("����� ����� ������ � ����������: %10ld KB\n", (long)total/1024);
                }
                if(total > 1048576)   // ��� �������� ���������� ��������� ������� ����������� � �������
                {
                    printf("����� ����� ������ � ����������: %10ld MB\n", ((long)total/1048576));
                }
                if(total > 1073741824)  // ��� �������� ���������� ��������� ������� ����������� � �������
                {
                    printf("����� ����� ������ � ����������: %10ld GB\n", ((long)total/1073741824));
                }

    if(closedir(folder) != 0)
    {
        printf("���������� ������� ���������� \"%s\"\n", pathDir);
        system("pause");
        exit(2);
    }
    closedir(folder);
    printf("������� ���������� \"%s\"\n", pathDir);
    printf("����� ���������\n");
    return 0;
}
