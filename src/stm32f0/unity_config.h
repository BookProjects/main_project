#ifndef CROSS_UNITY_CONFIG_H
#define CROSS_UNITY_CONFIG_H

/*
#define UNITY_INT_WIDTH 32
#define UNITY_POINTER_WIDTH UNITY_INT_WIDTH
*/

#include "unity.h"

int cross_unity_output_char(int a);
void cross_unity_output_complete();
void setup_unity_output();

#define UNITY_OUTPUT_CHAR(a) cross_unity_output_char(a)
#define UNITY_OUTPUT_FLUSH()
#define UNITY_OUTPUT_START()
#define UNITY_OUTPUT_COMPLETE() cross_unity_output_complete()

#endif  // CROSS_UNITY_CONFIG_H
