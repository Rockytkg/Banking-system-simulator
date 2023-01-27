#include "functions.h"

int input_select() {
    saveBankAccounts();
    char ch, choose;
    int index = 0;
    while ((ch = getch()) != '\r' || index == 0) {
        if (isdigit(ch)) {
            if (index < 1) {
                putchar(ch);
                choose = ch;
                index++;
            }
        } else if (ch == '\b' && index != 0) {
            index--;
            printf("\b \b"); // �������ǰ�ƶ�һλ�������ַ����
        }
    }
    index = atoi(&choose);
    printf("\n");
    return index;
}

void menu1() {
    system("cls");//����
    time_t t;
    struct tm *p;
    time(&t);
    p = gmtime(&t);
    printf("\t\t\t\t\033[1m**********************************************\n");
    printf("\t\t\t\t**--------------\033[31m%d��%02d��%02d��\033[0m\033[1m--------------**\n", 1900 + p->tm_year,
           1 + p->tm_mon, p->tm_mday);
    printf("\t\t\t\t****************��ǰʱ��%02dʱ%02d��**************\n", 8 + p->tm_hour, p->tm_min);
    printf("\t\t\t\t**********************************************\n");
    printf("\t\t\t\t**                                          **\n");
    printf("\t\t\t\t**\033[32m               ATMģ��ϵͳ                \033[0m\033[1m**\n");
    printf("\t\t\t\t**                                          **\n");
    printf("\t\t\t\t**********************************************\n");
    printf("\t\t\t\t**\033[1;31m     ��лʹ�ñ�ϵͳ��ϣ����ʹ����죡     \033[0m\033[1m**\n");
    printf("\t\t\t\t**********************************************\n");
    printf("\t\t\t\t**    �����ߣ���**    ѧ�ţ�22041132**      **\n");
    printf("\t\t\t\t**           �༶����Ӧ����2��              **\n");
    printf("\t\t\t\t**           ѧԺ��xxxxxxѧԺ               **\n");
    printf("\t\t\t\t**********************************************\n");
    countdown(60);
}

void menu2() {
    int ch;
    while (1) {
        menu4();
        ch = input_select();
        countdown(0);
        switch (ch) {
            case 1:
                deposit();
                break;
            case 2:
                withdraw();
                break;
            case 3:
                makeTransfer();
                break;
            case 4:
                change_password();
                break;
            case 5:
                queryBalance();
                break;
            case 6:
                deleteAccount();
                printTransactions();
                break;
            case 7:
                printTransactions();
                break;
            case 8:
                return;
            default:
                printf("�������벻�Ϸ�������������:");
        }
    }
}

int menu3() {
    int choose;
    printf("\t\t\t\t���������������������������������������������������������������������������������������������\n\n");
    printf("\t\t\t\t             | 1. �����ϼ� |\n\n");
    printf("\t\t\t\t             | 2. �������� |\n\n");
    printf("\t\t\t\t���������������������������������������������������������������������������������������������\n");
    printf("��ѡ��:");
    while (1) {
        choose = input_select();
        if (choose == 1 || choose == 2) {
            countdown(0);
            return choose;
        }
        printf("�������벻�Ϸ�������������:");
    }
}

void menu4() {
    printf("\t\t\t\t���������������������������������������������������������������������������������������������\n\n");
    printf("\t\t\t\t   | 1. ���       2. ȡ��       3. ת��     |\n\n");
    printf("\t\t\t\t   | 4. �޸�����   5. ��ѯ���   6. ɾ���˻� |\n\n");
    printf("\t\t\t\t   | 7. ��ѯ���׼�¼             8. �˳�ϵͳ |\n\n");
    printf("\t\t\t\t���������������������������������������������������������������������������������������������\n");
    printf("��ѡ��:");
}

void menu5() {
    printf("\033[1;31m����˵����\033[0m\n\n");
    printf("\033[1m��������ڵ�ǰĿ¼�´���\033[32m'AESBankAccounts.txt'\033[0m�ļ������ڴ洢���ݣ�����Ѿ���������ļ��������ñ��ݣ�\n");
    printf("\033[1m��������ڵ�ǰĿ¼�´���\033[32m'key.bin'\033[0m�ļ������ڴ洢��Կ������Ѿ���������ļ��������ñ��ݣ�\n");
    printf("��Ϊ��������ܻ�\033[1;31m�޸�\033[0m\033[1m����ļ�������(��ʾ������ʹ�ñ������������ɾ��)\n\n");
    printf("\033[1;31m���棺\n");
    printf("�粻���ܣ��������رմ˳��򣡣���\n\n\033[0m");
    countdown(60);
}
