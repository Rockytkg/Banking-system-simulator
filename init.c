#include "functions.h"

//��Ϊ head ��ȫ�ֱ������ñ�����ָ������ĵ�һ���˻�
BankAccount *head = NULL;
BankAccount *currents = NULL;

//��ӡ��ǰ�������Զ����û���ʼ�Ժ�ĵ���������
void printBankAccount(BankAccount *account) {
    while (account != NULL) {
        printf("�˺�: %s\n", account->accountNumber);
        printf("����: %s\n", account->accountHolderName);
        printf("��������: %s\n", account->bankName);
        printf("����: %s\n", account->password);
        printf("���: %llu.%d Ԫ\n", account->balance.dollars, account->balance.cents);
        printf("������¼:\n");
        Transaction *trans = account->transactions;
        if (trans == NULL)
            printf("û�в�����¼������\n");
        while (trans != NULL) {
            printf("\t%s\n", trans->message);
            trans = trans->next;
        }
        account = account->next;
    }
}

//У�麯��
void open_file(FILE *fp) {
    if (fp == NULL) {
        system("cls");
        printf("\033[1;31m���棺\033[0m\n\n");
        printf("\033[1m�����쳣�����ļ�ʱ��������");
        getchar();
        exit(0);
    }
}

void saveBankAccounts() {
    FILE *file = fopen("bankAccounts.txt", "w");
    open_file(file);
    BankAccount *current = head;
    while (current != NULL) {
        int nameLength = strlen(current->accountHolderName);
        int bankLength = strlen(current->bankName);
        fprintf(file, "%s,%d,%s,%d,%s,%s,%llu.%d:", current->accountNumber, nameLength, current->accountHolderName,
                bankLength, current->bankName, current->password, current->balance.dollars, current->balance.cents);
        Transaction *trans = current->transactions;
        while (trans != NULL) {
            fprintf(file, "%s,", trans->message);
            trans = trans->next;
        }
        fprintf(file, "-1\n");
        current = current->next;
    }
    fclose(file);
}

void loadBankAccounts() {
    FILE *file = fopen("bankAccounts.txt", "r");
    open_file(file);
    BankAccount *current = NULL;
    char c;
    int i = 0;
    char *line = NULL;
    int len = 0;
    while ((c = fgetc(file)) != EOF) {
        //�����ȡ�����з�����ʾһ�����ݶ�ȡ���
        if (c == '\n') {
            //��β��ӿ��ַ�
            line[i] = '\0';
            //���ն��ŷָ�����
            char *token = strtok(line, ",");
            BankAccount *newAccount = (BankAccount *) malloc(sizeof(BankAccount));
            strcpy(newAccount->accountNumber, token);
            token = strtok(NULL, ",");
            int nameLength = atoi(token);
            token = strtok(NULL, ",");
            newAccount->accountHolderName = (char *) malloc(sizeof(char) * (nameLength + 1));
            strcpy(newAccount->accountHolderName, token);
            token = strtok(NULL, ",");
            int bankNameLength = atoi(token);
            token = strtok(NULL, ",");
            newAccount->bankName = (char *) malloc(sizeof(char) * (bankNameLength + 1));
            strcpy(newAccount->bankName, token);
            token = strtok(NULL, ",");
            strcpy(newAccount->password, token);
            token = strtok(NULL, ".");
            newAccount->balance.dollars = atoi(token);
            token = strtok(NULL, ":");
            newAccount->balance.cents = atoi(token);
            newAccount->transactions = NULL;
            Transaction *currentTransaction = NULL;
            token = strtok(NULL, ",");
            while (strcmp(token, "-1") != 0) {
                if (newAccount->transactions == NULL) {
                    newAccount->transactions = (Transaction *) malloc(sizeof(Transaction));
                    currentTransaction = newAccount->transactions;
                } else {
                    currentTransaction->next = (Transaction *) malloc(sizeof(Transaction));
                    currentTransaction = currentTransaction->next;
                }
                strcpy(currentTransaction->message, token);
                currentTransaction->next = NULL;
                token = strtok(NULL, ",");
            }
            if (head == NULL) {
                head = newAccount;
            } else {
                current->next = newAccount;
            }
            current = newAccount;
            i = 0;
            free(line);
            line = NULL;
            len = 0;
        } else {
            if (i + 1 > len) {
                len = 2 * len + 1;
                line = (char *) realloc(line, sizeof(char) * len);
            }
            line[i++] = c;
        }
    }
    // �ر������һ���û�ָ����һ���û���ָ��
    if (current != NULL)
        current->next = NULL;
    free(line);
    fclose(file);
    //printBankAccount(head);
}

//��ʼ���û�����
int DocumentVerification() {
    if (fopen("bankAccounts.txt", "r") == NULL) {
        createAccount("1234567890", "���̩", "�黨����", "123456");
        createAccount("2234567890", "��̩", "�黨����", "123456");
        createAccount("3234567890", "̩��", "�黨����", "123456");
        saveBankAccounts();
        return 0;
    }
    return 1;
}