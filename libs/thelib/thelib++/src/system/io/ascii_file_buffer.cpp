/*
 * the\system\io\ascii_file_buffer.cpp
 *
 * Copyright (C) 2013 TheLib Team (http://www.thelib.org/license)
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

#include "the/system/io/ascii_file_buffer.h"

#include "the/argument_null_exception.h"
#include "the/memory.h"
#include "the/system/io/file.h"
#include "the/math/functions.h"
#include "the/text/char_utility.h"


/*
 * the::system::io::ascii_file_buffer::line_buffer::line_buffer
 */
the::system::io::ascii_file_buffer::line_buffer::line_buffer(void) : cnt(0), words(nullptr) {
    THE_STACK_TRACE;
    // intentionally empty
}


/*
 * the::system::io::ascii_file_buffer::line_buffer::~line_buffer
 */
the::system::io::ascii_file_buffer::line_buffer::~line_buffer(void) {
    THE_STACK_TRACE;
    // do not delete the element pointers
    safe_array_delete(this->words);
}


/*
 * the::system::io::ascii_file_buffer::line_buffer::operator=
 */
the::system::io::ascii_file_buffer::line_buffer&
the::system::io::ascii_file_buffer::line_buffer::operator=(const line_buffer& rhs) {
    THE_STACK_TRACE;
    this->cnt = rhs.cnt;
    safe_array_delete(this->words);
    this->words = new const char*[this->cnt];
    ::memcpy(this->words, rhs.words, sizeof(const char*) * this->cnt);
    return *this;
}


/*
 * the::system::io::ascii_file_buffer::ascii_file_buffer
 */
the::system::io::ascii_file_buffer::ascii_file_buffer(parsing_elements el,
        size_t max_size) : base(), buffer(nullptr), lines(), el(el),
        maxlen(max_size) {
    THE_STACK_TRACE;
    // intentionally empty
}


/*
 * the::system::io::ascii_file_buffer::~ascii_file_buffer
 */
the::system::io::ascii_file_buffer::~ascii_file_buffer(void) {
    THE_STACK_TRACE;
    this->lines.clear();
    safe_array_delete(this->buffer);
}


/*
 * the::system::io::ascii_file_buffer::clear
 */
void the::system::io::ascii_file_buffer::clear(void) {
    THE_STACK_TRACE;
    this->lines.clear();
    safe_array_delete(this->buffer);
}


/*
 * the::system::io::ascii_file_buffer::load
 */
void the::system::io::ascii_file_buffer::load(const char *path) {
    THE_STACK_TRACE;
    file f;

    this->clear();

    f.open(path, file::mode::open, file::access::read, file::share::read);

    if (this->buffer != nullptr) {
        safe_array_delete(this->buffer);
    }

    size_t mem_len = this->maxlen;
    try {
        mem_len = math::minimum<size_t>(mem_len,
            static_cast<size_t>(file::get_size(path)));
    } catch(...) {
    }

    this->buffer = new char[mem_len + 1]; // one extra byte for the last terminating zero

    size_t len = static_cast<size_t>(f.read(this->buffer, mem_len));

    f.close();

    if (len > 0) {
        if (this->el == parsing_elements::lines) {
            this->parse_buffer_lines(len);
        } else {
            this->parse_buffer_words(len);
        }
    }
}


/*
 * the::system::io::ascii_file_buffer::load
 */
void the::system::io::ascii_file_buffer::load(const wchar_t *path) {
    THE_STACK_TRACE;
    file f;

    this->clear();

    f.open(path, file::mode::open, file::access::read, file::share::read);

    if (this->buffer != nullptr) {
        safe_array_delete(this->buffer);
    }

    size_t mem_len = this->maxlen;
    try {
        mem_len = math::minimum<size_t>(mem_len, 
            static_cast<size_t>(file::get_size(path)));
    } catch(...) {
    }

    this->buffer = new char[mem_len + 1]; // one extra byte for the last terminating zero

    size_t len = static_cast<size_t>(f.read(this->buffer, mem_len));

    f.close();

    if (len > 0) {
        if (this->el == parsing_elements::lines) {
            this->parse_buffer_lines(len);
        } else {
            this->parse_buffer_words(len);
        }
    }
}


/*
 * the::system::io::ascii_file_buffer::load_buffer
 */
void the::system::io::ascii_file_buffer::load_buffer(const char *buffer, size_t size) {

    this->clear();
    if (this->buffer != nullptr) {
        safe_array_delete(this->buffer);
    }

    if ((buffer != nullptr) && (size > 0)) {
        this->buffer = new char[size + 1]; // one extra byte for the last terminating zero
        ::memcpy(this->buffer, buffer, size);

        if (this->el == parsing_elements::lines) {
            this->parse_buffer_lines(size);
        } else {
            this->parse_buffer_words(size);
        }
    }
}


/*
 * the::system::io::ascii_file_buffer::parse_buffer_lines
 */
void the::system::io::ascii_file_buffer::parse_buffer_lines(size_t len) {
    THE_STACK_TRACE;
    THE_ASSERT(this->lines.size() == 0);
    THE_ASSERT(this->buffer != nullptr);
    THE_ASSERT(len > 0);
    THE_ASSERT(this->el == parsing_elements::lines);

    size_t pos = 0;
    size_t start = 0;

    this->lines.add(line_buffer());
    line_buffer *line = &this->lines.last();

    while (pos < len) {
        if ((this->buffer[pos] == '\n') || (this->buffer[pos] == '\r')) {
            // this is a new line!
            if (start != pos) {
                // this line is not empty!
                line->cnt = 1;
                line->words = new const char*[1];
                line->words[0] = this->buffer + start;
            } else {
                // this line is empty!
                line->cnt = 0;
                line->words = nullptr; // paranoia
            }
            if ((pos + 1 < len) && ((this->buffer[pos + 1] == '\n') || (this->buffer[pos + 1] == '\r')) && (this->buffer[pos] != this->buffer[pos + 1])) {
                // two char new line command
                this->buffer[pos] = 0; // line terminating zero
                pos++;
            }
            this->buffer[pos] = 0; // line terminating zero
            pos++; // skip the new line

            this->lines.add(line_buffer());
            line = &this->lines.last();
            start = pos;

        } else {
            pos++;
        }
    }

    if (start != pos) {
        // final line exists
        line->cnt = 1;
        line->words = new const char*[1];
        line->words[0] = this->buffer + start;
    } else {
        // final line is empty
        line->cnt = 0;
        line->words = nullptr; // paranoia
    }

    this->buffer[len] = 0; // last terminating zero

}


/*
 * the::system::io::ascii_file_buffer::parse_buffer_words
 */
void the::system::io::ascii_file_buffer::parse_buffer_words(size_t len) {
    THE_STACK_TRACE;
    THE_ASSERT(this->lines.size() == 0);
    THE_ASSERT(this->buffer != nullptr);
    THE_ASSERT(len > 0);
    THE_ASSERT(this->el == parsing_elements::words);

    this->buffer[len] = 0; // last terminating zero

    collections::array<const char*> word_starts;
    size_t pos = 0;

    this->lines.add(line_buffer());
    line_buffer *line = &this->lines.last();
    word_starts.clear();

    while (pos <= len) {
        // search start of word
        while ((this->buffer[pos] != 0)
                && (this->buffer[pos] != '\n')
                && (this->buffer[pos] != '\r')
                && the::text::char_utility::is_space(this->buffer[pos])) {
            pos++;
        }

        if ((this->buffer[pos] == 0) || (this->buffer[pos] == '\n') || (this->buffer[pos] == '\r')) {
            // end of line
            line->cnt = word_starts.size();
            line->words = new const char*[line->cnt];
            for (size_t i = 0; i < line->cnt; i++) {
                line->words[i] = word_starts[i];
            }

            if (this->buffer[pos] == 0) {
                // end of data
                break;
            }

            this->lines.add(line_buffer());
            line = &this->lines.last();
            word_starts.clear();

            if ((pos + 1 < len) && ((this->buffer[pos + 1] == '\n') || (this->buffer[pos + 1] == '\r')) && (this->buffer[pos] != this->buffer[pos + 1])) {
                // two char new line command
                this->buffer[pos] = 0; // line terminating zero
                pos++;
            }
            this->buffer[pos] = 0; // line terminating zero
            pos++;
            continue; // search start for next word
        }

        word_starts.add(this->buffer + pos);

        // search end of word
        while ((this->buffer[pos] != 0)
                && (this->buffer[pos] != '\n')
                && (this->buffer[pos] != '\r')
                && !the::text::char_utility::is_space(this->buffer[pos])) {
            pos++;
        }

        if ((this->buffer[pos] == 0) || (this->buffer[pos] == '\n') || (this->buffer[pos] == '\r')) {
            // end of line
            line->cnt = word_starts.size();
            line->words = new const char*[line->cnt];
            for (size_t i = 0; i < line->cnt; i++) {
                line->words[i] = word_starts[i];
            }

            if (this->buffer[pos] == 0) {
                // end of data
                break;
            }

            this->lines.add(line_buffer());
            line = &this->lines.last();
            word_starts.clear();

            if ((pos + 1 < len) && ((this->buffer[pos + 1] == '\n') || (this->buffer[pos + 1] == '\r')) && (this->buffer[pos] != this->buffer[pos + 1])) {
                // two char new line command
                this->buffer[pos] = 0; // line terminating zero
                pos++;
            }
        }

        this->buffer[pos] = 0; // line terminating zero
        pos++;
    }
}
