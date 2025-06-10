/* Small testing library that can run on the host or on the device.
When running on the device, functions to print characters must be provided.
*/
#include <stdint.h>
#include <stdbool.h>

// Functions to run and process the tests
void JTEST_run_tests(void);
int JTEST_run_on_target(void);
int JTEST_run_on_host(void);
void JTEST_end_of_test(uint16_t count, bool first_call);

// Actual functions doing the tests
void JT_assert(bool);


// Test Suite Macros

// Defines a block containing multiple tests in one file. Exactly one block is
// required.
#define JT_TESTS const char JT_test_file_[] = __FILE__; \
    /* This is increased in the test macros */ \
    /* Reset to 0 before each set of asserts within  a test. */ \
    uint16_t count = 0; \
    static bool first_call_to_end_of_test = true; \
    void JTEST_run_tests(void)

// Defines a group of asserts/other tests and names them. This is just there to
// provide structure, and helpful output when tests fail/pass
#define JT_TEST(name_)\
    /* This is where the results of the previous test get printed. It is done */ \
    /* here so the test code can look a bit nicer bracket-wise, and one macro less */ \
    /* can be used at the end of a test. This is also means JTEST_end_of_test must */ \
    /* be run at the end of ALL tests, to process the last result.*/ \
    JTEST_end_of_test(count, first_call_to_end_of_test); \
    first_call_to_end_of_test = false; \
    count = 0; \
    printf("\t [TEST] --- %s ---", name_);

// This part should only be reached if all tests pass
#define JT_TEST_END printf("-> PASS\n\t\t %u checks completed.\n", count);


#define JT_ASSERT(condition_)\
    if (condition_){ \
         /* Print a stripe after each succesful test and count tests.
        // The growing arrow should look cool.*/ \
        printf("-"); \
        /* count is defined in JT_run_single_test
        as static uint16_t count = 0;  */ \
        count++; \
    } \
    else { \
        printf("-> FAIL\n"); \
        printf("\t\t Line {%u}: %s is FALSE", __LINE__, #condition_); \
        /* Do not finish the test.*/ \
        return; \
    }

// Extern variables declared in the test files.
extern char const JT_test_file_[];


