#include "main.h"

//校验函数
void open_file(FILE *fp) {
    if (fp == NULL) {
        system("cls");
        printf("\033[1;31m警告：\033[0m\n\n");
        printf("\033[1m程序异常，打开文件时出错！！！");
        getchar();
        exit(0);
    }
}

//暂停时间
void countdown(int pause_time) {
    int sleep_time, actual_time;
    if (pause_time > 0) {
        // 如果提供了参数，使用参数指定的休眠时间
        sleep_time = pause_time;
        printf("按下 \033[1;32mEnter\033[0m 键或等待 \033[1;32m倒计时\033[0m \033[1;31m结束\033[0m 可以继续操作\n");
        while (sleep_time > 0) {
            sleep_time--;
            printf("\r倒计时 %d 秒 ", sleep_time);
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
        // 否则，使用随机数生成一个指定范围内的休眠时间
        srand(time(NULL));
        sleep_time = rand() % (25 - 15 + 1) + 10;
        for (actual_time = 30; actual_time > sleep_time; actual_time--) {
            printf("正在操作，请稍等 %d \r", actual_time);
            fflush(stdout);
            Sleep(1000);
        }
    }
    system("cls");
}

// 隐藏输入密码，用*代替
void hide_password(char *pswd, unsigned maxlen) {
    int index = 0;
    char buff;
    // 当输入的字符不是回车，或者输入的字符数还没有达到最大长度时进入循环
    while ((buff = getch()) != '\r' || index < maxlen - 1) {
        // 检查字符是否为数字，否则如果输入的字符是退格，并且密码中有字符，则删除最后一个字符
        if (isdigit(buff)) {
            // 如果输入的字符数还没有达到最大长度，将字符加入密码中
            if (index < maxlen - 1) {
                pswd[index++] = buff;
                putchar('*'); // 将输入的字符替换成 *
            }
        } else if (buff == '\b' && index != 0) {
            index--;
            printf("\b \b"); // 将光标向前移动一位，并将字符清除
        }
    }
    pswd[index] = '\0'; // 在密码的末尾加上终止符
    printf("\n");
}

//密码验证，正确返回1，错误返回0
int verify_password() {
    char key[7], password[7];
    int sum = 3;
    // 打开文件
    FILE *fp = fopen("password.txt", "r");
    open_file(fp);
    // 读取密码并写入数组
    fscanf(fp, "%s", key);
    fclose(fp);
    printf("请输入密码(初始密码为123456):");
    while (1) {
        hide_password(password, 7);
        countdown(0);
        sum--;
        if (sum > 0) {
            if (strcmp(key, password) == 0) {
                break;
            } else {
                system("cls");
                printf("密码错误，你还有 %d 次机会\n", sum);
                printf("请重新输入密码:");
            }
        } else {
            printf("密码错误3次，请联系银行工作人员取卡");
            return 0;
        }
    }
    system("cls");
    return 1;
}

//记录模块
void recording(char *str) {
    FILE *fp = fopen("recording.txt", "a");
    open_file(fp);
    // 获取当前时间
    time_t t = time(NULL);
    struct tm *timeinfo = localtime(&t);
    // 将时间转换为 24 小时制字符串
    char timestr[20];
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", timeinfo);
    // 写入时间和字符串
    fprintf(fp, "[%s] %s\n", timestr, str);
    // 关闭文件
    fclose(fp);
}

//查询记录
void print_recording() {
    int choose = 2, balance;
    while (choose == 2) {
        if (access("recording.txt", F_OK) == -1) {
            // 文件不存在
            FILE *fa = fopen("balance.txt", "r");
            open_file(fa);
            fscanf(fa, "%d", &balance);
            printf("您当前余额为 %d 元\n", balance);
            fclose(fa);
            printf("没有操作记录！！！\n");
            goto anew;
        }
        // 打开文件
        FILE *fp = fopen("recording.txt", "r");
        FILE *fa = fopen("balance.txt", "r");
        open_file(fp);
        open_file(fa);
        fscanf(fa, "%d", &balance);
        printf("您当前余额为 %d 元\n", balance);
        fclose(fa);
        // 判断文件是否为空
        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        rewind(fp);
        if (file_size == 0) {
            printf("没有操作记录！！！\n");
            goto anew;
        } else {
            // 逐行读取文件内容
            char line[1024];
            while (fgets(line, sizeof(line), fp) != NULL) {
                // 输出行内容
                printf("%s", line);
            }
        }
        // 关闭文件
        fclose(fp);
        anew:
        choose = menu3();
    }
}

//修改密码
void change_password() {
    int choose = 2;
    while (choose == 2) {
        // 打开文件
        FILE *fp = fopen("password.txt", "r+");
        open_file(fp);
        char new_password1[7], new_password2[7];
        printf("请输入新密码:");
        while (1) {
            hide_password(new_password1, 7);
            printf("请再次输入新密码:");
            hide_password(new_password2, 7);
            if (strcmp(new_password1, new_password2) == 0) {
                break;
            } else {
                printf("两次输入不一致，请重新输入:");
            }
        }
        // 将文件指针移动到文件开头
        rewind(fp);
        // 写入新密码
        fprintf(fp, "%s", new_password1);
        // 关闭文件
        fclose(fp);
        choose = menu3();
    }
}

//取款
void withdraw() {
    // 从文件中读取余额
    int balance, amount, choose;
    FILE *fp = fopen("balance.txt", "r");
    open_file(fp);
    fscanf(fp, "%d", &balance);
    fclose(fp);
    do {
        printf("请输入取款金额:");
        scanf("%d", &amount);
        while (amount <= 0 || amount % 10 != 0) {
            printf("您的输入不合法,请重新输入:");
            scanf("%d", &amount);
        }
        countdown(0);
        // 判断余额是否足够
        if (balance < amount) {
            printf("余额不足!!!\n");
            goto anew;
        }
        // 执行取款操作
        balance -= amount;
        printf("成功取款 %d 元\n", amount);
        // 写入操作记录
        char str[50];
        sprintf(str, "取款 %d 元", amount);
        recording(str);
        // 将新的余额写入文件
        fp = fopen("balance.txt", "w");
        open_file(fp);
        fprintf(fp, "%d", balance);
        fclose(fp);
        anew:
        choose = menu3();
    } while (choose == 2);
}

// 存款函数
void deposit() {
    // 打开 balance.txt 文件
    FILE *fp = fopen("balance.txt", "r");
    open_file(fp);
    // 从 balance.txt 文件中读取之前的余额
    int balance, amount, choose;
    fscanf(fp, "%d", &balance);
    // 关闭文件
    fclose(fp);
    do {
        printf("请输入存款金额:");
        scanf("%d", &amount);
        countdown(0);
        // 校验输入的余额是否合法
        while (amount < 0 || amount % 10 != 0) {
            // 余额不合法，输出错误信息并要求用户重新输入
            printf("错误：金额无效\n请重新输入存款金额:");
            scanf("%d", &amount);
            countdown(0);
        }
        // 计算新的余额
        balance += amount;
        // 写入操作记录
        char str[50];
        sprintf(str, "存款 %d 元", amount);
        recording(str);
        // 打开 balance.txt 文件（用于写入
        fp = fopen("balance.txt", "w");
        open_file(fp);
        // 将新的余额写入 balance.txt 文件
        fprintf(fp, "%d", balance);
        // 关闭文件
        choose = menu3();
        fclose(fp);
    } while (choose == 2);
}