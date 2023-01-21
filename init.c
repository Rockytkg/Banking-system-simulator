#include "functions.h"

//名为 head 的全局变量，该变量将指向链表的第一个账户
BankAccount *head = NULL;
BankAccount *currents = NULL;

//校验函数
void open_file(FILE *fp) {
    if (fp == NULL) {
        system("cls");
        printf("\033[1;31m警告：\033[0m\n\n");
        printf("\033[1m程序异常，打开文件时出错！！！");
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

// 从文件中读取链表数据
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


//初始化用户数据
int DocumentVerification() {
    if (fopen("bankAccounts.txt", "r") == NULL) {
        createAccount("1234567890", "马宏泰", "麻花银行", "123456");
        createAccount("2234567890", "马泰", "麻花银行", "123456");
        createAccount("3234567890", "泰宏", "麻花银行", "123456");
        saveBankAccounts();
        return 0;
    }
    return 1;
}