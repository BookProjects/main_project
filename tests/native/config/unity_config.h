#ifndef NATIVE_UNITY_CONFIG_H
#define NATIVE_UNITY_CONFIG_H

/*
#define UNITY_INT_WIDTH 32
#define UNITY_POINTER_WIDTH UNITY_INT_WIDTH
*/

#include "unity.h"
#include <stdio.h>

int native_unity_output_char(int a);
void native_unity_output_flush();
void native_unity_output_start();
void native_unity_output_complete();

#define UNITY_OUTPUT_CHAR(a) native_unity_output_char(a)
#define UNITY_OUTPUT_FLUSH() native_unity_output_flush()
#define UNITY_OUTPUT_START() native_unity_output_start()
#define UNITY_OUTPUT_COMPLETE() native_unity_output_complete()

#endif  // NATIVE_UNITY_CONFIG_H
