#include "main.h"

void menu1() {
    system("cls");//����
    time_t t;
    struct tm *p;
    time(&t);
    p = gmtime(&t);
    printf("\t\t\t\t\033[1m**********************************************\n");
    printf("\t\t\t\t**--------------\033[31m%d��%02d��%02d��\033[0m\033[1m--------------**\n", 1900 + p->tm_year,
           1 + p->tm_mon, p->tm_mday);
    printf("\t\t\t\t****************��ǰʱ��%02dʱ%02d��**************\n", 8 + p->tm_hour, p->tm_min);
    printf("\t\t\t\t**********************************************\n");
    printf("\t\t\t\t**                                          **\n");
    printf("\t\t\t\t**\033[32m               ATMģ��ϵͳ                \033[0m\033[1m**\n");
    printf("\t\t\t\t**                                          **\n");
    printf("\t\t\t\t**********************************************\n");
    printf("\t\t\t\t**\033[1;31m     ��лʹ�ñ�ϵͳ��ϣ����ʹ����죡     \033[0m\033[1m**\n");
    printf("\t\t\t\t**********************************************\n");
    printf("\t\t\t\t**    �����ߣ���**    ѧ�ţ�22041132**      **\n");
    printf("\t\t\t\t**           �༶����Ӧ����2��              **\n");
    printf("\t\t\t\t**           ѧԺ��xxxxxxѧԺ               **\n");
    printf("\t\t\t\t**********************************************\n");
    countdown(60);
}

void menu2() {
    int choose;
    while (1) {
        menu4();
        choose:
        scanf("%d", &choose);
        countdown(0);
        switch (choose) {
            case 1:
                deposit();
                break;
            case 2:
                withdraw();
                break;
            case 3:
                printf("�����С�������������");
                break;
            case 4:
                change_password();
                break;
            case 5:
                print_recording();
                break;
            case 6:
                return;
            default:
                printf("�������벻�Ϸ�������������:");
                goto choose;
        }
    }
}

int menu3() {
    int choose;
    while (1) {
        printf("\t\t\t\t���������������������������������������������������������������������������������������������\n\n");
        printf("\t\t\t\t             | 1. �����ϼ� |\n\n");
        printf("\t\t\t\t             | 2. �������� |\n\n");
        printf("\t\t\t\t���������������������������������������������������������������������������������������������\n");
        printf("��ѡ��:");
        scanf("%d", &choose);
        countdown(0);
        if (choose == 1 || choose == 2)
            return choose;
        printf("�������벻�Ϸ�������������\n");
    }
}

void menu4() {
    printf("\t\t\t\t���������������������������������������������������������������������������������������������\n\n");
    printf("\t\t\t\t   | 1. ���       2. ȡ��       3. ת��     |\n\n");
    printf("\t\t\t\t   | 4. �޸�����   5. ��ѯ��¼   6. �˳�ϵͳ |\n\n");
    printf("\t\t\t\t���������������������������������������������������������������������������������������������\n");
    printf("��ѡ��:");
}

void menu5() {
    printf("\033[1;31m����˵����\033[0m\n\n");
    printf("\033[1m��������ڵ�ǰĿ¼�´���\033[32m'balance.txt'��'password.txt'��'recording.txt'\033[0m�����ļ������ڴ洢���ݣ�����Ѿ������������ļ��������ñ��ݣ�\n");
    printf("��Ϊ��������ܻ�\033[1;31m�޸�\033[0m\033[1m�������ļ�������(��ʾ������ʹ�ñ������������ɾ��)\n\n");
    printf("\033[1;31m���棺\n");
    printf("�粻���ܣ��������رմ˳��򣡣���\n\n\033[0m");
    countdown(60);
}

float f(float x, float y, float z) {
    float a = x * x + 9.0f / 4.0f * y * y + z * z - 1;
    return a * a * a - x * x * z * z * z - 9.0f / 80.0f * y * y * z * z * z;
}

float h(float x, float z) {
    for (float y = 1.0f; y >= 0.0f; y -= 0.001f) {
        if (f(x, y, z) <= 0.0f) {
            return y;
        }
    }
    return 0.0f;
}

int love() {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    _TCHAR buffer[25][80] = {_T(' ')};
    _TCHAR ramp[] = _T(".:-=+*#%@");

    for (float t = 0.0f;; t += 0.1f) {
        int sy = 0;
        float s = sinf(t);
        float a = s * s * s * s * 0.2f;
        for (float z = 1.3f; z > -1.2f; z -= 0.1f) {
            _TCHAR *p = &buffer[sy++][0];
            float tz = z * (1.2f - a);
            for (float x = -1.5f; x < 1.5f; x += 0.05f) {
                float tx = x * (1.2f + a);
                float v = f(tx, 0.0f, tz);
                if (v <= 0.0f) {
                    float y0 = h(tx, tz);
                    float ny = 0.01f;
                    float nx = h(tx + ny, tz) - y0;
                    float nz = h(tx, tz + ny) - y0;
                    float nd = 1.0f / sqrtf(nx * nx + ny * ny + nz * nz);
                    float d = (nx + ny - nz) * nd * 0.5f + 0.5f;
                    *p++ = ramp[(int) (d * 5.0f)];
                } else {
                    *p++ = ' ';
                }
            }
        }

        for (sy = 0; sy < 25; sy++) {
            COORD coord = {0, sy};
            SetConsoleCursorPosition(console_handle, coord);
            WriteConsole(console_handle, buffer[sy], 79, NULL, 0);
        }
        Sleep(33);
    }
}
