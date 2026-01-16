#ifndef __JEVENT_H_
#define __JEVENT_H_


#include <stdint.h>
#include <stdbool.h>

/// @brief The base J_event class. Can potentially be subclassed to pass more
/// information along with the event.
typedef struct J_Event {
    uint8_t sig; // The event to be handled. Events should be defined in an enum.
    uint16_t repeats; // How often the event is duplicated
} J_Event;

#define J_EVENT_MAX_REPEATS UINT16_MAX

// Default events.
enum DefaultSignals{
    STOP_SIG,
    INIT_SIG,
    USER_SIG // When custom signals are defined, the first one must be equal to USER_SIG
};

/// @brief A FIFO queue for the J_Event object. Implemented as a circular buffer.
typedef struct J_EventQueue{
    J_Event * start;
    J_Event * end;
    J_Event * head;
    J_Event * tail;
    uint8_t capacity;
    uint8_t size;
} J_EventQueue;

void J_EventQueue_init(J_EventQueue *, J_Event *, uint8_t size);
void J_EventQueue_put(J_EventQueue *, J_Event);
J_Event J_EventQueue_get(J_EventQueue *);
bool J_EventQueue_isEmpty(J_EventQueue *);


#endif // __JEVENT_H_