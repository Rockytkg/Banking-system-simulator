#include "main.h"

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

//��ͣʱ��
void countdown(int pause_time) {
    int sleep_time, actual_time;
    if (pause_time > 0) {
        // ����ṩ�˲�����ʹ�ò���ָ��������ʱ��
        sleep_time = pause_time;
        printf("���� \033[1;32mEnter\033[0m ����ȴ� \033[1;32m����ʱ\033[0m \033[1;31m����\033[0m ���Լ�������\n");
        while (sleep_time > 0) {
            sleep_time--;
            printf("\r����ʱ %d �� ", sleep_time);
            fflush(stdout);
            if (kbhit()) {
                char input = getchar();
                if (input == '\n') {
                    break;
                }
            }
            Sleep(1000);
        }
    } else {
        // ����ʹ�����������һ��ָ����Χ�ڵ�����ʱ��
        srand(time(NULL));
        sleep_time = rand() % (25 - 15 + 1) + 10;
        for (actual_time = 30; actual_time > sleep_time; actual_time--) {
            printf("���ڲ��������Ե� %d \r", actual_time);
            fflush(stdout);
            Sleep(1000);
        }
    }
    system("cls");
}

// �����������룬��*����
void hide_password(char *pswd, unsigned maxlen) {
    int index = 0;
    char buff;
    // ��������ַ����ǻس�������������ַ�����û�дﵽ��󳤶�ʱ����ѭ��
    while ((buff = getch()) != '\r' || index < maxlen - 1) {
        // ����ַ��Ƿ�Ϊ���֣��������������ַ����˸񣬲������������ַ�����ɾ�����һ���ַ�
        if (isdigit(buff)) {
            // ���������ַ�����û�дﵽ��󳤶ȣ����ַ�����������
            if (index < maxlen - 1) {
                pswd[index++] = buff;
                putchar('*'); // ��������ַ��滻�� *
            }
        } else if (buff == '\b' && index != 0) {
            index--;
            printf("\b \b"); // �������ǰ�ƶ�һλ�������ַ����
        }
    }
    pswd[index] = '\0'; // �������ĩβ������ֹ��
    printf("\n");
}

//������֤����ȷ����1�����󷵻�0
int verify_password() {
    char key[7], password[7];
    int sum = 3;
    // ���ļ�
    FILE *fp = fopen("password.txt", "r");
    open_file(fp);
    // ��ȡ���벢д������
    fscanf(fp, "%s", key);
    fclose(fp);
    printf("����������(��ʼ����Ϊ123456):");
    while (1) {
        hide_password(password, 7);
        countdown(0);
        sum--;
        if (sum > 0) {
            if (strcmp(key, password) == 0) {
                break;
            } else {
                system("cls");
                printf("��������㻹�� %d �λ���\n", sum);
                printf("��������������:");
            }
        } else {
            printf("�������3�Σ�����ϵ���й�����Աȡ��");
            return 0;
        }
    }
    system("cls");
    return 1;
}

//��¼ģ��
void recording(char *str) {
    FILE *fp = fopen("recording.txt", "a");
    open_file(fp);
    // ��ȡ��ǰʱ��
    time_t t = time(NULL);
    struct tm *timeinfo = localtime(&t);
    // ��ʱ��ת��Ϊ 24 Сʱ���ַ���
    char timestr[20];
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", timeinfo);
    // д��ʱ����ַ���
    fprintf(fp, "[%s] %s\n", timestr, str);
    // �ر��ļ�
    fclose(fp);
}

//��ѯ��¼
void print_recording() {
    int choose = 2, balance;
    while (choose == 2) {
        if (access("recording.txt", F_OK) == -1) {
            // �ļ�������
            FILE *fa = fopen("balance.txt", "r");
            open_file(fa);
            fscanf(fa, "%d", &balance);
            printf("����ǰ���Ϊ %d Ԫ\n", balance);
            fclose(fa);
            printf("û�в�����¼������\n");
            goto anew;
        }
        // ���ļ�
        FILE *fp = fopen("recording.txt", "r");
        FILE *fa = fopen("balance.txt", "r");
        open_file(fp);
        open_file(fa);
        fscanf(fa, "%d", &balance);
        printf("����ǰ���Ϊ %d Ԫ\n", balance);
        fclose(fa);
        // �ж��ļ��Ƿ�Ϊ��
        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        rewind(fp);
        if (file_size == 0) {
            printf("û�в�����¼������\n");
            goto anew;
        } else {
            // ���ж�ȡ�ļ�����
            char line[1024];
            while (fgets(line, sizeof(line), fp) != NULL) {
                // ���������
                printf("%s", line);
            }
        }
        // �ر��ļ�
        fclose(fp);
        anew:
        choose = menu3();
    }
}

//�޸�����
void change_password() {
    int choose = 2;
    while (choose == 2) {
        // ���ļ�
        FILE *fp = fopen("password.txt", "r+");
        open_file(fp);
        char new_password1[7], new_password2[7];
        printf("������������:");
        while (1) {
            hide_password(new_password1, 7);
            printf("���ٴ�����������:");
            hide_password(new_password2, 7);
            if (strcmp(new_password1, new_password2) == 0) {
                break;
            } else {
                printf("�������벻һ�£�����������:");
            }
        }
        // ���ļ�ָ���ƶ����ļ���ͷ
        rewind(fp);
        // д��������
        fprintf(fp, "%s", new_password1);
        // �ر��ļ�
        fclose(fp);
        choose = menu3();
    }
}

//ȡ��
void withdraw() {
    // ���ļ��ж�ȡ���
    int balance, amount, choose;
    FILE *fp = fopen("balance.txt", "r");
    open_file(fp);
    fscanf(fp, "%d", &balance);
    fclose(fp);
    do {
        printf("������ȡ����:");
        scanf("%d", &amount);
        while (amount <= 0 || amount % 10 != 0) {
            printf("�������벻�Ϸ�,����������:");
            scanf("%d", &amount);
        }
        countdown(0);
        // �ж�����Ƿ��㹻
        if (balance < amount) {
            printf("����!!!\n");
            goto anew;
        }
        // ִ��ȡ�����
        balance -= amount;
        printf("�ɹ�ȡ�� %d Ԫ\n", amount);
        // д�������¼
        char str[50];
        sprintf(str, "ȡ�� %d Ԫ", amount);
        recording(str);
        // ���µ����д���ļ�
        fp = fopen("balance.txt", "w");
        open_file(fp);
        fprintf(fp, "%d", balance);
        fclose(fp);
        anew:
        choose = menu3();
    } while (choose == 2);
}

// ����
void deposit() {
    // �� balance.txt �ļ�
    FILE *fp = fopen("balance.txt", "r");
    open_file(fp);
    // �� balance.txt �ļ��ж�ȡ֮ǰ�����
    int balance, amount, choose;
    fscanf(fp, "%d", &balance);
    // �ر��ļ�
    fclose(fp);
    do {
        printf("����������:");
        scanf("%d", &amount);
        countdown(0);
        // У�����������Ƿ�Ϸ�
        while (amount < 0 || amount % 10 != 0) {
            // ���Ϸ������������Ϣ��Ҫ���û���������
            printf("���󣺽����Ч\n��������������:");
            scanf("%d", &amount);
            countdown(0);
        }
        // �����µ����
        balance += amount;
        // д�������¼
        char str[50];
        sprintf(str, "��� %d Ԫ", amount);
        recording(str);
        // �� balance.txt �ļ�������д��
        fp = fopen("balance.txt", "w");
        open_file(fp);
        // ���µ����д�� balance.txt �ļ�
        fprintf(fp, "%d", balance);
        // �ر��ļ�
        choose = menu3();
        fclose(fp);
    } while (choose == 2);
}