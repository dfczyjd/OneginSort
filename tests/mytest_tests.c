#include "mytest.h"
#include "../src/OneginSort.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

void testMytest()
{
    ASSERT_EQUAL(1, 1);
}

int main()
{
    initializeTests(1);
    registerTest("test", testMytest);
    runTests();
    clearTests();
    return 0;
}