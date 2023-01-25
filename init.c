#include "functions.h"

//��Ϊ head ��ȫ�ֱ������ñ�����ָ������ĵ�һ���˻�
BankAccount *head = NULL;
BankAccount *currents = NULL;

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
    FILE *file = fopen("bankAccounts.dat", "w");
    open_file(file);
    BankAccount *current = head;
    while (current != NULL) {
        int nameLength = strlen(current->accountHolderName);
        int bankLength = strlen(current->bankName);
        fprintf(file, "%s,%d,%s,%d,%s,%s,%llu.%d:", current->accountNumber, nameLength, current->accountHolderName, bankLength, current->bankName, current->password, current->balance.dollars, current->balance.cents);
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

// ���ļ��ж�ȡ��������
void loadBankAccounts() {
    FILE *file = fopen("bankAccounts.dat", "r");
    open_file(file);
    BankAccount *current = NULL;
    char *line = NULL;
    size_t len = 0;
    while (fgets(line, len, file) != NULL) {
        BankAccount *newAccount = (BankAccount *) malloc(sizeof(BankAccount));
        char *token = strtok(line, ",");
        strcpy(newAccount->accountNumber, token);
        token = strtok(NULL, ",");
        int nameLength = atoi(token);
        token = strtok(NULL, ",");
        newAccount->accountHolderName = (char *) malloc(nameLength + 1);
        strcpy(newAccount->accountHolderName, token);
        token = strtok(NULL, ",");
        int bankLength = atoi(token);
        token = strtok(NULL, ",");
        newAccount->bankName = (char *) malloc(bankLength + 1);
        strcpy(newAccount->bankName, token);
        token = strtok(NULL, ",");
        strcpy(newAccount->password, token);
        token = strtok(NULL, ":");
        sscanf(token, "%llu.%d", &(newAccount->balance.dollars), &(newAccount->balance.cents));
        newAccount->transactions = NULL;
        Transaction *trans = NULL;
        while (1) {
            token = strtok(NULL, ",");
            if (strcmp(token, "-1") == 0) {
                break;
            }
            Transaction *newTrans = (Transaction *) malloc(sizeof(Transaction));
            newTrans->next = NULL;
            strcpy(newTrans->message, token);
            if (trans == NULL) {
                newAccount->transactions = newTrans;
            } else {
                trans->next = newTrans;
            }
            trans = newTrans;
        }
        newAccount->next = NULL;
        if (head == NULL) {
            head = newAccount;
        } else {
            current->next = newAccount;
        }
        current = newAccount;
    }
    if (line) {
        free(line);
    }
    fclose(file);
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