#include "signal_handler.h"

void handler_ctrl_z(int sigval) {
    fflush(stdout);
}

void handler_ctrl_c(int sigval) {
    char c;
    printf("\nDo you want to exit the program? (y - yes) (n - no)\n");
    scanf(" %c", &c);
    while (c != 'y' && c != 'n') {
        printf("\nInvalid input. Enter y for YES, n for NO\n");
        scanf(" %c", &c);
    }
    if (c == 'y') {
        kill(0, SIGKILL);
        exit(0);
    }
    else {
        fflush(stdout);
    }
}