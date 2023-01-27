#include "functions.h"

//��Ϊ head ��ȫ�ֱ������ñ�����ָ������ĵ�һ���˻�
extern BankAccount *head;
extern BankAccount *currents;
BankAccount *accountes;
char accounts[11];

// ��ջ�����
void clear_kb_buffer() {
    while (kbhit()) {
        getch();
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
        saveBankAccounts();
        // ����ʹ�����������һ��ָ����Χ�ڵ�����ʱ��
        srand(time(NULL));
        sleep_time = rand() % (25 - 10 + 1) + 10;
        for (actual_time = 30; actual_time > sleep_time; actual_time--) {
            printf("���ڲ��������Ե� %d \r", actual_time);
            fflush(stdout);
            //Sleep(1000);
        }
    }
    system("cls");
    clear_kb_buffer();
}

// �����������룬��*����
void hide_password(char *key) {
    int index = 0;
    char buff;
    clear_kb_buffer();
    // ��������ַ����ǻس�������������ַ�����û�дﵽ��󳤶�ʱ����ѭ��
    while ((buff = getch()) != '\r' || index < 6) {
        // ����ַ��Ƿ�Ϊ���֣��������������ַ����˸񣬲������������ַ�����ɾ�����һ���ַ�
        if (isdigit(buff)) {
            // ���������ַ�����û�дﵽ��󳤶ȣ����ַ�����������
            if (index < 6) {
                key[index++] = buff;
                putchar('*'); // ��������ַ��滻�� *
            }
        } else if (buff == '\b' && index != 0) {
            index--;
            printf("\b \b"); // �������ǰ�ƶ�һλ�������ַ����
        }
    }
    key[index] = '\0'; // �������ĩβ������ֹ��
    printf("\n");
    clear_kb_buffer();
}

// �����˻�
void input_account(char *id, int i) {
    int index = 0;
    char ch;
    BankAccount *current = NULL;
    printf("�������˺�:");
    do {
        while ((ch = getch()) != '\r' || index < 10) {
            // ����ַ��Ƿ�Ϊ���֣��������������ַ����˸񣬲���id�����ַ�����ɾ�����һ���ַ�
            if (isdigit(ch)) {
                // ���������ַ�����û�дﵽ��󳤶ȣ����ַ�����id��
                if (index < 10) {
                    id[index++] = ch;
                    putchar(ch);
                }
            } else if (ch == '\b' && index != 0) {
                index--;
                printf("\b \b"); // �������ǰ�ƶ�һλ�������ַ����
            }
        }
        id[index] = '\0'; // ��ĩβ������ֹ��
        printf("\n");
        current = findAccount(id);
        if (current == NULL) {
            printf("��������˺Ų�����,����������:");
            index = 0;
        }
        if (current != NULL && i == 1)
            currents = findAccount(id);
        else if (current != NULL && i == 2)
            accountes = findAccount(id);
    } while (current == NULL);
    clear_kb_buffer();
}

//������֤����ȷ����1�����󷵻�0
int verify_password() {
    char key[7], password[7], id[11];
    input_account(id, 1);
    int sum = 3;
    strcpy(key, currents->password);
    printf("����������:");
    while (1) {
        hide_password(password);
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

//��ѯ���
void queryBalance() {
    printf("��ǰ�˻����Ϊ: %lld.%02d Ԫ\n", currents->balance.dollars, currents->balance.cents);
}

//�޸�����
void change_password() {
    int choose = 2;
    while (choose == 2) {
        // ���ļ�
        char new_password1[7], new_password2[7];
        printf("������������:");
        while (1) {
            hide_password(new_password1);
            printf("���ٴ�����������:");
            hide_password(new_password2);
            if (strcmp(new_password1, new_password2) == 0) {
                strcpy(currents->password, new_password2);
                break;
            } else {
                printf("�������벻һ�£�����������:");
            }
        }
        choose = menu3();
    }
}

//ȡ��
void withdraw() {
    int choose;
    do {
        printf("������ȡ����:");
        input_number(2);
        choose = menu3();
    } while (choose == 2);
}

// ����
void deposit() {
    int choose;
    do {
        printf("����������:");
        input_number(1);
        choose = menu3();
    } while (choose == 2);
}

//�����µ��˻�
BankAccount *createAccount(char *accountNumber, char *accountHolderName, char *bankName, char *password) {
    BankAccount *account = malloc(sizeof(BankAccount));
    strcpy(account->accountNumber, accountNumber);
    account->accountHolderName = malloc(strlen(accountHolderName) + 1);
    strcpy(account->accountHolderName, accountHolderName);
    account->bankName = malloc(strlen(bankName) + 1);
    strcpy(account->bankName, bankName);
    strcpy(account->password, password);
    account->balance.dollars = 0; // �޸ĸ�ֵ���
    account->balance.cents = 0; // �޸ĸ�ֵ���
    account->transactions = NULL;
    account->next = head;
    head = account; // ���� head ��ֵ
    return account;
}

//��������˻�����µĽ�����Ϣ
void addTransaction(char *message, BankAccount *current) {
    // ��ȡ��ǰʱ��
    time_t t = time(NULL);
    struct tm *timeInfo = localtime(&t);
    // ��ʱ��ת��Ϊ 24 Сʱ���ַ���
    char timeStr[50];
    strftime(timeStr, sizeof(timeStr), "[%Y-%m-%d %H:%M:%S]", timeInfo);
    snprintf(timeStr, sizeof(timeStr), "%s %s", timeStr, message);
    Transaction *transaction = malloc(sizeof(Transaction));
    strncpy(transaction->message, timeStr, sizeof(transaction->message));
    transaction->next = current->transactions;
    current->transactions = transaction;
}

//��ӡ�����˻������н�����Ϣ
void printTransactions() {
    int choose = 2;
    Transaction *current = currents->transactions;
    if (current == NULL) {
        printf("û�н��׼�¼������\n");
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

//�ͷŸ������˻��������ڴ�
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

//�����˻�
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

//ɾ���˻�
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

//�����������⾫�ȶ�ʧ������
int processBankAccount(int operation, unsigned long long integer, int fraction) {
    unsigned long long dollars, dollar;
    int cents, cent;
    dollars = currents->balance.dollars;
    cents = currents->balance.cents;
    countdown(0); // ���ģ��ȴ�ʱ��
    if (operation == 1) {
        // ���
        dollars += integer;
        cents += fraction;
        if (cents >= 100) {
            cents -= 100;
            dollars += 1;
        }
        printf("��� %lld.%02d Ԫ�ɹ�\n", integer, fraction);
        printf("����ǰ���Ϊ %lld.%02d Ԫ\n", dollars, cents);
    } else if (operation == 2) {
        // ȡ��
        if (dollars < integer) {
            printf("\n���㣬�޷�ȡ�\n");
            return 0;
        }
        dollars -= integer;
        cents -= fraction;
        if (cents < 0) {
            cents += 100;
            dollars -= 1;
        }
        printf("ȡ�� %lld.%02d Ԫ�ɹ�\n", integer, fraction);
        printf("����ǰ���Ϊ %lld.%02d Ԫ\n", dollars, cents);
    } else if (operation == 3) {
        if (dollars < integer) {
            printf("\n���㣬�޷�ת�ˡ�\n");
            return 0;
        }
        dollars -= integer;
        cents -= fraction;
        if (cents < 0) {
            cents += 100;
            dollars -= 1;
        }
        printf("ת�˸� %s %lld.%02d Ԫ�ɹ�\n", accounts, integer, fraction);
        printf("����ǰ���Ϊ %lld.%02d Ԫ\n", dollars, cents);
        // Ϊ�տ��˻���������д���տ��˻��Ĳ�����¼
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
        printf("��Ч�Ĳ�����\n");
    }
    currents->balance.dollars = dollars;
    currents->balance.cents = cents;
    return 1;
}

//��ʽ��������
void remove_trailing_zeros(char *amount, int i) {
    char buffer[100];  // ���ڴ洢�ַ����Ļ�����
    if (i == 1)
        snprintf(buffer, sizeof(buffer), "%s %s %s", "���", amount, "Ԫ");
    else if (i == 2)
        snprintf(buffer, sizeof(buffer), "%s %s %s", "ȡ��", amount, "Ԫ");
    else if (i == 3)
        snprintf(buffer, sizeof(buffer), "%s %s %s %s", "ת�˸�", accounts, amount, "Ԫ");
    else if (i == 4) {
        snprintf(buffer, sizeof(buffer), "%s %s %s", "�յ�ת��", amount, "Ԫ");
        addTransaction(buffer, accountes);
        return;
    }
    addTransaction(buffer, currents);
}

//����ʽ���������
void input_number(int i) {
    char amount[30]; // �洢����Ľ���ַ���
    char c; // �洢������ַ�
    int index = 0; // ��ǰ�������ַ���λ��
    int hasDecimal = 0; // ����Ƿ�������С����
    int decimals = 0; // С�����ֵ�λ��
    clear_kb_buffer();
    // ��ȡ�ַ�����������ַ���
    while ((c = getch()) != '\r' || index == 0) {
        if (isdigit(c) && hasDecimal != 1) {
            // ��������֣����������ֻ�δ���� 20 λ����������ַ���
            if (index < 14) {
                amount[index++] = c;
                putchar(c);
            }
        } else if (c == '.' && !hasDecimal && index > 0) {
            // �����С���㣬�һ�δ����С���㣬���������ֲ�Ϊ�գ���������ַ���
            amount[index++] = c;
            putchar(c);
            hasDecimal = 1;
        } else if (c != '\b' && hasDecimal == 1 && decimals < 2) {
            decimals++;
            amount[index++] = c;
            putchar(c);
        } else if (c == '\b' && index > 0) {
            // ������˸�������ַ�����Ϊ�գ���ɾ�����һ���ַ�
            printf("\b \b");
            if (amount[index - 1] == '.') {
                hasDecimal = 0;
            }
            if (decimals > 0)
                decimals--;
            index--;
        }
    }
    // ��������Ƿ����С���㣬���û�У������ַ���ĩβ��� .00
    // ����У�����С������ж��ٸ��ַ������ֻ��һ��������ĩβ��� 0
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

//��������Ĳ���������������
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

//ת��
void makeTransfer() {
    int choose;
    do {
        input_account(accounts, 2);
        printf("������ת�˽��:");
        input_number(3);
        choose = menu3();
    } while (choose == 2);
}