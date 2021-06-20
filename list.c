// ������� ������� ���� �� ������ ���������� ������.
// ��������� ����� ������� ��� ���������� �������� ������ � ���� � ��� ��������

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

char * filename = "list.dat"; /* ��� ����� ���� ����� ��������� ������*/


int showMenu(void);             // �������� ������� ������ �� ����� ����
void addNode(void);             // �������� ������� ���������� ������ ���� ������
void removeNode(void);          // �������� ������� �������� ���� ������
void showList(void);            // �������� ������� ������ �� ����� �������� ����
struct order * memAlloc(void);  // ��������� ������ ��� ������ ����
void getInput(int * result, int min, int max, char * welcome);  // ����� �������� ������ �������������

// ���. ������� �������� � ���������� ������
void loadList(void);
void saveList(void);

int main()
{
    // ���������� ������
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int choice = '\0';      // ������������� ������ ������ ������� ENTER
    first = NULL;

    loadList();             // ��������� ������ ��� ������� ���������

    while(choice != 'Q')    // ������ ������
    {
        choice = showMenu();
        switch (choice)
        {
        case 'S':           // ����� �� ����� ���� ������
            CLS;
            showList();
            break;
        case 'A':           // ���������� ������ ���� ������
            CLS;
            addNode();
            break;
        case 'R':           // �������� ���� ������
            CLS;
            removeNode();
            break;
        case 'Q':
            puts("�����...");
            saveList();     // ���������� �� ���� ������ ����� �������
            system("pause");
            break;

        default:
            CLS;
            puts("������������ ����");
            break;
        }
    }
}

// ������� ������ �� ����� ����
int showMenu(void)
{
    int ch = '\0';
    printf("A - �������� ���� ������\nR - ������� ���� ������\nS - �������� ������\nQ - �����\n");
    ch = getch();
    return(toupper(ch));
}

// ��������� ������ ��� ������ ���� ������
struct order * memAlloc(void)
{
    struct order * temp;
    temp = (struct order *)malloc(sizeof(struct order));
    if(temp == NULL)                                // �������� ��������� ������
    {
        puts("������ ��������� ������!!! �����...\n");   // ���� ��� �� ������� ������
        system("pause");
        exit(1);
    }
    return temp;
}

// ������� ���������� ������ ����
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
        newNode = memAlloc();           // ��������� ����������
        current->next = newNode;
        current = newNode;
    }
    current->next = NULL;

    getInput(&current->number, 0, 1000000, "Enter order number: \n");
    getInput(&current->sum, 0, 4000000, "Enter sum of order: \n");
}

// ����� �� ����� ����������� �����
void showList(void)
{
    int count = 1;
    // �������� �� �������
    if(first == NULL)
    {
        puts("������ ����\n");
        return;
    }
    puts("������: \n");

    current = first;
    while(current != NULL)
    {
        printf("���� #%d: ����� #%d, ����� %d\n",
               count, current->number, current->sum);
        current = current->next;
        count++;
    }
}

// �������� �������� ����
void removeNode(void)
{
    struct order * prev;
    int rem = 0;
    int num;

    if(first == NULL)
    {
        puts("������ �������! ������ ����.\n");
        return;
    }
    showList();

    getInput(&rem, 1, 1000000, "�������� ����� ���� ��� ��������: \n");

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
            puts("���� �� ������.\n");
            return;
        }
    }
    if(prev == NULL)
        first = current->next;
    else
        prev->next=current->next;
    printf("���� #%d ������\n", rem);
    free(current);
}

// ��������� ������ �����
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

// ������� �������� � ���������� ������ ������
void loadList(void)
{
	FILE * fptr;                  // ��������� �� ��� ����
	struct order loaded;          // ��������� ������� ����� ��������� �� ������ �����
	int records = 0;              // ���������� ����������� ���������

	fptr = fopen(filename, "rb"); // �������� ����� � ������ ������
	if(fptr == NULL)              // ���� ������ ����� ���, �� ������� �� �������
		return;

	// ����������  ���� ������� �� ����� �� �����. ���� ���������� ���-�� ��������� ���������
	while( fread(&loaded, sizeof(struct order), 1, fptr) )
	{
	    // ������� ������ ��������� ������ ������ (������ addNode) ��� ���������� �������� � ���� ������ ������
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

		// ��������� ������� ������� ������� ������������ �� �����
		current->number = loaded.number;
		current->sum = loaded.sum;
		// ��������� �� ���� ������� ������ ��������� �� �����

		current->next = NULL;   // ��������� ������
		records++;              // ������� ���������� ����� ���������
	}
	fclose(fptr);
	printf("%d ������ �������� �� \'%s\'\n", records, filename);
}

// ������� ���������� ������ ������ � ����
void saveList(void)
{
	int records = 0;    // ���������� ���������� ���������
	FILE * fptr;        // ���� ��� ���������� ������

    fptr = fopen(filename, "wb");   // �������� ����� � ������ �������� ������/��������
	if(fptr == NULL)
	{
		puts("���������� ������� ���� ��� ���������� ������!\n");
		return;
	}

    // ���������� �������� � ���� �� ������ ������� � ������� ��������
    current = first;                // ���� ������ ���� (current == NULL), ���� �� �����������
    while(current)
    {
        fwrite(current, sizeof(struct order), 1, fptr);
        NEXT;
        records++;
    }

    fclose(fptr);
    printf("%d ������� �������� � \'%s\'\n", records, filename);
}
