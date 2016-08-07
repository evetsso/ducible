/*
 * Copyright (c) 2016 Jason White
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

#include <stdint.h>
#include <stdio.h> // For FILE*
#include <vector>

/**
 * Helper function for computing the number of pages required to hold a length
 * of bytes.
 */
template<typename T>
inline T pageCount(T pageSize, T length) {
    return (length + pageSize - 1) / pageSize;
}

/**
 * Represents an MSF stream.
 *
 * An MSF stream is made up of 1 or more pages. This class abstracts away the
 * task of reading from a stream to make it seem as if the data is sequential.
 */
class MsfStream {
private:

    size_t _pageSize;
    size_t _length;
    std::vector<uint32_t> _pages;

public:
    /**
     * Params:
     *   pageSize = Length of one page, in bytes.
     *   length   = Length of the stream, in bytes.
     *   pages    = List of pages. The length of this array is calculated using
     *              the page size and stream length.
     */
    MsfStream(size_t pageSize, size_t length, const uint32_t* pages);

    /**
     * Returns the length of the stream, in bytes.
     */
    size_t length() const;

    /**
     * Returns the page size, in bytes.
     */
    size_t pageSize() const;

    /**
     * Returns the number of pages in the stream.
     */
    size_t pageCount() const;

    /**
     * Reads a single page from the stream.
     *
     * Params:
     *   f    = The file to read from. The seek location is not guaranteed to be
     *          the same after this function is finished.
     *   page = The page number to read from.
     *   length = The number of bytes to read from the page.
     *   buf  = The buffer to read the page into.
     */
    void readFromPage(FILE* f, size_t page, size_t length, void* buf, size_t offset = 0) const;

    /**
     * Reads a length of the stream. This abstracts reading from multiple pages.
     *
     * Params:
     *   f      = The file to read from.
     *   length = The number of bytes to read from the stream.
     *   buf    = The buffer to read the stream into.
     *   offset = The offset to start reading from. Optional.
     */
    void read(FILE* f, size_t length, void* buf, size_t pos = 0) const;

    /**
     * Reads the entire stream.
     *
     * Params:
     *   f      = The file to read from.
     *   buf    = The buffer to read the stream into. This must be large enough
     *            to hold the entire stream.
     *   offset = The offset to start reading from. Optional.
     */
    void read(FILE* f, void* buf, size_t pos = 0) const;
};
