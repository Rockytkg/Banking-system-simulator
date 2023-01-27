#include "functions.h"

//��Ϊ head ��ȫ�ֱ������ñ�����ָ������ĵ�һ���˻�
BankAccount *head = NULL;
BankAccount *currents = NULL;
unsigned char key[16];

//��ӡ��ǰ�������Զ����û���ʼ�Ժ�ĵ���������
void printBankAccount(BankAccount *account) {
    printf("\033[32m��ʾ\033[0m\033[1m����ǰϵͳ���û��У�\n");
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
            printf("%s\n", trans->message);
            trans = trans->next;
        }
        printf("\n");
        account = account->next;
    }
    countdown(120);
}

//У�麯��
void open_file(FILE *fp) {
    if (fp == NULL) {
        system("cls");
        printf("\033[1;31m���棺\033[0m\n\n");
        printf("\033[1m�����쳣�����ļ�ʱ��������\n");
        printf("\033[1m��ɾ�����������ļ������ԡ�");
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
    remove("bankAccounts.txt");
    printBankAccount(head);
}

//��ʼ���û�����
int DocumentVerification() {
    if (fopen("AESBankAccounts.txt", "r") == NULL) {
        createAccount("1234567890", "���̩", "�黨����", "123456");
        createAccount("2234567890", "��̩", "�黨����", "123456");
        createAccount("3234567890", "̩��", "�黨����", "123456");
        saveBankAccounts();
        return 0;
    }
    return 1;
}

// �����ļ�
void encrypt_file() {
    // ���ļ�
    FILE *fin = fopen("bankAccounts.txt", "rb");
    FILE *fout = fopen("AESBankAccounts.txt", "wb");
    open_file(fin);
    open_file(fout);
    // ��ʼ��AES������
    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit(ctx, EVP_aes_128_cbc(), key, NULL);
    // �����ļ�
    unsigned char buffer[1024];
    int bytes_read;
    int len;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fin)) > 0) {
        EVP_EncryptUpdate(ctx, buffer, &len, buffer, bytes_read);
        fwrite(buffer, 1, len, fout);
    }
    EVP_EncryptFinal(ctx, buffer, &len);
    fwrite(buffer, 1, len, fout);
    // �ر��ļ�
    fclose(fin);
    fclose(fout);
    EVP_CIPHER_CTX_free(ctx);
    remove("bankAccounts.txt");
}

// �����ļ�
void decrypt_file() {
    // ���ļ�
    FILE *fin = fopen("AESBankAccounts.txt", "rb");
    FILE *fout = fopen("bankAccounts.txt", "wb");
    open_file(fin);
    open_file(fout);
    // ��ʼ��AES������
    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit(ctx, EVP_aes_128_cbc(), key, NULL);
    // �����ļ�
    unsigned char buffer[1024];
    int bytes_read;
    int len;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fin)) > 0) {
        EVP_DecryptUpdate(ctx, buffer, &len, buffer, bytes_read);
        fwrite(buffer, 1, len, fout);
    }
    EVP_DecryptFinal(ctx, buffer, &len);
    fwrite(buffer, 1, len, fout);
    // �ر��ļ�
    fclose(fin);
    fclose(fout);
    EVP_CIPHER_CTX_free(ctx);
    remove("AESBankAccounts.txt");
}

void generateKey() {
    FILE *fa = fopen("AESBankAccounts.txt", "rb");
    if (fa) {
        FILE *fp = fopen("key.bin", "rb");
        // key.bin�ļ������ڣ��׳��쳣
        open_file(fp);
        // key.bin�ļ����ڣ����ļ��ж�ȡ��Կ
        fread(key, 1, 16, fp);
        fclose(fp);
    } else {
        // AESBankAccounts.txt �� key.bin�ļ������ڣ����������Կ
        RAND_bytes(key, 16);
        FILE *fp = fopen("key.bin", "wb");
        fwrite(key, 1, 16, fp);
        fclose(fp);
    }
    RAND_cleanup();
}