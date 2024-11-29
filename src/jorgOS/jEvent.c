#include "jAssert.h"
#include "jEvent.h"

void J_EventQueue_init(J_EventQueue * jeq, J_Event * e_buffer, uint8_t size){
    J_REQUIRE( jeq != (void *)0);
    J_REQUIRE( e_buffer != (void *)0);
    J_REQUIRE( size > 0);

    // Initialze the circulur buffer holding the events.
    jeq->start = e_buffer;
    jeq->end = e_buffer + (size-1);
    jeq->head = e_buffer;
    jeq->tail = e_buffer;
    jeq->capacity = size;
    jeq->size = size;

}
void J_EventQueue_put(J_EventQueue * jeq, J_Event e){
    // Simply crash if too many events are put in the queue.
    J_REQUIRE( jeq->capacity > 0 );
    J_REQUIRE( jeq->head >= jeq->start && jeq->head <= jeq->end);
    J_REQUIRE( jeq->tail >= jeq->start && jeq->tail <= jeq->end);

    // Add the event to the actual buffer by derefenencing the pointer
    *(jeq->tail) =  e;
    ++jeq->tail;

    if (jeq->tail > jeq->end){
        jeq->tail = jeq-> start;
    }

    --jeq->capacity;

    J_ENSURE( jeq->head >= jeq->start && jeq->head <= jeq->end);
    J_ENSURE( jeq->tail >= jeq->start && jeq->tail <= jeq->end);
    J_ENSURE( jeq->capacity <= jeq->size);

}
/// @brief Return a J_Event * pointer from the queue. Should not be called on an
/// empty queue.
/// @param jeq 
/// @return J_Event * pointer to the first element in the queue.
J_Event J_EventQueue_get(J_EventQueue * jeq){
    // Ensure the queue isn't empty. This function should simply not be called
    // on an empty queue.
    J_REQUIRE (jeq->capacity != jeq->size && jeq->capacity < jeq->size);
    J_REQUIRE( jeq->head >= jeq->start && jeq->head <= jeq->end);
    J_REQUIRE( jeq->tail >= jeq->start && jeq->tail <= jeq->end);
    // Update the head and capacity counter, as if an element was removed
    // already. Of course, the element still needs to be returned.
    ++jeq->head;
    ++jeq->capacity;
    // Reset the head to start if it moved past the end of the buffer.
    if (jeq->head > jeq->end){
        jeq->head = jeq->start;
    }
    J_ENSURE( jeq->head >= jeq->start && jeq->head <= jeq->end );
    J_ENSURE( jeq->capacity <= jeq->size );
    // The head was already moved, so subtract 1 from the pointer.
    return *(jeq->head-1);
}

bool J_EventQueue_isEmpty(J_EventQueue * jeq){
    J_REQUIRE(jeq->capacity <= jeq->size);
    return jeq->capacity == jeq->size;

}