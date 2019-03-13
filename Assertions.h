/*
 * Assertions.h
 *
 *  Created on: 11.09.2018
 *      Author: t.loch
 */
 
#ifndef ASSERTIONS_IMPLEMENTED
#define ASSERTIONS_IMPLEMENTED

/*
 *	If the Macro USE_DEFAULT_MAIN_FUNCTION is defined, then the user needs to include its test file into this file.
 *	
 *	The included file needs to define the macro TEST_FUNCTION_POINTER, that contains an array of function pointer to all tested functions.
 *	example: 
 *	#define TEST_FUNCTION_POINTER {function1, function2}
 *	for the test functions function1 and function2
 *
*/
// include your test header file here if you want to use the main functions of the assertions library
#define _GNU_SOURCE
#include "Test/test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define TIMEVAL_TO_MICSEC(time) ((time.tv_sec * (long)1000000) + (time.tv_usec))

#ifndef CU_ASSERT
#define CU_ASSERT(a) assert(a, __FUNCTION__, __LINE__)
#endif

#ifndef CU_ASSERT_EQUAL
#define CU_ASSERT_EQUAL(a,b) assert_equal(a, b, __FUNCTION__, __LINE__)
#endif

#ifndef CU_FAIL
#define CU_FAIL(a) assert_fail(a, __FUNCTION__, __LINE__);
#endif

#define TIMEOUT_LINE_NUMBER -1
#define DEFAULT_F_NAME "-1"

// getter and setter
void setTimeOut(time_t timeoutSec, long timeoutNanoSec);
int getRunned_TestsCount();
int getFailedTestsCount();
int getRunnedAssertionsCount();
double getSpentTime();


void assert(int expression, const char funcName[], const int line);
void assert_equal(int actual, int expected, const char funcName[], const int line);
void assert_fail(char* message, const char funcName[], const int line);

void runTest(void (*func)());
void runTests(void (*functions[])(), int numberOfFunctions);

void print_Summary(void);
void execute();

#endif