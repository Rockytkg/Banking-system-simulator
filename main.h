#ifndef C_HOME_MAIN_H
#define C_HOME_MAIN_H

#endif //C_HOME_MAIN_H
//ͷ�ļ�
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include <tchar.h>
#include <math.h>

//��������
//��ͣʱ��
void countdown(int pause_time);

//�б���
void menu1();

void menu2();

int menu3();

void menu4();

void menu5();

//У�麯��
void open_file(FILE *fp);

//�����������룬��*����
void hide_password(char *pswd, unsigned maxlen);

//������֤����
int verify_password();

//�޸�����
void change_password();

//��¼ģ��
void recording(char *str);

//��ѯ��¼
void print_recording();

//ȡ��
void withdraw();

// ����
void deposit();

int love();