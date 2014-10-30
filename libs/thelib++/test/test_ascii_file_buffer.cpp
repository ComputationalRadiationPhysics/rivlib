/*
 * test_ascii_file_buffer.cpp
 *
 * Copyright (c) 2013, TheLib Team (http://www.thelib.org/license)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of TheLib, TheLib Team, nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THELIB TEAM AS IS AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THELIB TEAM BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "stdafx.h"
#include "test_ascii_file_buffer.h"
#include "testhelper.h"
//
#include "the/string.h"
#include "the/system/io/ascii_file_buffer.h"


void test_ascii_file_buffer(void) {
    std::string test(
        "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do\n"
        "eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut  \r"
        "enim ad minim veniam, quis nostrud exercitation ullamco laboris\n\r"
        "nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in\r\n"
        "reprehenderit in voluptate velit esse cillum dolore eu fugiat\n\n\r"
        "  nulla pariatur. Excepteur sint occaecat cupidatat non proident,\n"
        "sunt in culpa qui officia deserunt mollit anim id est laborum.");
    using the::system::io::ascii_file_buffer;

    // parsing lines will include leading and trailing spaces
    ascii_file_buffer line_buf(ascii_file_buffer::parsing_elements::lines);
    line_buf.load_buffer(test.c_str(), test.size());

    AssertEqual<size_t>("8 Lines", line_buf.size(), 8);
    AssertEqual<size_t>("Line 1 contains 1 words", line_buf[0].size(), 1);
    AssertEqual<size_t>("Line 2 contains 1 words", line_buf[1].size(), 1);
    AssertEqual<size_t>("Line 3 contains 1 words", line_buf[2].size(), 1);
    AssertEqual<size_t>("Line 4 contains 1 words", line_buf[3].size(), 1);
    AssertEqual<size_t>("Line 5 contains 1 words", line_buf[4].size(), 1);
    AssertEqual<size_t>("Line 6 contains 0 words", line_buf[5].size(), 0);
    AssertEqual<size_t>("Line 7 contains 1 words", line_buf[6].size(), 1);
    AssertEqual<size_t>("Line 8 contains 1 words", line_buf[7].size(), 1);

    AssertTrue("Word 1 is complete line", strcmp(line_buf[0].get(), "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do") == 0);
    AssertTrue("Word 2 is complete line", strcmp(line_buf[1].get(), "eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut  ") == 0);
    AssertTrue("Word 3 is complete line", strcmp(line_buf[2].get(), "enim ad minim veniam, quis nostrud exercitation ullamco laboris") == 0);
    AssertTrue("Word 4 is complete line", strcmp(line_buf[3].get(), "nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in") == 0);
    AssertTrue("Word 5 is complete line", strcmp(line_buf[4].get(), "reprehenderit in voluptate velit esse cillum dolore eu fugiat") == 0);
    AssertException("Word 6 does not exist", line_buf[5].get(), the::exception);
    AssertTrue("Word 7 is complete line", strcmp(line_buf[6].get(), "  nulla pariatur. Excepteur sint occaecat cupidatat non proident,") == 0);
    AssertTrue("Word 8 is complete line", strcmp(line_buf[7].get(), "sunt in culpa qui officia deserunt mollit anim id est laborum.") == 0);

    // parsing words will exclude all spaces
    ascii_file_buffer word_buf(ascii_file_buffer::parsing_elements::words);
    word_buf.load_buffer(test.c_str(), test.size());

    AssertEqual<size_t>("8 Lines", word_buf.size(), 8);
    AssertEqual<size_t>("Line 1 contains 10 words", word_buf[0].size(), 10);
    AssertEqual<size_t>("Line 2 contains 10 words", word_buf[1].size(), 10);
    AssertEqual<size_t>("Line 3 contains 9 words", word_buf[2].size(), 9);
    AssertEqual<size_t>("Line 4 contains 12 words", word_buf[3].size(), 12);
    AssertEqual<size_t>("Line 5 contains 9 words", word_buf[4].size(), 9);
    AssertEqual<size_t>("Line 6 contains 0 words", word_buf[5].size(), 0);
    AssertEqual<size_t>("Line 7 contains 8 words", word_buf[6].size(), 8);
    AssertEqual<size_t>("Line 8 contains 11 words", word_buf[7].size(), 11);
    AssertException("Line 9 does not exist", word_buf[8].get(), the::exception);

    AssertTrue("Word 1.2 is \"ipsum\"", strcmp(word_buf[0][1], "ipsum") == 0);
    AssertTrue("Word 2.1 is \"eiusmod\"", strcmp(word_buf[1][0], "eiusmod") == 0);
    AssertTrue("Word 2.10 is \"eiusmod\"", strcmp(word_buf[1][9], "Ut") == 0);
    AssertTrue("Word 3.9 is \"laboris\"", strcmp(word_buf[2][8], "laboris") == 0);
    AssertException("Word 3.10 does not exist", word_buf[2][9], the::exception);
    AssertTrue("Word 4.7 is \"consequat.\"", strcmp(word_buf[3][6], "consequat.") == 0);
    AssertTrue("Word 5.9 is \"fugiat\"", strcmp(word_buf[4][8], "fugiat") == 0);
    AssertException("Word 6.1 does not exist", word_buf[5][0], the::exception);
    AssertTrue("Word 7.1 is \"nulla\"", strcmp(word_buf[6][0], "nulla") == 0);
    AssertTrue("Word 7.8 is \"proident,\"", strcmp(word_buf[6][7], "proident,") == 0);
    AssertTrue("Word 8.3 is \"culpa\"", strcmp(word_buf[7][2], "culpa") == 0);
    AssertTrue("Word 8.11 is \"laborum.\"", strcmp(word_buf[7][10], "laborum.") == 0);

}
