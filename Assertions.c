/*
 * Assertions.c
 *
 *  Created on: 11.09.2018
 *      Author: t.loch
 */
#include "Assertions.h"

char f_name[64] = DEFAULT_F_NAME;
char* (failed_f_names[32]);
int failed_lines[32];

pthread_t active_thread;

int t_count = 0;
int runned_tests = 0; // number of runned tests
int failed_count = 0; // number of failed tests
int runned_count = 0; // number of runned Assertions
double runned_tests_sum = 0; // time spent

time_t timeoutSeconds = 3;
long timeoutNanoSeconds = 0;

void setTimeOut(time_t timeoutSec, long timeoutNanoSec) {
	timeoutSeconds = timeoutSec;
	timeoutNanoSeconds = timeoutNanoSec;
}

int getRunned_TestsCount() {
	return runned_tests;
}

int getFailedTestsCount() {
	return failed_count;
}

int getRunnedAssertionsCount() {
	return runned_count;
}

double getSpentTime() {
	return runned_tests_sum;
}

void testCounter(const char funcName[]) {
	runned_count++;

	if (strcmp(funcName, f_name) == 0) {
		t_count++;
	} else {
		
		strcpy(f_name, funcName);
		t_count = 1;
	}
}

void failed(const char funcName[], const int line) {
	failed_f_names[failed_count] = (char*)funcName;
	failed_lines[failed_count] = line;
	
	failed_count++;
	
	pthread_exit((void*)NULL);
}

void assert(int expression, const char funcName[], const int line) {
	testCounter(funcName);

	if (!expression) {
		printf("%s Failed in line %d!%s\n", KRED, line, KWHT);
		failed(funcName, line);
	}
}

void assert_equal(int actual, int expected, const char funcName[], const int line) {
	testCounter(funcName);

	if (actual != expected) {
		printf("%s Failed in line %d! Value was %d, but expected was %d%s\n", KRED,line, actual, expected,KWHT);
		failed(funcName, line);
	}
}

void assert_fail(char* message, const char funcName[], const int line) {
	testCounter(funcName);
	
	printf("%s Failed in line %d! Message: %s%s\n",KRED, line, message, KWHT);
	
	failed(funcName, line);
}

void runTest(void (*func)()) {
	int f_count = failed_count;
	struct timeval start_time;
	struct timeval end_time;

	// checking if first test
	if (runned_count==0) {
		printf("%s###STARTING TESTS###%s\n\n", KMAG, KWHT);
	}
	
	// setting timeout
	struct timespec timeout;
	
	if (clock_gettime(CLOCK_REALTIME, &timeout) == -1) {
		printf("Time could not be read! Test can not run.\n");
		return;
	}
	
	timeout.tv_sec += timeoutSeconds;
	timeout.tv_nsec += timeoutNanoSeconds;
	
	gettimeofday(&start_time, NULL);
	int success = pthread_create(&active_thread,NULL,(void*)func,(void*)NULL);
	
	if (success != 0) {
		printf("Test thread could not be created! Errorcode %d\n", success);
		return;
	}
	
	int status = pthread_timedjoin_np(active_thread, (void*)NULL, &timeout);
	gettimeofday(&end_time, NULL);
	
	if (status == ETIMEDOUT) {
		printf("The test timed out after %f seconds!\n", ((float)timeoutSeconds + ((float)timeoutNanoSeconds/1000000)));
		
		failed_f_names[failed_count] = (char*)f_name;
		failed_lines[failed_count] = TIMEOUT_LINE_NUMBER;
	
		failed_count++;
	} else if (status != 0) {
		printf("An error occured in joining operation! Errorcode: %d\n", status);
	}
	
	long long spentTime = TIMEVAL_TO_MICSEC(end_time) - TIMEVAL_TO_MICSEC(start_time);
	double spentTime_float = ((double)spentTime)/1000000;
	
	runned_tests_sum += spentTime_float;
	runned_tests++;
	
	// printing test summary
	printf("%sRunned test %d: %s%s\n", KGRN, runned_tests, f_name, KWHT);
	printf("\tRunned Assertions: %d\n", t_count);
	printf("\tTests runned %f seconds.\n", spentTime_float);
	if (f_count != failed_count) printf("\t###Test failed!###\n");
}

void runTests(void (*functions[])(), int numberOfFunctions) {
	for (int i = 0; i < numberOfFunctions; i++) {
		runTest(functions[i]);
	}
}

void print_Summary(void) {
	printf("\n%s###TEST SUMMARY###%s\n", KMAG, KWHT);
	printf("\tRunned Tests: %d\n", runned_tests);
	printf("\tRunned Assertions: %d\n", runned_count);
	printf("\tTime needed for processing: %f seconds\n", runned_tests_sum);
	
	if (failed_count == 0) printf("\n%s###All Tests runned successful!###%s\n", KMAG, KWHT);
	else {
		printf("\t%d Tests failed:\n", failed_count);
		for (int i = 0; i < failed_count; i++) {
			if (failed_lines[i] > 0) printf("\t\t%s failed in line %d\n", failed_f_names[i], failed_lines[i]);
			else if (failed_lines[i] == TIMEOUT_LINE_NUMBER) printf("\t\t%s timed out\n", failed_f_names[i]);
		}
	}
}

void execute() {
	exit(failed_count);
}

#ifdef USE_DEFAULT_MAIN_FUNCTION
int main() {
	void (*functions[])() = TEST_FUNCTION_POINTER;
	int size = sizeof(functions)/sizeof(functions[0]);
	
	runTests(functions, size);
	
	print_Summary();
	execute();
}
#endif