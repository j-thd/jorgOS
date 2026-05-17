target_sources(jlib
    PRIVATE
        jfp.c
        jfp.h
        jmath.c
        jmath.h
        jUtil.h

    PUBLIC
        FILE_SET jlibHeaders
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            jfp.h
            jmath.h
            jUtil.h
)