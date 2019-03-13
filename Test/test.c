#include "test.h"

void tester1() {
	//CU_ASSERT_EQUAL(test1(), 2);
	CU_ASSERT_EQUAL(test1(), 1);
}

void tester2() {
	//CU_ASSERT_EQUAL(test2(), 1);
	CU_ASSERT_EQUAL(test2(), 2);
}

void tester3() {
	CU_ASSERT(1);
}

#ifndef USE_DEFAULT_MAIN_FUNCTION
int main() {
	void (*functions[])() = {tester1, tester2};
	
	runTests(functions, 2);
	
	print_Summary();
	execute();
}
#endif