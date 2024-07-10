/* 
 * Part of jorgOS that keeps track of critical sections.
 * Sometimes code must disable interupts, and re-enable them again.
 * But when this happened in a nested fashion this must NOT happen inside the
 * nested critical sections. Therefore __disable_irq(); and __enable_irq(); are
 * not sufficient unless one is absolutely sure one is not inside a nested
 * section or no critical sections are inside the current section.
 * 
 * This code will simply count how deeply one is nested, enabling interupts only
 * when the counter reaches 0 again.
 * 
 * IMPORTANT: this 
 * 
 */

#ifndef __JCRIT_SEC_H__
#define __JCRIT_SEC_H__

#include <stdbool.h>

#define J_CRIT_SEC_START J_crit_sec_start
#define J_CRIT_SEC_END J_crit_sec_end

void J_crit_sec_init();


void J_crit_sec_start();
void J_crit_sec_end();

bool J_assert_in_crit_sec();


#endif //__JCRIT_SEC_H__