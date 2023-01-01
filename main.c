#include "main.h"

// 判余额文件是否存在，如果不存在，则创建文件并向里面写入数字 10000
void check_balance_file() {
    // 判断文件是否存在
    FILE *fp = fopen("balance.txt", "r");
    if (fp == NULL) {
        // 如果文件不存在，则创建文件并写入数字 10000
        fp = fopen("balance.txt", "w");
        open_file(fp);
        fprintf(fp, "10000");
        fclose(fp);
        return;
    }
    // 使用动态数组来存储文件中的内容
    char *content = NULL;
    size_t len = 0;
    int balance = 0, i, flag = 0, c;
    while ((c = fgetc(fp)) != EOF) {
        // 跳过空白字符
        if (isspace(c)) {
            continue;
        }
        len++;
        char *temp = realloc(content, len * sizeof(char));
        if (temp == NULL) {
            free(content);
            fclose(fp);
            printf("严重错误！！！");
            exit(0);
        }
        content = temp;
        content[len - 1] = c;
    }
    fclose(fp);
    // 遍历数组中的每个字符，并判断是否是数字
    for (i = 0; i < len; i++) {
        if (isdigit(content[i])) {
            balance = balance * 10 + content[i] - '0';
        } else {
            flag = 1;
            break;
        }
    }
    // 如果遇到了非数字字符，则用默认余额(10000)覆盖它
    if (flag == 1 || balance <= 0) {
        fp = fopen("balance.txt", "w");
        open_file(fp);
        if (fprintf(fp, "%d", 10000) < 0) {
            fprintf(stderr, "错误：无法写入balance.txt\n");
        }
        fclose(fp);
    }
    // 释放动态数组
    free(content);
}

// 判断密码文件是否存在，如果存在，则判断内容是否为 6 位数字的字符串，如果不是，则更改内容为字符串 123456
void check_password_file() {
    // 判断文件是否存在
    FILE *fp = fopen("password.txt", "r");
    if (fp == NULL) {
        // 如果文件不存在，则创建文件并写入字符串 123456
        fp = fopen("password.txt", "w");
        open_file(fp);
        fprintf(fp, "123456");
        fclose(fp);
    } else {
        // 如果文件存在，则读取内容
        char *password = malloc(1), c;
        int i = 0;
        while ((c = fgetc(fp)) != EOF) {
            password[i++] = c;
            password = realloc(password, i + 1);
        }
        password[i] = '\0';
        fclose(fp);
        // 判断内容是否为 6 位数字的字符串
        if (strlen(password) != 6 || !isdigit(password[0]) ||
            !isdigit(password[1]) || !isdigit(password[2]) ||
            !isdigit(password[3]) || !isdigit(password[4]) || !isdigit(password[5])) {
            // 如果不是，则更改内容为字符串 123456
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