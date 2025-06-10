#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "jtest.h"

// Make some variables accessible from test-code
// Bit of a nasty solution, but I want the test-code to look nicer bracket-wise,
// and this does the trick for now. :)
extern uint16_t count;


// FUNCTIONS RUNNING THE TESTS
int main(void){
    #ifdef JTEST_TARGET
        return JTEST_run_on_target();
    #else
        #ifdef JTEST_HOST
        return JTEST_run_on_host();
        #else
            #error "JTEST_HOST and JTEST_TARGET undefined."
        #endif
    
    #endif

}

#ifdef JTEST_TARGET
int JTEST_run_on_target(void){
    printf("\n\t JTEST started in TARGET mode\n\n");
    printf("TEST FILE loaded: %s\n", JT_test_file_);
    printf("--------------------------------------\n");
    JTEST_run_tests();
    JTEST_end_of_test(count, false);

    return 0;
}
#endif //JTEST_TARGET

#ifdef JTEST_HOST
int JTEST_run_on_host(void){
    printf("\n\t JTEST started in HOST mode\n\n");
    printf("TEST FILE loaded: %s\n", JT_test_file_);
    printf("--------------------------------------\n");
    JTEST_run_tests();
    JTEST_end_of_test(count, false);

    return 0;
}
#endif //JTEST_HOST


/// @brief Prints result of a JT_TEST block, but actually runs at start of test.
/// This is to make the test macros look nicer. On first_call it should print
/// nothing. Finally, it is called at the end of the JT_TESTS block to print the
/// result of the last test.
/// @param count 
/// @param first_call 
/// @return 
void JTEST_end_of_test(uint16_t count, bool first_call){
    if (!first_call){
         printf("-> PASS\n\t\t %u checks completed.\n", count);
    }
}