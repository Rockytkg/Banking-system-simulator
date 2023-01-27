#ifndef C_HOME_MAIN_H
#define C_HOME_MAIN_H

#endif //C_HOME_MAIN_H

//头文件
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

//定义结构体
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

//函数声明
//暂停时间
void countdown(int pause_time);

//列表函数
void menu1();

void menu2();

int menu3();

void menu4();

void menu5();

//输入选择
int input_select();

//校验函数
void open_file(FILE *fp);

//隐藏输入密码，用*代替
void hide_password(char *key);

// 输入账户
void input_account(char *id);

//密码验证函数
int verify_password();

//修改密码
void change_password();

//查询余额
void queryBalance();

//取款
void withdraw();

// 存款函数
void deposit();

//创建新的账户
BankAccount *createAccount(char *accountNumber, char *accountHolderName, char *bankName, char *password);

//向给定的账户添加新的交易信息
void addTransaction(char *message);

//打印给定账户的所有交易信息
void printTransactions();

//释放给定的账户的所有内存
void freeAccount(BankAccount *account);

//查找账户
BankAccount *findAccount(char *accountNumber);

//删除账户
void deleteAccount();

//余额处理函数，避免精度丢失的问题
int processBankAccount(int operation, unsigned long long integer, int fraction);

//格式化操作数，用来去掉无意义的0
void remove_trailing_zeros(char *amount, int i);

//按格式输入操作数
void input_number(int i);

//处理输入的操作数方便余额计算
void parse_string(char *str, int i);

//初始化用户数据
int DocumentVerification();

// 清空缓存区
void clear_kb_buffer();

// 将链表数据存储在文件中
void saveBankAccounts();

// 从文件中读取链表数据
void loadBankAccounts();

//打印当前链表中自定义用户开始以后的的所有数据
void printBankAccount(BankAccount *account);

// 加密文件
void encrypt_file();

// 解密文件
void decrypt_file();

// 读取密钥
void generateKey();