add_library(RingBufferLib
    include/RingBuffer.h
    include/RingBufferRo.h
    include/RingBufferWo.h
    src/RingBuffer.c
    src/RingBufferRo.c
    src/RingBufferWo.c
)

set_target_properties(RingBufferLib PROPERTIES OUTPUT_NAME "ringbuffer")

target_include_directories(RingBufferLib PUBLIC include)
