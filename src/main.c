#include <stdint.h>

#include "TM4C123.h"
#include "bsp.h"
#include "bsp_timer.h"
#include "bsp_led.h"
#include "jorgOS.h"
#include "jEventQueueThread.h"
#include "jAssert.h"
#include "jSema.h"
#include "jMutex.h"
#include "runtime_environment.h"
#include "led_manager.h"
#include "qei.h"


// Semaphore test
J_sema sema_test;
// Mutex test
J_mutex mutex_test;


#define MUTEX_ONE_STACKSIZE 100U
#define PRIO_MUTEX_ONE 12U
uint32_t mutex_one_stack[MUTEX_ONE_STACKSIZE];
OS_Thread mutex_one_thread;

void mutex_one(void){
    JSM_PRINTF("MUTEX ONE\n");
    while(1) {
        J_ASSERT_TCB_INTEGRITY;
        J_ASSERT_STACK_INTEGRITY(mutex_one_stack, MUTEX_ONE_STACKSIZE);
        JSM_PRINTF("Mutex one: Attempting to acquire mutex.\n");
        J_Mutex_acquire(&mutex_test);
        JSM_PRINTF("Mutex one: Mutex acquired.\n");
        OS_delay(BSP_TICKS_PER_SEC*10);
        JSM_PRINTF("Mutex one: Mutex about to be released.\n");
        J_Mutex_release(&mutex_test);
        
        OS_delay(BSP_TICKS_PER_SEC / 2U);

    }
}

#define MUTEX_TWO_STACKSIZE 100U
#define PRIO_MUTEX_TWO 11U
uint32_t mutex_two_stack[MUTEX_TWO_STACKSIZE];
OS_Thread mutex_two_thread;
void mutex_two(void){
    JSM_PRINTF("MUTEX TWO\n");
    while(1) {
        J_ASSERT_TCB_INTEGRITY;
        J_ASSERT_STACK_INTEGRITY(mutex_two_stack, MUTEX_TWO_STACKSIZE);
        JSM_PRINTF("Mutex two: Attempting to acquire mutex.\n");
        J_Mutex_acquire(&mutex_test);
        JSM_PRINTF("Mutex two: Mutex acquired.\n");
        OS_delay(BSP_TICKS_PER_SEC*3);
        JSM_PRINTF("Mutex two: About to release mutex.\n");
        J_Mutex_release(&mutex_test);
        
        

    }
}

#define BLINKY_BLUE_STACKSIZE 100U
#define PRIO_BLINKY_BLUE 5U
uint32_t blinky_blue_stack[BLINKY_BLUE_STACKSIZE];
OS_Thread blinky_blue_thread;

void blinky_blue(void){
    JSM_PRINTF("BLINKY BLUE\n");
    while(1) {
        J_ASSERT_TCB_INTEGRITY;
        J_ASSERT_STACK_INTEGRITY(blinky_blue_stack, BLINKY_BLUE_STACKSIZE);
        //BSP_LED_blue_on();    
        OS_delay(3*BSP_TICKS_PER_SEC); 
        //BSP_LED_blue_off();
        OS_delay(1*BSP_TICKS_PER_SEC); 
    }
}

#define ENCODER_MONITOR_STACKSIZE 100U
#define PRIO_ENCODER_MONITOR 31U
uint32_t encoder_monitor_stack[ENCODER_MONITOR_STACKSIZE];
OS_Thread encoder_monitor_thread;

void encoder_monitor(void){
    JSM_PRINTF("Encoder monitor.\n");

    // Monitor the QEI_0 position, and create events for the knob moving and/or
    // down.
    
    // First, we get the current position, and every-time we simply check if the
    // qei travelled up or down, emitting multiple events if the knob turned up
    // multiple times.

    while(1) {
        J_ASSERT_TCB_INTEGRITY;
        J_ASSERT_STACK_INTEGRITY(encoder_monitor_stack, ENCODER_MONITOR_STACKSIZE);   
        OS_delay(BSP_TICKS_PER_SEC/10U); 
        JSM_PRINTF("QEI 0 Pos: %u\n", QEI_0_get_position());
    }
}

#define ONE_SECOND_PRINT_STACKSIZE 100U
#define PRIO_ONE_SECOND_PRINT 32U
uint32_t one_second_print_stack[ONE_SECOND_PRINT_STACKSIZE];
OS_Thread one_second_print_thread;

void one_second_print(void){
    JSM_PRINTF("TWO SECOND PRINT...\n");
    while(1) {
        J_ASSERT_TCB_INTEGRITY;
        J_ASSERT_STACK_INTEGRITY(one_second_print_stack, ONE_SECOND_PRINT_STACKSIZE);   
        OS_delay(2*BSP_TICKS_PER_SEC); 
        JSM_PRINTF("[%u] 2 second later....\n", BSP_get_time_millis());
    }
}


#define SEMA_RED_STACKSIZE 100U
#define PRIO_SEMA_RED 10U
uint32_t sema_red_stack[SEMA_RED_STACKSIZE];
OS_Thread sema_red_thread;

// Thread to see if semaphore works in response to signals from another thread.
void sema_red(void){
    JSM_PRINTF("SEMA RED\n");
    while (1) {
        J_ASSERT_TCB_INTEGRITY;
        J_ASSERT_STACK_INTEGRITY(sema_red_stack, SEMA_RED_STACKSIZE);
        // Blink briefly after the semaphore has been signaled.
        JSM_PRINTF("Sema_red waiting... \n");
        //BSP_LED_red_on();
        J_sema_wait(&sema_test);
        JSM_PRINTF("Sema_red thread continues. \n");
        
        OS_delay(BSP_TICKS_PER_SEC*5U);
        //BSP_LED_red_off();

    }
}

#define SEMA_TEST_SIGNAL_STACKSIZE 100U
#define PRIO_SEMA_TEST_SIGNAL 9U
uint32_t sema_test_signal_stack[SEMA_TEST_SIGNAL_STACKSIZE];
OS_Thread sema_test_signal_thread;

// Dummy thread to signal a semaphore.
void sema_test_signal(void){
    JSM_PRINTF("SEMA TEST SIGNAL \n");
    JSM_transmit_buffer();
    while(1){
        J_ASSERT_TCB_INTEGRITY;
        J_ASSERT_STACK_INTEGRITY(sema_test_signal_stack, SEMA_TEST_SIGNAL_STACKSIZE);
        JSM_PRINTF("Sema test about to be  signaled... \n");
        J_sema_signal(&sema_test);
        
        OS_delay(BSP_TICKS_PER_SEC*15U); // Wait longer than the sema_red threads sleeps.

    }
}

// Create an EventQueue Thread, which also needs to be passed a J_event buffer
#define EVENT_TEST_STACKSIZE 100U
#define PRIO_EVENT_TEST 4U
#define EVENT_TEST_EVENT_BUFFER_SIZE 100U
uint32_t event_test_stack[EVENT_TEST_STACKSIZE];
OS_EventQueue_Thread event_test_eq_thread;
J_Event event_test_event_buffer[EVENT_TEST_EVENT_BUFFER_SIZE];

// The event handler

void event_test_handler(J_Event e){
    //JSM_PRINTF("Event %u\n", e.sig);
    JSM_transmit_buffer();
    switch (e.sig)
    {
        
    case INIT_SIG:
        JSM_PRINTF("EVENT TEST: INIT SIG handled!\n");
        break;
    
    default:
        J_ERROR(); // Should not be reached.
        break;
    }
}

// Create an EventQueue Thread solely for responding to events the BSP produces.
#define BSP_EQT_STACKSIZE 100U
#define PRIO_BSP_EQT 17U
#define BSP_EQT_EVENT_BUFFER_SIZE 5U
uint32_t bsp_eqt_stack[BSP_EQT_STACKSIZE];
OS_EventQueue_Thread bsp_eqt_thread;
J_Event bsp_eqt_event_buffer[BSP_EQT_EVENT_BUFFER_SIZE];

// Led colour setting
BSP_LED_HSL hsl = {
    0,
    SFP_11_20_ONE,
    SFP_11_20_ONE/2
};


// BSP event handler
void bsp_event_handler(J_Event e){
    //JSM_PRINTF("Event %u\n", e.sig);
    JSM_transmit_buffer();
    switch (e.sig)
    {
        
    case INIT_SIG:
        JSM_PRINTF("BSP EQT: INIT SIG handled!\n");
        break;
    
    case BUTTON_1_DEPRESSED:
        // Testing the semaphore.
       
        JSM_PRINTF("BUTTON 1 PRESSED\n");
        LM_set_active_mode(LM_ALWAYS_ON_MODE, true);
        
        //J_sema_signal(&sema_test);

        break;

    case BUTTON_1_RELEASED:
        JSM_PRINTF("BUTTON 1 RELEASED\n");
        LM_set_active_mode(LM_BLINKING_MODE, true);
        break;

    case BUTTON_2_DEPRESSED:
        // Test cycling through hues.

        JSM_PRINTF("BUTTON 2 PRESSED\n");
        LM_set_active_mode(LM_ALWAYS_ON_MODE, true);
        break;

    case BUTTON_2_RELEASED:
        JSM_PRINTF("BUTTON 2 RELEASED\n");
        LM_set_active_mode(LM_HUE_SHIFTING_MODE, true);
        break;

    case KNOB_1_CLOCKWISE:
        JSM_PRINTF("KNOB 1 TURNED CLOCKWISE\n");
        break;

    case KNOB_1_ANTI_CLOCKWISE:
        JSM_PRINTF("KNOB 1 TURNED ANTI-CLOCKWISE\n");
        break;

    default:
        J_ERROR(); // Should not be reached.
        break;
    }
}

int main(void) {
    BSP_init();
    JSM_PRINTF("[%u] BSP initialized...\n", BSP_get_time_millis());
    BSP_LED_set_color_HSL(&hsl);
    
    LM_init(LM_BLINKING_MODE);
    JSM_PRINTF("[%u] LM initialized...\n", BSP_get_time_millis());
    BSP_LED_HSL hsl_3 = {0, SFP_11_20_ONE, SFP_11_20_ONE };
    LM_config_always_on_mode(&hsl_3);
    BSP_LED_HSL hsl_2 = { SFP_11_20_ONE*240, SFP_11_20_ONE, SFP_11_20_ONE /2 };
    LM_config_blinking_mode(&hsl_2,2000U, 1000U);
    LM_config_hue_shifting_mode(&hsl_2, 30000U);

    OS_init();
    JSM_PRINTF("[%u] OS initialized...\n", BSP_get_time_millis());

    // Initialize semaphores and mutexes
    J_sema_init(&sema_test, 0, 10);
    J_Mutex_init(&mutex_test);

    // Initialize the BSP event thread and register the thread to it, in which
    // the BSP package can post events.
    OS_EventQueue_Thread_start(&bsp_eqt_thread, PRIO_BSP_EQT, & bsp_event_handler,
        bsp_eqt_stack, sizeof(bsp_eqt_stack),
        bsp_eqt_event_buffer, BSP_EQT_EVENT_BUFFER_SIZE);
    BSP_register_EQT_thread(&bsp_eqt_thread);

    // Blinky test threads
    OS_Thread_start(&blinky_blue_thread, PRIO_BLINKY_BLUE, &blinky_blue, 
        blinky_blue_stack, sizeof(blinky_blue_stack));
    
    OS_Thread_start(&one_second_print_thread, PRIO_ONE_SECOND_PRINT, &one_second_print, 
        one_second_print_stack, sizeof(one_second_print_stack));

    OS_Thread_start(&encoder_monitor_thread, PRIO_ENCODER_MONITOR, &encoder_monitor, 
        encoder_monitor_stack, sizeof(encoder_monitor_stack));

    // Sema test threads
    OS_Thread_start(&sema_red_thread, PRIO_SEMA_RED, &sema_red, 
        sema_red_stack, sizeof(sema_red_stack));

    OS_Thread_start(&sema_test_signal_thread, PRIO_SEMA_TEST_SIGNAL, &sema_test_signal, 
        sema_test_signal_stack, sizeof(sema_test_signal_stack));

    // Mutex test threads
    OS_Thread_start(&mutex_one_thread, PRIO_MUTEX_ONE, &mutex_one, 
        mutex_one_stack, sizeof(mutex_one_stack));

    OS_Thread_start(&mutex_two_thread, PRIO_MUTEX_TWO, &mutex_two, 
        mutex_two_stack, sizeof(mutex_two_stack));    

    // Event test threads
    OS_EventQueue_Thread_start(&event_test_eq_thread, PRIO_EVENT_TEST, &event_test_handler,
        event_test_stack, sizeof(event_test_stack), // Stack
        event_test_event_buffer, EVENT_TEST_EVENT_BUFFER_SIZE); // EventQueue buffer
    
    JSM_PRINTF("[%u] Registered all threads. Running OS...\n", BSP_get_time_millis());
    OS_run();

    // After the first thread is scheduled, OS_run() should never return.
    
    J_ERROR();

    
    //return 0;
}

void OS_onIdle(void){
    #ifdef JSM_ENABLE
    JSM_transmit_buffer();
    #endif // JSM_ENABLE
    LM_tick();
    
    __WFI();
}