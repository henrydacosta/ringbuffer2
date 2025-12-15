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

/**
 * @file
 * Declares RingBufferWo and associated functions.
 *
 * The functions are not thread safe.
 */

#ifndef _RINGBUFFERWO_H
#define _RINGBUFFERWO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * A writeBytes-only ring buffer.
 *
 * The data must be read by external code.
 *
 * Acts like a ring buffer that is always empty.
 */
typedef struct {
    uint8_t *_data;
    size_t _cap;
    size_t _wpos;
} RingBufferWo;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the writeBytes-only ring buffer.
 *
 * @param[out]      rb      The writeBytes-only ring buffer, must not be @c
 *                          NULL.
 * @param[in,out]   data    The data memory, or external memory for storing the
 *                          data, must not be @c NULL.
 * @param[in]       cap     The data memory capacity in bytes, must not be zero.
 *
 * @retval  false   A parameter is invalid.
 * @retval  true    Success.
 */
inline bool RingBufferWo_initialize(RingBufferWo *rb, void *data, size_t cap) {
    if ((rb == NULL) || (data == NULL) || (cap == 0)) {
        return false;
    }
    rb->_data = (uint8_t *)data;
    rb->_cap = cap;
    rb->_wpos = 0;
    return true;
}

/**
 * Resets the writeBytes-only ring buffer.
 *
 * @param[in,out]   rb  The writeBytes-only ring buffer, must not be @c NULL.
 *
 * @retval  false   The @p rb parameter is @c NULL.
 * @retval  true    Success.
 */
inline bool RingBufferWo_reset(RingBufferWo *rb) {
    if (rb == NULL) {
        return false;
    }
    rb->_wpos = 0;
    return true;
}

/**
 * Returns the write-only ring buffer's data pointer.
 *
 * Returns @c NULL if the @p rb parameter is @c NULL.
 *
 * @param[in]   rb  The write-only ring buffer, must not be @c NULL.
 */
inline void *RingBufferWo_getDataPointer(const RingBufferWo *rb) {
    return (rb != NULL) ? rb->_data : NULL;
}

/**
 * Returns the write-only ring buffer's data memory capacity in bytes.
 *
 * Returns zero if the @p rb parameter is @c NULL.
 *
 * @param[in]   rb  The write-only ring buffer, must not be @c NULL.
 */
inline size_t RingBufferWo_getByteCapacity(const RingBufferWo *rb) {
    return (rb != NULL) ? rb->_cap : 0;
}

/**
 * Returns the write-only ring buffer's write position.
 *
 * Returns zero if the @p rb parameter is @c NULL.
 *
 * @param[in]   rb  The write-only ring buffer, must not be @c NULL.
 */
inline size_t RingBufferWo_getWriteBytePosition(const RingBufferWo *rb) {
    return (rb != NULL) ? rb->_wpos : 0;
}

/**
 * Writes data to the write-only ring buffer.
 *
 * @param[in,out]   rb  The write-only ring buffer, must not be @c NULL.
 * @param[out]      buf The source memory, must not be @c NULL.
 * @param[in]       len The number of bytes to copy from the source memory to
 *                      the ring buffer.
 *
 * @return  The number of bytes copied or zero if a parameter is invalid.
 */
extern size_t RingBufferWo_writeBytes(RingBufferWo *rb, const void *buf,
                                      size_t len);

#ifdef __cplusplus
}
#endif

#endif // _RINGBUFFERWO_H