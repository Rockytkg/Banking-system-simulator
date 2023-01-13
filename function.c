#include "functions.h"

//��Ϊ head ��ȫ�ֱ������ñ�����ָ������ĵ�һ���˻�
extern BankAccount *head;
extern BankAccount *currents;

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
            Sleep(1000);
        }
    }
    system("cls");
}

// �����������룬��*����
void hide_password(char *key) {
    int index = 0;
    char buff;
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
}

// �����˻�
void input_account(char *id) {
    int index = 0;
    char ch;
    printf("�������˺�:");
    while (currents == NULL) {
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
        currents = findAccount(id);
        if (currents == NULL) {
            printf("��������˺Ų�����,����������:");
            index = 0;
        }
    }
}

//������֤����ȷ����1�����󷵻�0
int verify_password() {
    char key[7], password[7], id[11];
    input_account(id);
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
    printf("��ǰ�˻����Ϊ: %lld.%d Ԫ\n", currents->balance.dollars, currents->balance.cents);
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
void addTransaction(char *message) {
    // ��ȡ��ǰʱ��
    time_t t = time(NULL);
    struct tm *timeInfo = localtime(&t);
    // ��ʱ��ת��Ϊ 24 Сʱ���ַ���
    char timeStr[50];
    strftime(timeStr, sizeof(timeStr), "[%Y-%m-%d %H:%M:%S]", timeInfo);
    snprintf(timeStr, sizeof(timeStr), "%s %s", timeStr, message);
    Transaction *transaction = malloc(sizeof(Transaction));
    strncpy(transaction->message, timeStr, sizeof(transaction->message));
    transaction->next = currents->transactions;
    currents->transactions = transaction;
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
    unsigned long long dollars;
    int cents;
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
        printf("��� %lld.%d Ԫ�ɹ�\n", integer, fraction);
        printf("����ǰ���Ϊ %lld.%d Ԫ\n", dollars, cents);
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
        printf("ȡ�� %lld.%d Ԫ�ɹ�\n", integer, fraction);
        printf("����ǰ���Ϊ %lld.%d Ԫ\n", dollars, cents);
    } else {
        printf("��Ч�Ĳ�����\n");
    }
    currents->balance.dollars = dollars;
    currents->balance.cents = cents;
    return 1;
}

//��ʽ��������������ȥ���������0
void remove_trailing_zeros(char *amount, int i) {
    char buffer[100];  // ���ڴ洢�ַ����Ļ�����

    if (i == 1)
        snprintf(buffer, sizeof(buffer), "%s %s %s", "���", amount, "Ԫ");
    else if (i == 2)
        snprintf(buffer, sizeof(buffer), "%s %s %s", "ȡ��", amount, "Ԫ");
    addTransaction(buffer);
}

//����ʽ���������
void input_number(int i) {
    char amount[16];  // �洢����Ľ���ַ���
    char c;  // �洢������ַ�
    int index = 0;  // ��ǰ�������ַ���λ��
    int hasDecimal = 0;  // ����Ƿ�������С����
    int decimals = 0;  // С�����ֵ�λ��
    // ��ȡ�ַ�����������ַ���
    while ((c = getch()) != '\r' && decimals <= 2) {
        if (isdigit(c) && hasDecimal != 1) {
            // ��������֣����������ֻ�δ���� 10 λ����������ַ���
            if (index < 10) {
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
    amount[index] = '\0';
    printf("\n");
    parse_string(amount, i);
}

//��������Ĳ���������������
void parse_string(char *str, int i) {
    char *endptr;
    int fraction;
    unsigned long long integer;
    integer = strtol(str, &endptr, 10);
    if (*endptr == '.') {
        fraction = strtol(endptr + 1, NULL, 10);
    } else {
        fraction = 0;
    }
    if (processBankAccount(i, integer, fraction) == 1)
        remove_trailing_zeros(str, i);
}
