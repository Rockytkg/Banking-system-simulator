#include "functions.h"

//名为 head 的全局变量，该变量将指向链表的第一个账户
BankAccount *head = NULL;
BankAccount *currents = NULL;

//打印当前链表中自定义用户开始以后的的所有数据
void printBankAccount(BankAccount *account) {
    while (account != NULL) {
        printf("账号: %s\n", account->accountNumber);
        printf("户主: %s\n", account->accountHolderName);
        printf("开户银行: %s\n", account->bankName);
        printf("密码: %s\n", account->password);
        printf("余额: %llu.%d 元\n", account->balance.dollars, account->balance.cents);
        printf("操作记录:\n");
        Transaction *trans = account->transactions;
        if (trans == NULL)
            printf("没有操作记录！！！\n");
        while (trans != NULL) {
            printf("\t%s\n", trans->message);
            trans = trans->next;
        }
        account = account->next;
    }
}

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
        //如果读取到换行符，表示一行数据读取完成
        if (c == '\n') {
            //结尾添加空字符
            line[i] = '\0';
            //按照逗号分割数据
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
    // 特别处理最后一个用户指向下一个用户的指针
    if (current != NULL)
        current->next = NULL;
    free(line);
    fclose(file);
    //printBankAccount(head);
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