#pragma once
#include <errno.h>
#include <stdbool.h>

#define PRECISION 1e-7

// Wrapper macros for _assert
#define ASSERT_TRUE(x) _assert(x, __FILE__, __LINE__)                     /**< Check, if boolean value is <CODE>true</CODE> */
#define ASSERT_FALSE(x) _assert(!x, __FILE__, __LINE__)                   /**< Check, if boolean value is <CODE>false</CODE> */
#define ASSERT_EQUAL(x,y) ASSERT_TRUE((x == y))                           /**< Check, if two values are equal */
#define ASSERT_FLOAT_EQUAL(x,y) ASSERT_TRUE((fabs(x - y) <= PRECISION))   /**< Check, if two float-point values are equal */
#define ASSERT_ERROR(x) ASSERT_EQUAL(x, errno)                            /**< Check, if given error happened. Caller must set errno to 0 manually. */

typedef void(*TEST_FUNCTION)(void); /**< Simplified name for test function */

bool lastTestResult;

/**
    \brief Initialize tests
*/
void initializeTests(int maxTestCount /**< Maximum allowed number of tests */);

/**
    \brief Register a test

    Registers function <CODE>testFunction</CODE> as test named <CODE>name</CODE>.
    The function shall return <CODE>true</CODE> if test was passed and <CODE>false</CODE>
    otherwise.
    \param name Test's name
    \param testFunction Test's function
*/
void registerTest(const char* name, TEST_FUNCTION testFunction);

/**
    \brief Run tests

    Runs all registered tests
*/
void runTests();

/**
    \brief Clear tests

    Clear tests in order to free memory
*/
void clearTests();

/**
    \brief Checks, if given <CODE>value</CODE> is <CODE>true</CODE>

    Check, if given <CODE>value</CODE> is <CODE>true</CODE> and prints a warning,
    if not. Returns the given value back.
    Note: Do not call this function directly. Use <CODE>ASSERT</CODE> macros instead.
    \param value Value to check
    \param file Name of file with assertion
    \param line Number of line with assertion
    \returns Value of <CODE>value</CODE> parameter
*/
void _assert(bool value, const char* file, int line);