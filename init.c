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

// ���������ݴ洢���ļ���
void saveBankAccounts() {
    FILE *file = fopen("bankAccounts.dat", "wb");
    open_file(file);
    BankAccount *current = head;
    while (current != NULL) {
        int nameLength = strlen(current->accountHolderName);
        int bankLength = strlen(current->bankName);
        fwrite(current->accountNumber, sizeof(char), 11, file);
        fwrite(&nameLength, sizeof(int), 1, file);
        fwrite(current->accountHolderName, sizeof(char), nameLength, file);
        fwrite(&bankLength, sizeof(int), 1, file);
        fwrite(current->bankName, sizeof(char), bankLength, file);
        fwrite(current->password, sizeof(char), 7, file);
        fwrite(&(current->balance), sizeof(Balance), 1, file);
        Transaction *trans = current->transactions;
        while (trans != NULL) {
            int messageLength = strlen(trans->message);
            fwrite(&messageLength, sizeof(int), 1, file);
            fwrite(trans->message, sizeof(char), messageLength, file);
            trans = trans->next;
        }
        int end = -1;
        fwrite(&end, sizeof(int), 1, file);
        current = current->next;
    }
    fclose(file);
}


// ���ļ��ж�ȡ��������
void loadBankAccounts() {
    FILE *file = fopen("bankAccounts.dat", "rb");
    open_file(file);
    BankAccount *current = NULL;
    while (1) {
        BankAccount *newAccount = (BankAccount *) malloc(sizeof(BankAccount));
        int nameLength, bankLength, messageLength, end;
        fread(newAccount->accountNumber, sizeof(char), 11, file);
        if (feof(file)) {
            free(newAccount);
            break;
        }
        fread(&nameLength, sizeof(int), 1, file);
        newAccount->accountHolderName = (char *) malloc(nameLength + 1);
        fread(newAccount->accountHolderName, sizeof(char), nameLength, file);
        newAccount->accountHolderName[nameLength] = '\0';
        fread(&bankLength, sizeof(int), 1, file);
        newAccount->bankName = (char *) malloc(bankLength + 1);
        fread(newAccount->bankName, sizeof(char), bankLength, file);
        newAccount->bankName[bankLength] = '\0';
        fread(newAccount->password, sizeof(char), 7, file);
        fread(&(newAccount->balance), sizeof(Balance), 1, file);
        newAccount->transactions = NULL;
        Transaction *trans = NULL;
        while (1) {
            fread(&messageLength, sizeof(int), 1, file);
            if (messageLength == -1) {
                break;
            }
            Transaction *newTrans = (Transaction *) malloc(sizeof(Transaction));
            newTrans->next = NULL;
            fread(newTrans->message, sizeof(char), messageLength, file);
            newTrans->message[messageLength] = '\0';
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
    fclose(file);
}

//��ʼ���û�����
int DocumentVerification() {
    if (fopen("bankAccounts.dat", "rb") == NULL) {
        createAccount("1234567890", "���̩", "�黨����", "123456");
        createAccount("2234567890", "��̩", "�黨����", "123456");
        createAccount("3234567890", "̩��", "�黨����", "123456");
        saveBankAccounts();
        return 0;
    }
    return 1;
}