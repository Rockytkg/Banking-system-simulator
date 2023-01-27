#ifndef C_HOME_MAIN_H
#define C_HOME_MAIN_H

#endif //C_HOME_MAIN_H

//ͷ�ļ�
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include "windows.h"
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <tchar.h>
#include <math.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/md5.h>
#include <openssl/aes.h>

//����ṹ��
typedef struct Balance {
    unsigned long long dollars;
    int cents;
} Balance;

typedef struct Transaction {
    char message[100];
    struct Transaction *next;
} Transaction;

typedef struct BankAccount {
    char accountNumber[11];
    char *accountHolderName;
    char *bankName;
    char password[7];
    Balance balance;
    Transaction *transactions;
    struct BankAccount *next;
} BankAccount;

//��������
//��ͣʱ��
void countdown(int pause_time);

//�б���
void menu1();

void menu2();

int menu3();

void menu4();

void menu5();

//����ѡ��
int input_select();

//У�麯��
void open_file(FILE *fp);

//�����������룬��*����
void hide_password(char *key);

// �����˻�
void input_account(char *id);

//������֤����
int verify_password();

//�޸�����
void change_password();

//��ѯ���
void queryBalance();

//ȡ��
void withdraw();

// ����
void deposit();

//�����µ��˻�
BankAccount *createAccount(char *accountNumber, char *accountHolderName, char *bankName, char *password);

//��������˻�����µĽ�����Ϣ
void addTransaction(char *message);

//��ӡ�����˻������н�����Ϣ
void printTransactions();

//�ͷŸ������˻��������ڴ�
void freeAccount(BankAccount *account);

//�����˻�
BankAccount *findAccount(char *accountNumber);

//ɾ���˻�
void deleteAccount();

//�����������⾫�ȶ�ʧ������
int processBankAccount(int operation, unsigned long long integer, int fraction);

//��ʽ��������������ȥ���������0
void remove_trailing_zeros(char *amount, int i);

//����ʽ���������
void input_number(int i);

//��������Ĳ���������������
void parse_string(char *str, int i);

//��ʼ���û�����
int DocumentVerification();

// ��ջ�����
void clear_kb_buffer();

// ���������ݴ洢���ļ���
void saveBankAccounts();

// ���ļ��ж�ȡ��������
void loadBankAccounts();

//��ӡ��ǰ�������Զ����û���ʼ�Ժ�ĵ���������
void printBankAccount(BankAccount *account);

// �����ļ�
void encrypt_file();

// �����ļ�
void decrypt_file();

// ��ȡ��Կ
void generateKey();