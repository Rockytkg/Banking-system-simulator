#include <graphics.h>

extern "C" {
#include "functions.h"
}

int main() {
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
    puts("感谢使用!!!");
    countdown(100);
    return 0;
}