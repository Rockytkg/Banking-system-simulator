#include <graphics.h>

extern "C" {
#include "functions.h"
}
extern unsigned char key[16];

int main() {
    generateKey();
    menu1();
    menu5();
    if (DocumentVerification() == 1)
        loadBankAccounts();
    switch (verify_password()) {
        case 1:
            menu2();
            break;
        default:
            getchar();
            return 0;
    }
    saveBankAccounts();
    puts("∏––ª π”√!!!");
    countdown(100);
    return 0;
}