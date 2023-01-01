#include "main.h"

// ������ļ��Ƿ���ڣ���������ڣ��򴴽��ļ���������д������ 10000
void check_balance_file() {
    // �ж��ļ��Ƿ����
    FILE *fp = fopen("balance.txt", "r");
    if (fp == NULL) {
        // ����ļ������ڣ��򴴽��ļ���д������ 10000
        fp = fopen("balance.txt", "w");
        open_file(fp);
        fprintf(fp, "10000");
        fclose(fp);
        return;
    }
    // ʹ�ö�̬�������洢�ļ��е�����
    char *content = NULL;
    size_t len = 0;
    int balance = 0, i, flag = 0, c;
    while ((c = fgetc(fp)) != EOF) {
        // �����հ��ַ�
        if (isspace(c)) {
            continue;
        }
        len++;
        char *temp = realloc(content, len * sizeof(char));
        if (temp == NULL) {
            free(content);
            fclose(fp);
            printf("���ش��󣡣���");
            exit(0);
        }
        content = temp;
        content[len - 1] = c;
    }
    fclose(fp);
    // ���������е�ÿ���ַ������ж��Ƿ�������
    for (i = 0; i < len; i++) {
        if (isdigit(content[i])) {
            balance = balance * 10 + content[i] - '0';
        } else {
            flag = 1;
            break;
        }
    }
    // ��������˷������ַ�������Ĭ�����(10000)������
    if (flag == 1 || balance <= 0) {
        fp = fopen("balance.txt", "w");
        open_file(fp);
        if (fprintf(fp, "%d", 10000) < 0) {
            fprintf(stderr, "�����޷�д��balance.txt\n");
        }
        fclose(fp);
    }
    // �ͷŶ�̬����
    free(content);
}

// �ж������ļ��Ƿ���ڣ�������ڣ����ж������Ƿ�Ϊ 6 λ���ֵ��ַ�����������ǣ����������Ϊ�ַ��� 123456
void check_password_file() {
    // �ж��ļ��Ƿ����
    FILE *fp = fopen("password.txt", "r");
    if (fp == NULL) {
        // ����ļ������ڣ��򴴽��ļ���д���ַ��� 123456
        fp = fopen("password.txt", "w");
        open_file(fp);
        fprintf(fp, "123456");
        fclose(fp);
    } else {
        // ����ļ����ڣ����ȡ����
        char *password = malloc(1), c;
        int i = 0;
        while ((c = fgetc(fp)) != EOF) {
            password[i++] = c;
            password = realloc(password, i + 1);
        }
        password[i] = '\0';
        fclose(fp);
        // �ж������Ƿ�Ϊ 6 λ���ֵ��ַ���
        if (strlen(password) != 6 || !isdigit(password[0]) ||
            !isdigit(password[1]) || !isdigit(password[2]) ||
            !isdigit(password[3]) || !isdigit(password[4]) || !isdigit(password[5])) {
            // ������ǣ����������Ϊ�ַ��� 123456
            fp = fopen("password.txt", "w");
            open_file(fp);
            fprintf(fp, "123456");
            fclose(fp);
        }
        free(password);
    }
}

int main() {
    setbuf(stdout, NULL);
    menu1();
    menu5();
    check_password_file();
    check_balance_file();
    switch (verify_password()) {
        case 1:
            menu2();
            break;
        default:
            getchar();
            return 0;
    }
    love();
    return 0;
}