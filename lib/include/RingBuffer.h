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
 * Declares RingBuffer and associated functions.
 *
 * The functions are not thread safe.
 */

#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * A ring buffer.
 *
 * The caller is responsible for thread safety.
 */
typedef struct {
    uint8_t *_data;
    size_t _cap;
    size_t _wpos;
    size_t _rpos;
    size_t _len;
} RingBuffer;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the ring buffer.
 *
 * @param[out]      rb      The ring buffer, must not be @c NULL.
 * @param[in,out]   data    The data memory, or external memory for storing the
 *                          data, must not be @c NULL.
 * @param[in]       cap     The data memory capacity in bytes, must not be zero.
 *
 * @retval  false   A parameter is invalid.
 * @retval  true    Success.
 */
inline bool RingBuffer_initialize(RingBuffer *rb, void *data, size_t cap) {
    if ((rb == NULL) || (data == NULL) || (cap == 0)) {
        return false;
    }
    rb->_data = (uint8_t *)data;
    rb->_cap = cap;
    rb->_wpos = 0;
    rb->_rpos = 0;
    rb->_len = 0;
    return true;
}

/**
 * Resets the ring buffer.
 *
 * @param[in,out]   rb  The ring buffer, must not be @c NULL.
 *
 * @retval  false   The @p rb parameter is @c NULL.
 * @retval  true    Success.
 */
inline bool RingBuffer_reset(RingBuffer *rb) {
    if (rb == NULL) {
        return false;
    }
    rb->_len = 0;
    rb->_rpos = 0;
    rb->_wpos = 0;
    return true;
}

/**
 * Returns the ring buffer's data memory pointer.
 *
 * Returns @c NULL if the @p rb parameter is @c NULL.
 *
 * @param[in]   rb  The ring buffer, must not be @c NULL.
 */
inline void *RingBuffer_getDataPointer(const RingBuffer *rb) {
    return (rb != NULL) ? rb->_data : NULL;
}

/**
 * Returns the ring buffer's data memory capacity in bytes.
 *
 * Returns zero if the @p rb parameter is @c NULL.
 *
 * @param[in]   rb  The ring buffer, must not be @c NULL.
 */
inline size_t RingBuffer_getByteCapacity(const RingBuffer *rb) {
    return (rb != NULL) ? rb->_cap : 0;
}

/**
 * Returns the number of bytes that can be written to the ring buffer before the
 * ring buffer becomes full.
 *
 * Returns zero if the @p rb parameter is @c NULL.
 *
 * @param[in]   rb  The ring buffer, must not be @c NULL.
 */
inline size_t RingBuffer_getWriteByteCapacity(const RingBuffer *rb) {
    return (rb != NULL) ? (rb->_cap - rb->_len) : 0;
}

/**
 * Returns the number of bytes that can be read from the ring buffer before the
 * ring buffer becomes empty.
 *
 * Returns zero if the @p rb parameter is @c NULL.
 *
 * @param[in]   rb  The ring buffer, must not be @c NULL.
 */
inline size_t RingBuffer_getReadByteCapacity(const RingBuffer *rb) {
    return (rb != NULL) ? rb->_len : 0;
}

/**
 * Returns whether the ring buffer is empty.
 *
 * This is equivalent to <code>RingBuffer_getReadByteCapacity(rb) == 0</code>
 * and provided for expressive clarity and convenience.
 *
 * Returns @c true if the @p rb parameter is @c NULL.
 *
 * @param[in]   rb  The ring buffer, must not be @c NULL.
 */
inline bool RingBuffer_isEmpty(const RingBuffer *rb) {
    return (rb != NULL) ? (rb->_len == 0) : true;
}

/**
 * Returns whether the ring buffer is full.
 *
 * This is equivalent to <code>RingBuffer_getWriteByteCapacity(rb) == 0</code>
 * and provided for expressive clarity and convenience.
 *
 * Returns @c true if the @p rb parameter is @c NULL.
 *
 * @param[in]   rb  The ring buffer, must not be @c NULL.
 */
inline bool RingBuffer_isFull(const RingBuffer *rb) {
    return (rb != NULL) ? (rb->_len == rb->_cap) : true;
}

/**
 * Returns the ring buffer's write position.
 *
 * Returns zero if the @p rb parameter is @c NULL.
 *
 * @param[in]   rb  The ring buffer, must not be @c NULL.
 */
inline size_t RingBuffer_getWriteBytePosition(const RingBuffer *rb) {
    return (rb != NULL) ? rb->_wpos : 0;
}

/**
 * Returns the ring buffer's read position.
 *
 * Returns zero if the @p rb parameter is @c NULL.
 *
 * @param[in]   rb  The ring buffer, must not be @c NULL.
 */
inline size_t RingBuffer_getReadBytePosition(const RingBuffer *rb) {
    return (rb != NULL) ? rb->_rpos : 0;
}

/**
 * Returns the number of bytes than can be written contigously to the ring
 * buffer.
 *
 * Returns zero if the @p rb parameter is @c NULL.
 *
 * @param[in]   rb  The ring buffer, must not be @c NULL.
 */
extern size_t RingBuffer_getWriteByteSpan(const RingBuffer *rb);

/**
 * Returns the number of bytes than can be read contigously from the ring
 * buffer.
 *
 *
 * Returns zero if the @p rb parameter is @c NULL.
 *
 * @param[in]   rb  The ring buffer, must not be @c NULL.
 */
extern size_t RingBuffer_getReadByteSpan(const RingBuffer *rb);

/**
 * Writes bytes to the ring buffer.
 *
 * @param[in,out]   rb  The ring buffer, must not be @c NULL.
 * @param[out]      buf The source memory, must not be @c NULL.
 * @param[in]       len The number of bytes to copy from the source memory to
 * the ring buffer.
 *
 * @return  The number of bytes copied or zero if a parameter is invalid.
 */
extern size_t RingBuffer_writeBytes(RingBuffer *rb, const void *buf,
                                    size_t len);

/**
 * Discards bytes from the ring buffer.
 *
 * @param[in,out]   rb  The ring buffer, must not be @c NULL.
 * @param[in]       len The number of bytes to skip.
 *
 * @return  The number of bytes skipped or zero if a parameter is invalid.
 */
extern size_t RingBuffer_discardBytes(RingBuffer *rb, size_t len);

/**
 * Reads bytes from the ring buffer.
 *
 * @param[in,out] rb  The ring buffer, must not be @c NULL.
 * @param[out]    buf The destination memory, must not be @c NULL.
 * @param[in]     len The number of bytes to copy from the ring buffer to the
 *                    destination memory.
 *
 * @return  The number of bytes copied or zero if a parameter is invalid.
 */
extern size_t RingBuffer_readBytes(RingBuffer *rb, void *buf, size_t len);

/**
 * Peeks bytes from the ring buffer.
 *
 * This is equivalent to <code>RingBuffer_peekBytesAt(rb, 0, buf, len)</code>.
 *
 * @param[in]   rb  The ring buffer, must not be @c NULL.
 * @param[out]  buf The destination memory, must not be @c NULL.
 * @param[in]   len The number of bytes to peek.
 *
 * @return  The number of bytes copied to the destination buffer or zero if a
 *          parameter is invalid.
 */
extern size_t RingBuffer_peekBytes(const RingBuffer *rb, void *buf, size_t len);

/**
 * Peeks bytes from the ring buffer at a byte offset from the ring buffer's read
 * position.
 *
 * @param[in]   rb  The ring buffer, must not be @c NULL.
 * @param[in]   pos The byte offset from the ring buffer's read position at
 *                  which to peek.
 * @param[out]  buf The destination memory, must not be @c NULL.
 * @param[in]   len The number of bytes to peek.
 *
 * @return  The number of bytes copied to the destination buffer or zero if a
 *          parameter is invalid.
 */
extern size_t RingBuffer_peekBytesAt(const RingBuffer *rb, size_t pos,
                                     void *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif // _RINGBUFFER_H