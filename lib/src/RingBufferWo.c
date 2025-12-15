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
 * Implements RingBufferWo and associated functions.
 */

#include "RingBufferWo.h"
#include <string.h>

size_t RingBufferWo_writeBytes(RingBufferWo *rb, const void *buf, size_t len) {
    if ((rb == NULL) || (buf == NULL) || (len == 0)) {
        return 0;
    }
    const uint8_t *tbuf = (uint8_t *)buf;
    size_t left = len;
    size_t skip = (left > rb->_cap) ? (left / rb->_cap - 1) * rb->_cap : 0;
    tbuf += skip;
    left -= skip;
    if ((rb->_wpos + left) >= rb->_cap) {
        size_t copy = rb->_cap - rb->_wpos;
        memcpy(rb->_data + rb->_wpos, tbuf, copy);
        rb->_wpos = 0;
        tbuf += copy;
        left -= copy;
    }
    if (left > 0) {
        memcpy(rb->_data + rb->_wpos, tbuf, left);
        rb->_wpos += left;
    }
    return len;
}
