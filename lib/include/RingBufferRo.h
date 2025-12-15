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
 * Declares RingBufferRo and associated functions.
 *
 * The functions are not thread safe.
 */

#ifndef _RINGBUFFERRO_H
#define _RINGBUFFERRO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * A read-only ring buffer.
 *
 * The data must be written by external code.
 *
 * Acts like a ring buffer that is always full.
 */
typedef struct {
    uint8_t *_data;
    size_t _cap;
    size_t _rpos;
} RingBufferRo;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the read-only ring buffer.
 *
 * @param[out]      rb      The read-only ring buffer, must not be @c NULL.
 * @param[in,out]   data    The data memory, or external memory for storing the
 *                          data, must not be @c NULL.
 * @param[in]       cap     The data memory capacity in bytes, must not be zero.
 *
 * @retval  false   A parameter is invalid.
 * @retval  true    Success.
 */
inline bool RingBufferRo_initialize(RingBufferRo *rb, void *data, size_t cap) {
    if ((rb == NULL) || (data == NULL) || (cap == 0)) {
        return false;
    }
    rb->_data = (uint8_t *)data;
    rb->_cap = cap;
    rb->_rpos = 0;
    return true;
}

/**
 * Resets the read-only ring buffer.
 *
 * @param[in,out]   rb  The read-only ring buffer, must not be @c NULL.
 *
 * @retval  false   The @p rb parameter is @c NULL.
 * @retval  true    Success.
 */
inline bool RingBufferRo_reset(RingBufferRo *rb) {
    if (rb == NULL) {
        return false;
    }
    rb->_rpos = 0;
    return true;
}

/**
 * Returns the read-only ring buffer's data pointer.
 *
 * Returns @c NULL if the @p rb parameter is @c NULL.
 *
 * @param[in]   rb  The read-only ring buffer, must not be @c NULL.
 */
inline void *RingBufferRo_getDataPointer(const RingBufferRo *rb) {
    return (rb != NULL) ? rb->_data : NULL;
}

/**
 * Returns the read-only ring buffer's data memory capacity in bytes.
 *
 * Returns zero if the @p rb parameter is @c NULL.
 *
 * @param[in]   rb  The read-only ring buffer, must not be @c NULL.
 */
inline size_t RingBufferRo_getByteCapacity(const RingBufferRo *rb) {
    return (rb != NULL) ? rb->_cap : 0;
}

/**
 * Returns the read-only ring buffer's read position.
 *
 * Returns zero if the @p rb parameter is @c NULL.
 *
 * @param[in]   rb  The read-only ring buffer, must not be @c NULL.
 */
inline size_t RingBufferRo_getReadBytePosition(const RingBufferRo *rb) {
    return (rb != NULL) ? rb->_rpos : 0;
}

/**
 * Discards bytes from the read-only ring buffer.
 *
 * @param[in,out]   rb  The read-only ring buffer, must not be @c NULL.
 * @param[in]       len The number of bytes to skip.
 *
 * @return  The number of bytes skipped or zero if a parameter is invalid.
 */
inline size_t RingBufferRo_discardBytes(RingBufferRo *rb, size_t len) {
    if ((rb == NULL) || (len == 0)) {
        return 0;
    }
    rb->_rpos = (rb->_rpos + len) % rb->_cap;
    return len;
}

/**
 * Reads bytes from the read-only ring buffer.
 *
 * @param[in,out]   rb  The read-only ring buffer, must not be @c NULL.
 * @param[out]      buf The destination memory, must not be @c NULL.
 * @param[in]       len The number of bytes to copy from the ring buffer to the
 *                      destination memory.
 *
 * @return  The number of bytes copied or zero if a parameter is invalid.
 */
extern size_t RingBufferRo_readBytes(RingBufferRo *rb, void *buf, size_t len);

/**
 * Peeks bytes from the read-only ring buffer.
 *
 * This is equivalent to <code>RingBufferRo_peekBytesAt(rb, 0, buf, len)</code>.
 *
 * @param[in]   rb  The read-only ring buffer, must not be @c NULL.
 * @param[out]  buf The destination memory, must not be @c NULL.
 * @param[in]   len The number of bytes to peek.
 *
 * @return  The number of bytes copied to the destination buffer or zero if a
 *          parameter is invalid.
 */
extern size_t RingBufferRo_peekBytes(const RingBufferRo *rb, void *buf,
                                     size_t len);

/**
 * Peeks bytes from the read-only ring buffer at an offset from the ring
 * buffer's read position.
 *
 * @param[in]   rb  The read-only ring buffer, must not be @c NULL.
 * @param[in]   pos The offset from the ring buffer's current read position at
 *                  which to peek.
 * @param[out]  buf The destination memory, must not be @c NULL.
 * @param[in]   len The number of bytes to peek.
 *
 * @return  The number of bytes copied to the destination buffer or zero if a
 *          parameter is invalid.
 */
extern size_t RingBufferRo_peekBytesAt(const RingBufferRo *rb, size_t pos,
                                       void *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif // _RINGBUFFERRO_H