#include "functions.h"

//名为 head 的全局变量，该变量将指向链表的第一个账户
BankAccount *head = NULL;
BankAccount *currents = NULL;
unsigned char key[16];

//打印当前链表中自定义用户开始以后的的所有数据
void printBankAccount(BankAccount *account) {
    printf("\033[32m提示\033[0m\033[1m：当前系统中用户有：\n");
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
            printf("%s\n", trans->message);
            trans = trans->next;
        }
        printf("\n");
        account = account->next;
    }
    countdown(120);
}

//校验函数
void open_file(FILE *fp) {
    if (fp == NULL) {
        system("cls");
        printf("\033[1;31m警告：\033[0m\n\n");
        printf("\033[1m程序异常，打开文件时出错！！！\n");
        printf("\033[1m请删除所有数据文件后重试。");
        clear_kb_buffer();
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
    encrypt_file();
}

void loadBankAccounts() {
    decrypt_file();
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
    remove("bankAccounts.txt");
    printBankAccount(head);
}

//初始化用户数据
int DocumentVerification() {
    if (fopen("AESBankAccounts.txt", "r") == NULL) {
        createAccount("1234567890", "马宏泰", "麻花银行", "123456");
        createAccount("2234567890", "马泰", "麻花银行", "123456");
        createAccount("3234567890", "泰宏", "麻花银行", "123456");
        saveBankAccounts();
        return 0;
    }
    return 1;
}

// 加密文件
void encrypt_file() {
    // 打开文件
    FILE *fin = fopen("bankAccounts.txt", "rb");
    FILE *fout = fopen("AESBankAccounts.txt", "wb");
    open_file(fin);
    open_file(fout);
    // 初始化AES加密器
    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit(ctx, EVP_aes_128_cbc(), key, NULL);
    // 加密文件
    unsigned char buffer[1024];
    int bytes_read;
    int len;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fin)) > 0) {
        EVP_EncryptUpdate(ctx, buffer, &len, buffer, bytes_read);
        fwrite(buffer, 1, len, fout);
    }
    EVP_EncryptFinal(ctx, buffer, &len);
    fwrite(buffer, 1, len, fout);
    // 关闭文件
    fclose(fin);
    fclose(fout);
    EVP_CIPHER_CTX_free(ctx);
    remove("bankAccounts.txt");
}

// 解密文件
void decrypt_file() {
    // 打开文件
    FILE *fin = fopen("AESBankAccounts.txt", "rb");
    FILE *fout = fopen("bankAccounts.txt", "wb");
    open_file(fin);
    open_file(fout);
    // 初始化AES解密器
    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit(ctx, EVP_aes_128_cbc(), key, NULL);
    // 解密文件
    unsigned char buffer[1024];
    int bytes_read;
    int len;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fin)) > 0) {
        EVP_DecryptUpdate(ctx, buffer, &len, buffer, bytes_read);
        fwrite(buffer, 1, len, fout);
    }
    EVP_DecryptFinal(ctx, buffer, &len);
    fwrite(buffer, 1, len, fout);
    // 关闭文件
    fclose(fin);
    fclose(fout);
    EVP_CIPHER_CTX_free(ctx);
    remove("AESBankAccounts.txt");
}

void generateKey() {
    FILE *fa = fopen("AESBankAccounts.txt", "rb");
    if (fa) {
        FILE *fp = fopen("key.bin", "rb");
        // key.bin文件不存在，抛出异常
        open_file(fp);
        // key.bin文件存在，从文件中读取密钥
        fread(key, 1, 16, fp);
        fclose(fp);
    } else {
        // AESBankAccounts.txt 和 key.bin文件不存在，生成随机密钥
        RAND_bytes(key, 16);
        FILE *fp = fopen("key.bin", "wb");
        fwrite(key, 1, 16, fp);
        fclose(fp);
    }
    RAND_cleanup();
}