#include "jtest.h"
#include <stdio.h>

int main(void){
    #ifdef JTEST_TARGET
        return run_test_on_target();
    #else
        #ifdef JTEST_HOST
        return run_test_on_host();
        #else
            #error "JTEST_HOST and JTEST_TARGET undefined."
        #endif
    
    #endif

}


int run_test_on_target(void){
    JTEST_run_test();

    return 0;
}

#ifdef JTEST_HOST
int run_test_on_host(void){
    printf("\n\t JTEST started in HOST mode\n\n");
    printf("TEST STARTED: %s\n", JT_test_file_);
    printf("-------------");
    JTEST_run_test();

    return 0;
}
#endif //JTEST_HOST