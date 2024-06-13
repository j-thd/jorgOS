#include <stdio.h> // Make sure to set MICROlib as the standard library

#include "runtime_environment.h"
#include "jSerialMonitor.h"
#include "jAssert.h"
#include "uart.h"

// To make sure the serial monitor is not blocking (too much) a buffer will be
// used to store the characters to send. The buffer will be read and sent in the
// idle thread. The buffer will be cyclic.

#define JSM_BUFFER_SIZE 200 // How many characters are stored in the buffer. 
static uint8_t buffer[JSM_BUFFER_SIZE];
static uint8_t * start;  // The start of the buffer in memory
static uint8_t * end; // The end of the buffer in memory
static uint8_t* head; // Points to the head of stored characters in the buffer
static uint8_t* tail; // Points to the tail of stored characters in the buffer
static uint8_t capacity; // How many characters still fit in the buffer

static UART0_Type * UARTn; // Pointer to the UART struct to be used. Usually UART0.

/// @brief Initializes the Serial Monitor over UART
/// @param UARTn the UART used to send characters
void JSM_init(UART0_Type * UARTn_for_serial_monitor){
    // Initialize the pointers of the cyclic buffer to the start
    start = buffer;
    end = &buffer[JSM_BUFFER_SIZE-1];
    head = start;
    tail = start;
    capacity = JSM_BUFFER_SIZE;

    // Store which UART is used.
    UARTn = UARTn_for_serial_monitor;
}

// fputc provided so printf can be used.
int fputc(int c, FILE * stream){
    (void)stream; //Unused paramater
    J_REQUIRE( capacity > 0 );
    J_REQUIRE( head >= start && head <= end);
    J_REQUIRE( tail >= start && tail <= end);
    
    // First write the character and then move the tail
    *tail = (char)c;
    // Increment the tail, wrap it around if it exceeds end
    ++tail;
    if (tail > end){
        tail = start;
    }
    
    --capacity; // Decrease the capacity by 1.

    // If the capacity reaches 0, make sure the entire buffer is transmitted
    // first. But also print a warning. Ideally this should never occur and the
    // buffer is transmitted in the idle thread.
    if (capacity == 0){
        JSM_transmit_buffer();
        // A newline is added to make sure at least part of the message is
        // printed, incase something really weird happens with the capacity (or
        // an insane buffer size is set.)
        printf("\nJSM buffer full!\n"); 
        JSM_transmit_buffer();
    }

    J_ENSURE( head >= start && head <= end);
    J_ENSURE( tail >= start && tail <= end);
    J_ENSURE( capacity >= 0 && capacity <= JSM_BUFFER_SIZE);

    return c;
}

/// @brief Takes one character from the cyclic buffer to send over UART.
/// @attention Should only be called in a critical section.
static void transmit_char(){
    J_REQUIRE( head >= start && head <= end);
    J_REQUIRE( tail >= start && tail <= end);

    // Write the character to UART, and update the counter and capacity
    UART_write(UARTn, *head);
    ++head;
    ++capacity;
    // Reset the head to start if it moved past the end of the buffer.
    if (head > end){
        head = start;
    }


    J_REQUIRE( head >= start && head <= end);
    J_ENSURE( capacity >= 0 && capacity <= JSM_BUFFER_SIZE);

}

void JSM_transmit_buffer(){
    J_REQUIRE(capacity <= JSM_BUFFER_SIZE);
    //Transmit everything in the buffer

    while(capacity != JSM_BUFFER_SIZE){
        transmit_char();
    }
}

/// @brief Returns how many characters still fit in the buffer, used to update
/// capacity after buffer is read and (partially) emptied.
/// @attention Must only be called in a critical section 
/// @return Amount of characters that still fit in the buffer
static uint8_t calc_capacity(){ // Perhaps can be deleted.

    // The calculation depends on whether the tail is ahead of the tail
    uint8_t cap;
    if (tail >= head){
        cap = JSM_BUFFER_SIZE - (tail - head);
    } 
    // If the tail is before the head
    else { 
        // Buffer size is irrelevant in this case, as the capacity is contained
        // between the head and the fail.
        cap = (head-tail) - 1;
    }

    return cap;
}
