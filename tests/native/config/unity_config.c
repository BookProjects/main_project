#include "config/unity_config.h"

static char answer[4] = "NO\n";

int native_unity_output_char(int a) {
    putchar(a);
    answer[0] = answer[1];
    answer[1] = answer[2];
    answer[2] = a;
}

void native_unity_output_flush() {
    fflush(stdout);
    printf("FLUSH\n");
}

void native_unity_output_start() {
    printf("START\n");
}

void native_unity_output_complete() {
    if(strcmp(answer, "OK\n") == 0) {
        printf("SUCCESS\n");
    } else {
        printf("FAILURE\n");
    }
}
