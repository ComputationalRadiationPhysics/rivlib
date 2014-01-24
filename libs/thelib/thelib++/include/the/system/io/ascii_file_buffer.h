/*
 * the\system\io\ascii_file_buffer.h
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

#ifndef THE_SYSTEM_IO_ASCII_FILE_BUFFER_H_INCLUDED
#define THE_SYSTEM_IO_ASCII_FILE_BUFFER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/not_copyable.h"
#include "the/stack_trace.h"
#include "the/index_out_of_range_exception.h"
#include "the/collections/array.h"


namespace the {
namespace system {
namespace io {

    /**
     * Buffer class loading a whole ASCII text file into memory and providing
     * a pointer array to access the lines or space-separated words.
     *
     * This class is meant for working with very small ASCII text files only.
     * Especially when you need global information (number of lines) or if you
     * need random access to your information.
     *
     * In all other cases (e.g. larger files) consider using
     * 'text_file_reader'.
     *
     * @remarks: When parsing lines, the line_buffer object will still be used
     *           and each object will hold exactly one 'word' containing the
     *           whole line, including spaces, excluding the new line
     *           characters.
     *           When parsing words, the line_buffer object will hold an array
     *           of words, each excluding any spaces and new line characters.
     *
     * New line commands can be '\n', '\r', '\n\r', or '\r\n'. The new line
     * commands are detected greedily, e.g. The sequence '\n\r\r' will result
     * in two new lines.
     */
    class ascii_file_buffer : public not_copyable {
    public:

        /** possible parsing elements */
        enum class parsing_elements {
            lines,
            words
        };

        /**
         * Holding access pointers to all words of a line
         */
        class line_buffer {
        public:

            /** ctor */
            line_buffer(void);

            /** dtor */
            ~line_buffer(void);

            /**
             * Answers the idx-th word of the line
             *
             * @param idx The zero-based index of the word to return
             */
            inline const char *get(size_t idx = 0) const {
                THE_STACK_TRACE;
                if ((idx < 0) || (idx >= this->cnt)) {
                    throw index_out_of_range_exception(idx, 0, this->cnt - 1,
                        __FILE__, __LINE__);
                }
                return this->words[idx];
            }

            /**
             * Answer the number of words in the line
             *
             * @return The number of words in the line
             */
            inline size_t size(void) const {
                THE_STACK_TRACE;
                return this->cnt;
            }

            /**
             * Assignment operator
             *
             * @param rhs The right hand side operand
             *
             * @return A reference to 'this'
             */
            line_buffer& operator=(const line_buffer& rhs);

            /**
             * Test for equality
             *
             * @param rhs The right hand side operand
             *
             * @return True if this and rhs are equal
             */
            inline bool operator==(const line_buffer& rhs) const {
                THE_STACK_TRACE;
                return this->cnt == rhs.cnt
                    && this->words == rhs.words;
            }

            ///**
            // * Answer the first word
            // *
            // * @return The first word
            // */
            //inline operator const char *(void) const explicit {
            //    THE_STACK_TRACE;
            //    return this->get();
            //}

            /**
             * Answer the idx-th word
             *
             * @param idx the zero-based index
             *
             * @return The idx-th word
             */
            inline const char * operator[](size_t idx) const {
                THE_STACK_TRACE;
                return this->get(idx);
            }

        private:

            /** number of words */
            size_t cnt;

            /** the words of this line */
            const char **words;

            /** friend class for creation */
            friend class ascii_file_buffer;
        };

        /**
         * Initialises a new instance.
         *
         * @param el The parsing element setting
         * @param max_size The maximum buffer size
         */
        ascii_file_buffer(parsing_elements el = parsing_elements::lines,
            size_t max_size = 1024 * 1024);

        /** Dtor. */
        virtual ~ascii_file_buffer(void);

        /** clears the buffer */
        void clear(void);

        /**
         * Returns the set maximum buffer size
         *
         * @return The maximum buffer size
         */
        inline size_t get_max_buffer_size(void) const {
            THE_STACK_TRACE;
            return this->maxlen;
        }

        /**
         * Answer the parsing element which will be parsed when no other
         * element is requested specifically.
         *
         * @return The default parsing element
         */
        inline parsing_elements get_parsing_elements_setting(void) const {
            THE_STACK_TRACE;
            return this->el;
        }

        /**
         * Loads a file into the buffer
         *
         * @param path The path to the file to load
         */
        void load(const char *path);

        /**
         * Loads a file into the buffer
         *
         * @param path The path to the file to load
         */
        void load(const wchar_t *path);

        /**
         * Loads the data from a buffer. The buffer content will be copied
         * into the internal buffer of the object
         *
         * @param buffer The buffer to read from
         * @param size The size of 'buffer' in bytes/characters
         */
        void load_buffer(const char *buffer, size_t size);

        /**
         * Sets the maximum buffer size
         *
         * @param s The new maximum buffer size
         */
        inline void set_max_buffer_size(size_t s) {
            THE_STACK_TRACE;
            this->maxlen = s;
        }

        /**
         * Answer the parsing element which will be parsed when no other
         * element is requested specifically.
         *
         * @return The default parsing element
         */
        inline void set_parsing_elements_setting(parsing_elements el) {
            THE_STACK_TRACE;
            this->el = el;
        }

        /**
         * Gets the number of lines parsed
         *
         * @return The number of lines
         */
        inline size_t size(void) const {
            THE_STACK_TRACE;
            return this->lines.size();
        }

        /**
         * Answer the idx-th line
         *
         * @param idx the zero-based index
         *
         * @return The idx-th line
         */
        inline const line_buffer& operator[](size_t idx) const {
            THE_STACK_TRACE;
            return this->lines[idx];
        }

    protected:

        /** Super class typedef. */
        typedef the::not_copyable base;

        /**
         * Parses the buffer data
         *
         * @param len The length of the buffer data (-1).
         *
         * @remarks 'this->buffer[len]' will be the last terminating zero of
         *          the data.
         */
        void parse_buffer_lines(size_t len);

        /**
         * Parses the buffer data
         *
         * @param len The length of the buffer data (-1).
         *
         * @remarks 'this->buffer[len]' will be the last terminating zero of
         *          the data.
         */
        void parse_buffer_words(size_t len);

        /** The buffer holding the whole file */
        char *buffer;

        /** the line access objects */
        collections::array<line_buffer> lines;

        /** The parsing elements */
        parsing_elements el;

        /** The maximum buffer length */
        size_t maxlen;

    };

} /* end namespace io */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_IO_ASCII_FILE_BUFFER_H_INCLUDED */
