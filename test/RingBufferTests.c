/*
MIT License

Copyright (c) 2025 Henry da Costa

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "RingBufferTests.h"
#include "RingBuffer.h"
#include "RingBufferRo.h"
#include "RingBufferWo.h"
#include "test.h"
#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 15
#define WRITE_STRING "Hello, world!\n"

static bool isEmpty(RingBuffer *rb, void *data, size_t cap) {
    return (RingBuffer_getDataPointer(rb) == data) &&
           (RingBuffer_getByteCapacity(rb) == cap) &&
           (RingBuffer_getWriteByteCapacity(rb) == cap) &&
           (RingBuffer_getReadByteCapacity(rb) == 0) &&
           (RingBuffer_getWriteBytePosition(rb) == 0) &&
           (RingBuffer_getReadBytePosition(rb) == 0) &&
           RingBuffer_isEmpty(rb) && !RingBuffer_isFull(rb);
}

static bool isFull(RingBuffer *rb, void *data, size_t cap) {
    return (RingBuffer_getDataPointer(rb) == data) &&
           (RingBuffer_getByteCapacity(rb) == cap) &&
           (RingBuffer_getWriteByteCapacity(rb) == 0) &&
           (RingBuffer_getReadByteCapacity(rb) == cap) &&
           (RingBuffer_getWriteBytePosition(rb) ==
            RingBuffer_getReadBytePosition(rb)) &&
           !RingBuffer_isEmpty(rb) && RingBuffer_isFull(rb);
}

static bool Ro_isReset(RingBufferRo *rb, void *data, size_t cap) {
    return (RingBufferRo_getDataPointer(rb) == data) &&
           (RingBufferRo_getByteCapacity(rb) == cap) &&
           (RingBufferRo_getReadBytePosition(rb) == 0);
}

static bool Wo_isReset(RingBufferWo *rb, void *data, size_t cap) {
    return (RingBufferWo_getDataPointer(rb) == data) &&
           (RingBufferWo_getByteCapacity(rb) == cap) &&
           (RingBufferWo_getWriteBytePosition(rb) == 0);
}

bool RingBuffer_test(void) {
    tests_run = 0;
    tests_succeeded = 0;

    char buff[BUFF_SIZE];
    char buff_read[BUFF_SIZE];
    char buff_writeBytes[BUFF_SIZE];
    RingBuffer rb;

    strncpy_s(buff_writeBytes, BUFF_SIZE, WRITE_STRING, strlen(WRITE_STRING));

    TEST(!RingBuffer_initialize(NULL, buff, BUFF_SIZE));
    TEST(!RingBuffer_initialize(&rb, NULL, BUFF_SIZE));
    TEST(!RingBuffer_initialize(&rb, buff, 0));
    TEST(RingBuffer_initialize(&rb, buff, BUFF_SIZE));
    TEST(isEmpty(&rb, buff, BUFF_SIZE));

    TEST(RingBuffer_reset(&rb));
    TEST(isEmpty(&rb, buff, BUFF_SIZE));

    TEST(RingBuffer_getDataPointer(NULL) == NULL);
    TEST(isEmpty(&rb, buff, BUFF_SIZE));
    TEST(RingBuffer_getDataPointer(&rb) == buff);
    TEST(isEmpty(&rb, buff, BUFF_SIZE));

    TEST(RingBuffer_getByteCapacity(NULL) == 0);
    TEST(isEmpty(&rb, buff, BUFF_SIZE));
    TEST(RingBuffer_getByteCapacity(&rb) == BUFF_SIZE);
    TEST(isEmpty(&rb, buff, BUFF_SIZE));

    TEST(RingBuffer_getWriteByteCapacity(NULL) == 0);
    TEST(isEmpty(&rb, buff, BUFF_SIZE));
    TEST(RingBuffer_getWriteByteCapacity(&rb) == BUFF_SIZE);
    TEST(isEmpty(&rb, buff, BUFF_SIZE));

    TEST(RingBuffer_getReadByteCapacity(NULL) == 0);
    TEST(isEmpty(&rb, buff, BUFF_SIZE));
    TEST(RingBuffer_getReadByteCapacity(&rb) == 0);
    TEST(isEmpty(&rb, buff, BUFF_SIZE));

    TEST(RingBuffer_isEmpty(NULL));
    TEST(isEmpty(&rb, buff, BUFF_SIZE));
    TEST(RingBuffer_isEmpty(&rb));
    TEST(isEmpty(&rb, buff, BUFF_SIZE));

    TEST(RingBuffer_isFull(NULL));
    TEST(isEmpty(&rb, buff, BUFF_SIZE));
    TEST(!RingBuffer_isFull(&rb));
    TEST(isEmpty(&rb, buff, BUFF_SIZE));

    TEST(RingBuffer_getWriteBytePosition(NULL) == 0);
    TEST(isEmpty(&rb, buff, BUFF_SIZE));
    TEST(RingBuffer_getWriteBytePosition(&rb) == 0);
    TEST(isEmpty(&rb, buff, BUFF_SIZE));

    TEST(RingBuffer_getReadBytePosition(NULL) == 0);
    TEST(isEmpty(&rb, buff, BUFF_SIZE));
    TEST(RingBuffer_getReadBytePosition(&rb) == 0);
    TEST(isEmpty(&rb, buff, BUFF_SIZE));

    TEST(RingBuffer_getWriteByteSpan(NULL) == 0);
    TEST(isEmpty(&rb, buff, BUFF_SIZE));
    TEST(RingBuffer_getWriteByteSpan(&rb) == BUFF_SIZE);
    TEST(isEmpty(&rb, buff, BUFF_SIZE));

    TEST(RingBuffer_getReadByteSpan(NULL) == 0);
    TEST(isEmpty(&rb, buff, BUFF_SIZE));
    TEST(RingBuffer_getReadByteSpan(&rb) == 0);
    TEST(isEmpty(&rb, buff, BUFF_SIZE));

    for (size_t n = 0; n <= BUFF_SIZE; ++n) {
        RingBuffer_initialize(&rb, buff, BUFF_SIZE);
        TEST(RingBuffer_writeBytes(NULL, buff_writeBytes, n) == 0);
        TEST(isEmpty(&rb, buff, BUFF_SIZE));
        TEST(RingBuffer_writeBytes(&rb, NULL, n) == 0);
        TEST(isEmpty(&rb, buff, BUFF_SIZE));

        TEST(RingBuffer_writeBytes(&rb, buff_writeBytes, n) == n);
        TEST(RingBuffer_getDataPointer(&rb) == buff);
        TEST(RingBuffer_getByteCapacity(&rb) == BUFF_SIZE);
        TEST(RingBuffer_getWriteByteCapacity(&rb) == (BUFF_SIZE - n));
        TEST(RingBuffer_getReadByteCapacity(&rb) == n);
        TEST(RingBuffer_getWriteBytePosition(&rb) == ((n < BUFF_SIZE) ? n : 0));
        TEST(RingBuffer_getReadBytePosition(&rb) == 0);
        TEST(RingBuffer_isEmpty(&rb) == (n == 0));
        TEST(RingBuffer_isFull(&rb) == (n == BUFF_SIZE));

        TEST(RingBuffer_writeBytes(&rb, buff_writeBytes, BUFF_SIZE) ==
             (BUFF_SIZE - n));
        TEST(isFull(&rb, buff, BUFF_SIZE));

        TEST(RingBuffer_reset(&rb));
        TEST(isEmpty(&rb, buff, BUFF_SIZE));
    }

    for (size_t n = 0; n <= BUFF_SIZE; ++n) {
        RingBuffer_initialize(&rb, buff, BUFF_SIZE);
        TEST(RingBuffer_discardBytes(NULL, n) == 0);
        TEST(isEmpty(&rb, buff, BUFF_SIZE));
        TEST(RingBuffer_discardBytes(&rb, n) == 0);
        TEST(isEmpty(&rb, buff, BUFF_SIZE));

        TEST(RingBuffer_writeBytes(&rb, buff_writeBytes, BUFF_SIZE) ==
             BUFF_SIZE);
        TEST(RingBuffer_discardBytes(&rb, n) == n);
        TEST(RingBuffer_getDataPointer(&rb) == buff);
        TEST(RingBuffer_getByteCapacity(&rb) == BUFF_SIZE);
        TEST(RingBuffer_getReadByteCapacity(&rb) == (BUFF_SIZE - n));
        TEST(RingBuffer_getWriteByteCapacity(&rb) == n);
        TEST(RingBuffer_getReadBytePosition(&rb) == ((n < BUFF_SIZE) ? n : 0));
        TEST(RingBuffer_getWriteBytePosition(&rb) == 0);
        TEST(RingBuffer_isEmpty(&rb) == (n == BUFF_SIZE));
        TEST(RingBuffer_isFull(&rb) == (n == 0));

        TEST(RingBuffer_discardBytes(&rb, BUFF_SIZE) == (BUFF_SIZE - n));
        TEST(isEmpty(&rb, buff, BUFF_SIZE));
    }

    for (size_t n = 0; n <= BUFF_SIZE; ++n) {
        RingBuffer_initialize(&rb, buff, BUFF_SIZE);
        TEST(RingBuffer_readBytes(NULL, buff_read, n) == 0);
        TEST(isEmpty(&rb, buff, BUFF_SIZE));
        TEST(RingBuffer_readBytes(&rb, NULL, n) == 0);
        TEST(isEmpty(&rb, buff, BUFF_SIZE));
        TEST(RingBuffer_readBytes(&rb, buff_read, n) == 0);
        TEST(isEmpty(&rb, buff, BUFF_SIZE));

        memset(buff_read, 0, n);
        TEST(RingBuffer_writeBytes(&rb, buff_writeBytes, BUFF_SIZE) ==
             BUFF_SIZE);
        TEST(RingBuffer_readBytes(&rb, buff_read, n) == n);
        TEST(RingBuffer_getDataPointer(&rb) == buff);
        TEST(RingBuffer_getByteCapacity(&rb) == BUFF_SIZE);
        TEST(RingBuffer_getReadByteCapacity(&rb) == (BUFF_SIZE - n));
        TEST(RingBuffer_getWriteByteCapacity(&rb) == n);
        TEST(RingBuffer_getReadBytePosition(&rb) == ((n < BUFF_SIZE) ? n : 0));
        TEST(RingBuffer_getWriteBytePosition(&rb) == 0);
        TEST(RingBuffer_isEmpty(&rb) == (n == BUFF_SIZE));
        TEST(RingBuffer_isFull(&rb) == (n == 0));
        TEST(strncmp(buff_read, buff_writeBytes, n) == 0);

        TEST(RingBuffer_readBytes(&rb, buff_read, BUFF_SIZE) ==
             (BUFF_SIZE - n));
        TEST(isEmpty(&rb, buff, BUFF_SIZE));
    }

    for (size_t n = 0; n <= BUFF_SIZE; ++n) {
        RingBuffer_initialize(&rb, buff, BUFF_SIZE);
        TEST(RingBuffer_peekBytes(NULL, buff_read, n) == 0);
        TEST(isEmpty(&rb, buff, BUFF_SIZE));
        TEST(RingBuffer_peekBytes(&rb, NULL, n) == 0);
        TEST(isEmpty(&rb, buff, BUFF_SIZE));
        TEST(RingBuffer_peekBytes(&rb, buff_read, n) == 0);
        TEST(isEmpty(&rb, buff, BUFF_SIZE));

        memset(buff_read, 0, n);
        TEST(RingBuffer_writeBytes(&rb, buff_writeBytes, BUFF_SIZE) ==
             BUFF_SIZE);
        TEST(RingBuffer_peekBytes(&rb, buff_read, n) == n);
        TEST(isFull(&rb, buff, BUFF_SIZE));
        TEST(strncmp(buff_read, buff_writeBytes, n) == 0);

        TEST(RingBuffer_readBytes(&rb, buff_read, BUFF_SIZE) == BUFF_SIZE);
        TEST(isEmpty(&rb, buff, BUFF_SIZE));
    }

    for (size_t n = 0; n <= BUFF_SIZE; ++n) {
        for (size_t p = 0; p <= BUFF_SIZE; ++p) {
            RingBuffer_initialize(&rb, buff, BUFF_SIZE);
            TEST(RingBuffer_peekBytesAt(NULL, p, buff_read, n) == 0);
            TEST(isEmpty(&rb, buff, BUFF_SIZE));
            TEST(RingBuffer_peekBytesAt(&rb, p, NULL, n) == 0);
            TEST(isEmpty(&rb, buff, BUFF_SIZE));
            TEST(RingBuffer_peekBytesAt(&rb, p, buff_read, n) == 0);
            TEST(isEmpty(&rb, buff, BUFF_SIZE));

            memset(buff_read, 0, n);
            TEST(RingBuffer_writeBytes(&rb, buff_writeBytes, BUFF_SIZE) ==
                 BUFF_SIZE);
            TEST(RingBuffer_peekBytesAt(&rb, p, buff_read, n) ==
                 ((n < (BUFF_SIZE - p)) ? n : (BUFF_SIZE - p)));
            TEST(isFull(&rb, buff, BUFF_SIZE));
            TEST(strncmp(buff_read, &buff_writeBytes[p],
                         (n < (BUFF_SIZE - p)) ? n : (BUFF_SIZE - p)) == 0);

            TEST(RingBuffer_readBytes(&rb, buff_read, BUFF_SIZE) == BUFF_SIZE);
            TEST(isEmpty(&rb, buff, BUFF_SIZE));
        }
    }

    printf("%s: passed %llu out of %llu\n", __func__, tests_succeeded,
           tests_run);

    return tests_succeeded == tests_run;
}

bool RingBufferRo_test(void) {
    tests_run = 0;
    tests_succeeded = 0;

    char buff[BUFF_SIZE];
    char buff_read[BUFF_SIZE];
    RingBufferRo rb;

    strncpy_s(buff, BUFF_SIZE, WRITE_STRING, strlen(WRITE_STRING));

    TEST(!RingBufferRo_initialize(NULL, buff, BUFF_SIZE));
    TEST(!RingBufferRo_initialize(&rb, NULL, BUFF_SIZE));
    TEST(!RingBufferRo_initialize(&rb, buff, 0));
    TEST(RingBufferRo_initialize(&rb, buff, BUFF_SIZE));
    TEST(Ro_isReset(&rb, buff, BUFF_SIZE));

    TEST(RingBufferRo_reset(&rb));
    TEST(Ro_isReset(&rb, buff, BUFF_SIZE));

    TEST(RingBufferRo_getDataPointer(NULL) == NULL);
    TEST(Ro_isReset(&rb, buff, BUFF_SIZE));
    TEST(RingBufferRo_getDataPointer(&rb) == buff);
    TEST(Ro_isReset(&rb, buff, BUFF_SIZE));

    TEST(RingBufferRo_getByteCapacity(NULL) == 0);
    TEST(Ro_isReset(&rb, buff, BUFF_SIZE));
    TEST(RingBufferRo_getByteCapacity(&rb) == BUFF_SIZE);
    TEST(Ro_isReset(&rb, buff, BUFF_SIZE));

    TEST(RingBufferRo_getReadBytePosition(NULL) == 0);
    TEST(Ro_isReset(&rb, buff, BUFF_SIZE));
    TEST(RingBufferRo_getReadBytePosition(&rb) == 0);
    TEST(Ro_isReset(&rb, buff, BUFF_SIZE));

    for (size_t n = 0; n <= BUFF_SIZE; ++n) {
        RingBufferRo_initialize(&rb, buff, BUFF_SIZE);

        TEST(RingBufferRo_discardBytes(&rb, n) == n);
        TEST(RingBufferRo_getDataPointer(&rb) == buff);
        TEST(RingBufferRo_getByteCapacity(&rb) == BUFF_SIZE);
        TEST(RingBufferRo_getReadBytePosition(&rb) ==
             ((n < BUFF_SIZE) ? n : 0));
    }

    for (size_t n = 0; n <= BUFF_SIZE; ++n) {
        RingBufferRo_initialize(&rb, buff, BUFF_SIZE);

        memset(buff_read, 0, n);
        TEST(RingBufferRo_readBytes(&rb, buff_read, n) == n);
        TEST(RingBufferRo_getDataPointer(&rb) == buff);
        TEST(RingBufferRo_getByteCapacity(&rb) == BUFF_SIZE);
        TEST(RingBufferRo_getReadBytePosition(&rb) ==
             ((n < BUFF_SIZE) ? n : 0));
        TEST(strncmp(buff_read, buff, n) == 0);
    }

    for (size_t n = 0; n <= BUFF_SIZE; ++n) {
        RingBufferRo_initialize(&rb, buff, BUFF_SIZE);

        memset(buff_read, 0, n);
        TEST(RingBufferRo_peekBytes(&rb, buff_read, n) == n);
        TEST(Ro_isReset(&rb, buff, BUFF_SIZE));
        TEST(strncmp(buff_read, buff, n) == 0);
    }

    for (size_t n = 0; n <= BUFF_SIZE; ++n) {
        for (size_t p = 0; p <= BUFF_SIZE; ++p) {
            RingBufferRo_initialize(&rb, buff, BUFF_SIZE);

            memset(buff_read, 0, n);
            TEST(RingBufferRo_peekBytesAt(&rb, p, buff_read, n) == n);
            TEST(strncmp(buff_read, &buff[p],
                         (n < (BUFF_SIZE - p)) ? n : (BUFF_SIZE - p)) == 0);
        }
    }

    printf("%s: passed %llu out of %llu\n", __func__, tests_succeeded,
           tests_run);

    return tests_succeeded == tests_run;
}

bool RingBufferWo_test(void) {
    tests_run = 0;
    tests_succeeded = 0;

    char buff[BUFF_SIZE];
    char buff_writeBytes[BUFF_SIZE];
    RingBufferWo rb;

    strncpy_s(buff_writeBytes, BUFF_SIZE, WRITE_STRING, strlen(WRITE_STRING));

    TEST(!RingBufferWo_initialize(NULL, buff, BUFF_SIZE));
    TEST(!RingBufferWo_initialize(&rb, NULL, BUFF_SIZE));
    TEST(!RingBufferWo_initialize(&rb, buff, 0));
    TEST(RingBufferWo_initialize(&rb, buff, BUFF_SIZE));
    TEST(Wo_isReset(&rb, buff, BUFF_SIZE));

    TEST(RingBufferWo_reset(&rb));
    TEST(Wo_isReset(&rb, buff, BUFF_SIZE));

    TEST(RingBufferWo_getDataPointer(NULL) == NULL);
    TEST(Wo_isReset(&rb, buff, BUFF_SIZE));
    TEST(RingBufferWo_getDataPointer(&rb) == buff);
    TEST(Wo_isReset(&rb, buff, BUFF_SIZE));

    TEST(RingBufferWo_getByteCapacity(NULL) == 0);
    TEST(Wo_isReset(&rb, buff, BUFF_SIZE));
    TEST(RingBufferWo_getByteCapacity(&rb) == BUFF_SIZE);
    TEST(Wo_isReset(&rb, buff, BUFF_SIZE));

    TEST(RingBufferWo_getWriteBytePosition(NULL) == 0);
    TEST(Wo_isReset(&rb, buff, BUFF_SIZE));
    TEST(RingBufferWo_getWriteBytePosition(&rb) == 0);
    TEST(Wo_isReset(&rb, buff, BUFF_SIZE));

    for (size_t n = 0; n <= BUFF_SIZE; ++n) {
        RingBufferWo_initialize(&rb, buff, BUFF_SIZE);
        TEST(RingBufferWo_writeBytes(NULL, buff_writeBytes, n) == 0);
        TEST(Wo_isReset(&rb, buff, BUFF_SIZE));
        TEST(RingBufferWo_writeBytes(&rb, NULL, n) == 0);
        TEST(Wo_isReset(&rb, buff, BUFF_SIZE));

        memset(buff, 0, n);
        TEST(RingBufferWo_writeBytes(&rb, buff_writeBytes, n) == n);
        TEST(RingBufferWo_getDataPointer(&rb) == buff);
        TEST(RingBufferWo_getByteCapacity(&rb) == BUFF_SIZE);
        TEST(RingBufferWo_getWriteBytePosition(&rb) ==
             ((n < BUFF_SIZE) ? n : 0));
        TEST(strncmp(buff, buff_writeBytes, n) == 0);

        TEST(RingBufferWo_reset(&rb));
        TEST(Wo_isReset(&rb, buff, BUFF_SIZE));
    }

    printf("%s: passed %llu out of %llu\n", __func__, tests_succeeded,
           tests_run);

    return tests_succeeded == tests_run;
}