#include "functions.h"

int input_select() {
    saveBankAccounts();
    char ch, choose;
    int index = 0;
    while ((ch = getch()) != '\r' || index == 0) {
        if (isdigit(ch)) {
            if (index < 1) {
                putchar(ch);
                choose = ch;
                index++;
            }
        } else if (ch == '\b' && index != 0) {
            index--;
            printf("\b \b"); // 将光标向前移动一位，并将字符清除
        }
    }
    index = atoi(&choose);
    printf("\n");
    return index;
}

void menu1() {
    system("cls");//清屏
    time_t t;
    struct tm *p;
    time(&t);
    p = gmtime(&t);
    printf("\t\t\t\t\033[1m**********************************************\n");
    printf("\t\t\t\t**--------------\033[31m%d年%02d月%02d日\033[0m\033[1m--------------**\n", 1900 + p->tm_year,
           1 + p->tm_mon, p->tm_mday);
    printf("\t\t\t\t****************当前时间%02d时%02d分**************\n", 8 + p->tm_hour, p->tm_min);
    printf("\t\t\t\t**********************************************\n");
    printf("\t\t\t\t**                                          **\n");
    printf("\t\t\t\t**\033[32m               ATM模拟系统                \033[0m\033[1m**\n");
    printf("\t\t\t\t**                                          **\n");
    printf("\t\t\t\t**********************************************\n");
    printf("\t\t\t\t**\033[1;31m     感谢使用本系统，希望您使用愉快！     \033[0m\033[1m**\n");
    printf("\t\t\t\t**********************************************\n");
    printf("\t\t\t\t**    制作者：刘**    学号：22041132**      **\n");
    printf("\t\t\t\t**           班级：计应程序2班              **\n");
    printf("\t\t\t\t**           学院：xxxxxx学院               **\n");
    printf("\t\t\t\t**********************************************\n");
    countdown(60);
}

void menu2() {
    int ch;
    while (1) {
        menu4();
        ch = input_select();
        countdown(0);
        switch (ch) {
            case 1:
                deposit();
                break;
            case 2:
                withdraw();
                break;
            case 3:
                makeTransfer();
                break;
            case 4:
                change_password();
                break;
            case 5:
                queryBalance();
                break;
            case 6:
                deleteAccount();
                printTransactions();
                break;
            case 7:
                printTransactions();
                break;
            case 8:
                return;
            default:
                printf("您的输入不合法，请重新输入:");
        }
    }
}

int menu3() {
    int choose;
    printf("\t\t\t\t★——————————————————————————————————————————————★\n\n");
    printf("\t\t\t\t             | 1. 返回上级 |\n\n");
    printf("\t\t\t\t             | 2. 继续操作 |\n\n");
    printf("\t\t\t\t★——————————————————————————————————————————————★\n");
    printf("请选择:");
    while (1) {
        choose = input_select();
        if (choose == 1 || choose == 2) {
            countdown(0);
            return choose;
        }
        printf("您的输入不合法，请重新输入:");
    }
}

void menu4() {
    printf("\t\t\t\t★——————————————————————————————————————————————★\n\n");
    printf("\t\t\t\t   | 1. 存款       2. 取款       3. 转账     |\n\n");
    printf("\t\t\t\t   | 4. 修改密码   5. 查询余额   6. 删除账户 |\n\n");
    printf("\t\t\t\t   | 7. 查询交易记录             8. 退出系统 |\n\n");
    printf("\t\t\t\t★——————————————————————————————————————————————★\n");
    printf("请选择:");
}

void menu5() {
    printf("\033[1;31m程序说明：\033[0m\n\n");
    printf("\033[1m本程序会在当前目录下创建\033[32m'AESBankAccounts.txt'\033[0m文件，用于存储数据，如果已经存在这个文件，请做好备份，\n");
    printf("\033[1m本程序会在当前目录下创建\033[32m'key.bin'\033[0m文件，用于存储密钥，如果已经存在这个文件，请做好备份，\n");
    printf("因为本程序可能会\033[1;31m修改\033[0m\033[1m这个文件的内容(提示：不在使用本程序后请自行删除)\n\n");
    printf("\033[1;31m警告：\n");
    printf("如不接受，请立即关闭此程序！！！\n\n\033[0m");
    countdown(60);
}
