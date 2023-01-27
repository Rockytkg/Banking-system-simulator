#include "functions.h"

//名为 head 的全局变量，该变量将指向链表的第一个账户
extern BankAccount *head;
extern BankAccount *currents;
BankAccount *accountes;
char accounts[11];

// 清空缓存区
void clear_kb_buffer() {
    while (kbhit()) {
        getch();
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
        saveBankAccounts();
        // 否则，使用随机数生成一个指定范围内的休眠时间
        srand(time(NULL));
        sleep_time = rand() % (25 - 10 + 1) + 10;
        for (actual_time = 30; actual_time > sleep_time; actual_time--) {
            printf("正在操作，请稍等 %d \r", actual_time);
            fflush(stdout);
            //Sleep(1000);
        }
    }
    system("cls");
    clear_kb_buffer();
}

// 隐藏输入密码，用*代替
void hide_password(char *key) {
    int index = 0;
    char buff;
    clear_kb_buffer();
    // 当输入的字符不是回车，或者输入的字符数还没有达到最大长度时进入循环
    while ((buff = getch()) != '\r' || index < 6) {
        // 检查字符是否为数字，否则如果输入的字符是退格，并且密码中有字符，则删除最后一个字符
        if (isdigit(buff)) {
            // 如果输入的字符数还没有达到最大长度，将字符加入密码中
            if (index < 6) {
                key[index++] = buff;
                putchar('*'); // 将输入的字符替换成 *
            }
        } else if (buff == '\b' && index != 0) {
            index--;
            printf("\b \b"); // 将光标向前移动一位，并将字符清除
        }
    }
    key[index] = '\0'; // 在密码的末尾加上终止符
    printf("\n");
    clear_kb_buffer();
}

// 输入账户
void input_account(char *id, int i) {
    int index = 0;
    char ch;
    BankAccount *current = NULL;
    printf("请输入账号:");
    do {
        while ((ch = getch()) != '\r' || index < 10) {
            // 检查字符是否为数字，否则如果输入的字符是退格，并且id中有字符，则删除最后一个字符
            if (isdigit(ch)) {
                // 如果输入的字符数还没有达到最大长度，将字符加入id中
                if (index < 10) {
                    id[index++] = ch;
                    putchar(ch);
                }
            } else if (ch == '\b' && index != 0) {
                index--;
                printf("\b \b"); // 将光标向前移动一位，并将字符清除
            }
        }
        id[index] = '\0'; // 在末尾加上终止符
        printf("\n");
        current = findAccount(id);
        if (current == NULL) {
            printf("您输入的账号不存在,请重新输入:");
            index = 0;
        }
        if (current != NULL && i == 1)
            currents = findAccount(id);
        else if (current != NULL && i == 2)
            accountes = findAccount(id);
    } while (current == NULL);
    clear_kb_buffer();
}

//密码验证，正确返回1，错误返回0
int verify_password() {
    char key[7], password[7], id[11];
    input_account(id, 1);
    int sum = 3;
    strcpy(key, currents->password);
    printf("请输入密码:");
    while (1) {
        hide_password(password);
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

//查询余额
void queryBalance() {
    printf("当前账户余额为: %lld.%02d 元\n", currents->balance.dollars, currents->balance.cents);
}

//修改密码
void change_password() {
    int choose = 2;
    while (choose == 2) {
        // 打开文件
        char new_password1[7], new_password2[7];
        printf("请输入新密码:");
        while (1) {
            hide_password(new_password1);
            printf("请再次输入新密码:");
            hide_password(new_password2);
            if (strcmp(new_password1, new_password2) == 0) {
                strcpy(currents->password, new_password2);
                break;
            } else {
                printf("两次输入不一致，请重新输入:");
            }
        }
        choose = menu3();
    }
}

//取款
void withdraw() {
    int choose;
    do {
        printf("请输入取款金额:");
        input_number(2);
        choose = menu3();
    } while (choose == 2);
}

// 存款函数
void deposit() {
    int choose;
    do {
        printf("请输入存款金额:");
        input_number(1);
        choose = menu3();
    } while (choose == 2);
}

//创建新的账户
BankAccount *createAccount(char *accountNumber, char *accountHolderName, char *bankName, char *password) {
    BankAccount *account = malloc(sizeof(BankAccount));
    strcpy(account->accountNumber, accountNumber);
    account->accountHolderName = malloc(strlen(accountHolderName) + 1);
    strcpy(account->accountHolderName, accountHolderName);
    account->bankName = malloc(strlen(bankName) + 1);
    strcpy(account->bankName, bankName);
    strcpy(account->password, password);
    account->balance.dollars = 0; // 修改赋值语句
    account->balance.cents = 0; // 修改赋值语句
    account->transactions = NULL;
    account->next = head;
    head = account; // 更新 head 的值
    return account;
}

//向给定的账户添加新的交易信息
void addTransaction(char *message, BankAccount *current) {
    // 获取当前时间
    time_t t = time(NULL);
    struct tm *timeInfo = localtime(&t);
    // 将时间转换为 24 小时制字符串
    char timeStr[50];
    strftime(timeStr, sizeof(timeStr), "[%Y-%m-%d %H:%M:%S]", timeInfo);
    snprintf(timeStr, sizeof(timeStr), "%s %s", timeStr, message);
    Transaction *transaction = malloc(sizeof(Transaction));
    strncpy(transaction->message, timeStr, sizeof(transaction->message));
    transaction->next = current->transactions;
    current->transactions = transaction;
}

//打印给定账户的所有交易信息
void printTransactions() {
    int choose = 2;
    Transaction *current = currents->transactions;
    if (current == NULL) {
        printf("没有交易记录！！！\n");
        choose = menu3();
    }
    while (choose == 2) {
        if (current != NULL) {
            printf("%s\n", current->message);
            current = current->next;
            continue;
        }
        choose = menu3();
    }
}

//释放给定的账户的所有内存
void freeAccount(BankAccount *account) {
    free(account->accountHolderName);
    free(account->bankName);
    Transaction *current = account->transactions;
    while (current != NULL) {
        Transaction *next = current->next;
        free(current);
        current = next;
    }
    free(account);
}

//查找账户
BankAccount *findAccount(char *accountNumber) {
    BankAccount *current = head;
    while (current != NULL) {
        if (strcmp(current->accountNumber, accountNumber) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

//删除账户
void deleteAccount() {
    if (currents == head) {
        head = currents->next;
    } else {
        BankAccount *current = head;
        while (current != NULL && current->next != currents) {
            current = current->next;
        }
        if (current != NULL) {
            current->next = currents->next;
        }
    }
    freeAccount(currents);
}

//余额处理函数，避免精度丢失的问题
int processBankAccount(int operation, unsigned long long integer, int fraction) {
    unsigned long long dollars, dollar;
    int cents, cent;
    dollars = currents->balance.dollars;
    cents = currents->balance.cents;
    countdown(0); // 随机模拟等待时间
    if (operation == 1) {
        // 存款
        dollars += integer;
        cents += fraction;
        if (cents >= 100) {
            cents -= 100;
            dollars += 1;
        }
        printf("存款 %lld.%02d 元成功\n", integer, fraction);
        printf("您当前余额为 %lld.%02d 元\n", dollars, cents);
    } else if (operation == 2) {
        // 取款
        if (dollars < integer) {
            printf("\n余额不足，无法取款。\n");
            return 0;
        }
        dollars -= integer;
        cents -= fraction;
        if (cents < 0) {
            cents += 100;
            dollars -= 1;
        }
        printf("取款 %lld.%02d 元成功\n", integer, fraction);
        printf("您当前余额为 %lld.%02d 元\n", dollars, cents);
    } else if (operation == 3) {
        if (dollars < integer) {
            printf("\n余额不足，无法转账。\n");
            return 0;
        }
        dollars -= integer;
        cents -= fraction;
        if (cents < 0) {
            cents += 100;
            dollars -= 1;
        }
        printf("转账给 %s %lld.%02d 元成功\n", accounts, integer, fraction);
        printf("您当前余额为 %lld.%02d 元\n", dollars, cents);
        // 为收款账户增加余额，并写入收款账户的操作记录
        dollar = accountes->balance.dollars;
        cent = accountes->balance.cents;
        dollar += integer;
        cent += fraction;
        if (cent >= 100) {
            cent -= 100;
            dollar += 1;
        }
        accountes->balance.dollars = dollar;
        accountes->balance.cents = cent;
        currents->balance.dollars = dollars;
        currents->balance.cents = cents;
        return 2;
    } else {
        printf("无效的操作。\n");
    }
    currents->balance.dollars = dollars;
    currents->balance.cents = cents;
    return 1;
}

//格式化操作数
void remove_trailing_zeros(char *amount, int i) {
    char buffer[100];  // 用于存储字符串的缓冲区
    if (i == 1)
        snprintf(buffer, sizeof(buffer), "%s %s %s", "存款", amount, "元");
    else if (i == 2)
        snprintf(buffer, sizeof(buffer), "%s %s %s", "取款", amount, "元");
    else if (i == 3)
        snprintf(buffer, sizeof(buffer), "%s %s %s %s", "转账给", accounts, amount, "元");
    else if (i == 4) {
        snprintf(buffer, sizeof(buffer), "%s %s %s", "收到转账", amount, "元");
        addTransaction(buffer, accountes);
        return;
    }
    addTransaction(buffer, currents);
}

//按格式输入操作数
void input_number(int i) {
    char amount[30]; // 存储输入的金额字符串
    char c; // 存储输入的字符
    int index = 0; // 当前已输入字符的位置
    int hasDecimal = 0; // 标记是否已输入小数点
    int decimals = 0; // 小数部分的位数
    clear_kb_buffer();
    // 读取字符并将其加入字符串
    while ((c = getch()) != '\r' || index == 0) {
        if (isdigit(c) && hasDecimal != 1) {
            // 如果是数字，且整数部分还未超过 20 位，则将其加入字符串
            if (index < 14) {
                amount[index++] = c;
                putchar(c);
            }
        } else if (c == '.' && !hasDecimal && index > 0) {
            // 如果是小数点，且还未输入小数点，且整数部分不为空，则将其加入字符串
            amount[index++] = c;
            putchar(c);
            hasDecimal = 1;
        } else if (c != '\b' && hasDecimal == 1 && decimals < 2) {
            decimals++;
            amount[index++] = c;
            putchar(c);
        } else if (c == '\b' && index > 0) {
            // 如果是退格键，且字符串不为空，则删除最后一个字符
            printf("\b \b");
            if (amount[index - 1] == '.') {
                hasDecimal = 0;
            }
            if (decimals > 0)
                decimals--;
            index--;
        }
    }
    // 检查输入是否包含小数点，如果没有，则在字符串末尾添加 .00
    // 如果有，请检查小数点后有多少个字符。如果只有一个，就在末尾添加 0
    if (!hasDecimal) {
        amount[index++] = '.';
        amount[index++] = '0';
        amount[index++] = '0';
    } else if (decimals == 1) {
        amount[index++] = '0';
    }
    amount[index] = '\0';
    printf("\n");
    parse_string(amount, i);
}

//处理输入的操作数方便余额计算
void parse_string(char *str, int i) {
    char *endptr;
    int fraction, m;
    unsigned long long integer;
    integer = strtol(str, &endptr, 10);
    if (*endptr == '.') {
        fraction = strtol(endptr + 1, NULL, 10);
    } else {
        fraction = 0;
    }
    m = processBankAccount(i, integer, fraction);
    if (m == 1)
        remove_trailing_zeros(str, i);
    else if (m == 2) {
        remove_trailing_zeros(str, 4);
        remove_trailing_zeros(str, 3);
    }
}

//转账
void makeTransfer() {
    int choose;
    do {
        input_account(accounts, 2);
        printf("请输入转账金额:");
        input_number(3);
        choose = menu3();
    } while (choose == 2);
}