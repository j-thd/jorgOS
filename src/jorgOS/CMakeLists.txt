target_link_libraries(jorgOS runtime_environment bsp)
target_sources(jorgOS
    PRIVATE
        jAssert.c
        jAssert.h
        jCritSec.c
        jCritSec.h
        jEvent.c
        jEvent.h
        jEventQueueThread.c
        jEventQueueThread.h
        jMutex.c
        jMutex.h
        jorgOS.c
        jorgOS.h
        jSema.c
        jSema.h
        jSerialMonitor.c
        jSerialMonitor.h
        jTCB.h

    PUBLIC
        FILE_SET jorgOSHeaders
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            jAssert.h
            jCritSec.h
            jEvent.h
            jEventQueueThread.h
            jMutex.h
            jorgOS.h
            jSema.h
            jSerialMonitor.h
            jTCB.h
)