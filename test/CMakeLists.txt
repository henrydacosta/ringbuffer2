enable_testing()

add_executable(RingBufferTest
    RingBufferTests.h
    RingBufferTests.c
    test.c
    main.c
)

target_link_libraries(RingBufferTest PRIVATE RingBufferLib)
