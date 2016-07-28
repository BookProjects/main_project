#include "stm32f0/unity_config.h"

static char answer[4] = "NO\n";

int cross_unity_output_char(int a) {
    answer[0] = answer[1];
    answer[1] = answer[2];
    answer[2] = a;
    return 0;
}

void cross_unity_output_complete() {
    if(strcmp(answer, "OK\n") == 0) {
        answer[0] = 'A';
    } else {
        answer[0] = 'F';
    }
}
