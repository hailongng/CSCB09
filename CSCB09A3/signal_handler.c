#include "signal_handler.h"

void handler_ctrl_z(int sigval) {
    fflush(stdout);
}

void handler_ctrl_c(int sigval) {
    char c;
    printf("Do you want to exit the program?\n");
    scanf("%c", &c);
    if (c == 'y' || c == 'Y') {
        exit(0);
    }
    else {
        fflush(stdout);
    }
}