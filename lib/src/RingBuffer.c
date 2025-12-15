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
 * Implements RingBuffer and associated functions.
 */

#include "RingBuffer.h"
#include <string.h>

size_t RingBuffer_getWriteByteSpan(const RingBuffer *rb) {
    if (rb == NULL) {
        return 0;
    }
    size_t wcap = rb->_cap - rb->_len;
    if ((rb->_wpos + wcap) >= rb->_cap) {
        return rb->_cap - rb->_wpos;
    }
    return wcap;
}

size_t RingBuffer_getReadByteSpan(const RingBuffer *rb) {
    if (rb == NULL) {
        return 0;
    }
    if ((rb->_rpos + rb->_len) >= rb->_cap) {
        return rb->_cap - rb->_rpos;
    }
    return rb->_len;
}

size_t RingBuffer_writeBytes(RingBuffer *rb, const void *buf, size_t len) {
    if ((rb == NULL) || (buf == NULL) || (len == 0)) {
        return 0;
    }
    const uint8_t *tbuf = (uint8_t *)buf;
    size_t wcap = rb->_cap - rb->_len;
    if (len > wcap) {
        len = wcap;
    }
    size_t left = len;
    if ((rb->_wpos + left) >= rb->_cap) {
        size_t copy = rb->_cap - rb->_wpos;
        memcpy(rb->_data + rb->_wpos, tbuf, copy);
        rb->_wpos = 0;
        rb->_len += copy;
        tbuf += copy;
        left -= copy;
    }
    if (left > 0) {
        memcpy(rb->_data + rb->_wpos, tbuf, left);
        rb->_wpos += left;
        rb->_len += left;
    }
    return len;
}

size_t RingBuffer_readBytes(RingBuffer *rb, void *buf, size_t len) {
    if ((rb == NULL) || (buf == NULL) || (len == 0)) {
        return 0;
    }
    if (len > rb->_len) {
        len = rb->_len;
    }
    uint8_t *tbuf = (uint8_t *)buf;
    size_t left = len;
    if ((rb->_rpos + left) >= rb->_cap) {
        size_t copy = rb->_cap - rb->_rpos;
        memcpy(tbuf, rb->_data + rb->_rpos, copy);
        rb->_rpos = 0;
        rb->_len -= copy;
        tbuf += copy;
        left -= copy;
    }
    if (left > 0) {
        memcpy(tbuf, rb->_data + rb->_rpos, left);
        rb->_len -= left;
    }
    if (rb->_len == 0) {
        rb->_wpos = 0;
        rb->_rpos = 0;
    } else {
        rb->_rpos += left;
    }
    return len;
}

size_t RingBuffer_discardBytes(RingBuffer *rb, size_t len) {
    if ((rb == NULL) || (len == 0)) {
        return 0;
    }
    if (len > rb->_len) {
        len = rb->_len;
    }
    size_t left = len;
    if ((rb->_rpos + left) >= rb->_cap) {
        rb->_rpos = 0;
        left -= rb->_cap - rb->_rpos;
    }
    rb->_len -= len;
    if (rb->_len == 0) {
        rb->_wpos = 0;
        rb->_rpos = 0;
    } else {
        rb->_rpos += left;
    }
    return len;
}

size_t RingBuffer_peekBytes(const RingBuffer *rb, void *buf, size_t len) {
    if ((rb == NULL) || (buf == NULL) || (len == 0)) {
        return 0;
    }
    if (len > rb->_len) {
        len = rb->_len;
    }
    uint8_t *tbuf = (uint8_t *)buf;
    size_t left = len;
    size_t rpos = rb->_rpos;
    if ((rpos + left) >= rb->_cap) {
        size_t copy = rb->_cap - rpos;
        memcpy(tbuf, rb->_data + rpos, copy);
        rpos = 0;
        tbuf += copy;
        left -= copy;
    }
    if (left > 0) {
        memcpy(tbuf, rb->_data + rpos, left);
    }
    return len;
}

size_t RingBuffer_peekBytesAt(const RingBuffer *rb, size_t pos, void *buf,
                              size_t len) {
    if ((rb == NULL) || (pos >= rb->_len) || (buf == NULL) || (len == 0)) {
        return 0;
    }
    uint8_t *tbuf = (uint8_t *)buf;
    size_t rcap = rb->_len - pos;
    if (len > rcap) {
        len = rcap;
    }
    size_t left = len;
    size_t rpos = rb->_rpos + pos;
    if (rpos >= rb->_cap) {
        rpos -= rb->_cap;
    }
    if ((rpos + left) >= rb->_cap) {
        size_t copy = rb->_cap - rpos;
        memcpy(tbuf, rb->_data + rpos, copy);
        rpos = 0;
        tbuf += copy;
        left -= copy;
    }
    if (left > 0) {
        memcpy(tbuf, rb->_data + rpos, left);
    }
    return len;
}
