
#include <stdint.h>

#include "runtime_environment.h"
#include "jCritSec.h"
#include "jAssert.h"



static uint8_t critical_section_depth; // Counts how deeply nested the criticals sections are

/// @brief The critical section depth must be initialized to 0 when the OS
/// starts. 
void J_crit_sec_init(){
    critical_section_depth = 0;
}

/// @brief Start a critical section, with jorgOS keeping track of nested
/// sections. It is mandatory to use this unless nesting is impossible.
void J_crit_sec_start(){
    // The counter must not overflow beyond 255.
    J_REQUIRE( critical_section_depth < 254 );
    // First disable the interupts, then increment the counter.
    __disable_irq(); 
    ++critical_section_depth;
}

/// @brief End a critcal section, with jorgOS keeping track of nested sections.
/// Interrupts only get re-enabled after the top of nested sections is exited.
/// It is mandatory to use this unless nesting is impossible. 
void J_crit_sec_end(){
    // The counter must not underflow. This would also indicate an error in the
    // code somewhere.
    J_REQUIRE( critical_section_depth > 0);
    // First decrement the counter, then possibly re-enable interrupts.
    --critical_section_depth;

    if (critical_section_depth == 0){
        __enable_irq();
    }
}
