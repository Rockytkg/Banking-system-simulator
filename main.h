#ifndef C_HOME_MAIN_H
#define C_HOME_MAIN_H

#endif //C_HOME_MAIN_H
//头文件
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include <tchar.h>
#include <math.h>

//函数声明
//暂停时间
void countdown(int pause_time);

//列表函数
void menu1();

void menu2();

int menu3();

void menu4();

void menu5();

//校验函数
void open_file(FILE *fp);

//隐藏输入密码，用*代替
void hide_password(char *pswd, unsigned maxlen);

//密码验证函数
int verify_password();

//修改密码
void change_password();

//记录模块
void recording(char *str);

//查询记录
void print_recording();

//取款
void withdraw();

// 存款函数
void deposit();

int love();