/*
 * version_number.cpp
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

#include "the/version_number.h"
#include "the/stack_trace.h"
#include "the/text/string_builder.h"


/*
 * the::version_number::version_number
 */
the::version_number::version_number(value_type major_number,
        value_type minor_number, value_type build_number,
        value_type revision_number) : major_version(major_number),
        minor_version(minor_number), build_number(build_number),
        revision(revision_number) {
    THE_STACK_TRACE;
    // intentionally empty
}


/*
 * the::version_number::version_number
 */
the::version_number::version_number(const version_number& src)
        : major_version(src.major_version), minor_version(src.minor_version),
        build_number(src.build_number), revision(src.revision)  {
    THE_STACK_TRACE;
    // intentionally empty
}


/*
 * the::version_number::version_number
 */
the::version_number::version_number(const char *ver) : major_version(0),
        minor_version(0), build_number(0), revision(0) {
    THE_STACK_TRACE;
    try {
        this->parse(ver);
    } catch(...) {
        this->set(0);
    }
}


/*
 * the::version_number::version_number
 */
the::version_number::version_number(const wchar_t *ver) : major_version(0),
        minor_version(0), build_number(0), revision(0) {
    THE_STACK_TRACE;
    try {
        this->parse(ver);
    } catch(...) {
        this->set(0);
    }
}


/*
 * the::version_number::version_number
 */
the::version_number::version_number(const the::astring& ver) : major_version(0),
        minor_version(0), build_number(0), revision(0) {
    THE_STACK_TRACE;
    try {
        this->parse(ver);
    } catch(...) {
        this->set(0);
    }
}


/*
 * the::version_number::version_number
 */
the::version_number::version_number(const the::wstring& ver) : major_version(0),
        minor_version(0), build_number(0), revision(0) {
    THE_STACK_TRACE;
    try {
        this->parse(ver);
    } catch(...) {
        this->set(0);
    }
}


/*
 * the::version_number::~version_number
 */
the::version_number::~version_number(void) {
    THE_STACK_TRACE;
    // intentionally empty
}


/*
 * the::version_number::parse
 */
unsigned int the::version_number::parse(const char *ver) {
    THE_STACK_TRACE;
    int retval = 0;
    int value = 0;
    int char_value;

    this->major_version = 0;
    this->minor_version = 0;
    this->build_number = 0;
    this->revision = 0;

    for (int state = 1; state > 0; ver++) {
        char_value = static_cast<int>(*ver) - static_cast<int>('0');
        switch (state) {
            case 1: // first character of a number
                if ((char_value < 0) || (char_value > 9)) {
                    state = 0; // unexpected; expecting a digit. 
                } else {
                    value = char_value;
                    state = 2;
                }
                break;
            case 2: // later character of a number
                if ((char_value < 0) || (char_value > 9)) {
                    switch (retval) {
                        case 0: this->major_version = value; break;
                        case 1: this->minor_version = value; break;
                        case 2: this->build_number = value; break;
                        case 3: this->revision = value; break;
                    }
                    retval++;
                    if (retval > 3) {
                        retval = 4;
                        state = 0; // finished.
                    } else {
                        if ((*ver == '.') || (*ver == ',')) {
                            state = 1; // valid separator
                        } else {
                            state = 0; // end of valid string
                        }
                    }
                } else {
                    value = 10 * value + char_value;
                }
                break;
            default: // something went really wrong!
                state = 0;
                break;
        }
    }

    return retval;
}


/*
 * the::version_number::parse
 */
unsigned int the::version_number::parse(const wchar_t *ver) {
    THE_STACK_TRACE;
    int retval = 0;
    int value = 0;
    int char_value;

    this->major_version = 0;
    this->minor_version = 0;
    this->build_number = 0;
    this->revision = 0;

    for (int state = 1; state > 0; ver++) {
        char_value = static_cast<int>(*ver) - static_cast<int>(L'0');
        switch (state) {
            case 1: // first character of a number
                if ((char_value < 0) || (char_value > 9)) {
                    state = 0; // unexpected; expecting a digit. 
                } else {
                    value = char_value;
                    state = 2;
                }
                break;
            case 2: // later character of a number
                if ((char_value < 0) || (char_value > 9)) {
                    switch (retval) {
                        case 0: this->major_version = value; break;
                        case 1: this->minor_version = value; break;
                        case 2: this->build_number = value; break;
                        case 3: this->revision = value; break;
                    }
                    retval++;
                    if (retval > 3) {
                        retval = 4;
                        state = 0; // finished.
                    } else {
                        if ((*ver == L'.') || (*ver == L',')) {
                            state = 1; // valid separator
                        } else {
                            state = 0; // end of valid string
                        }
                    }
                } else {
                    value = 10 * value + char_value;
                }
                break;
            default: // something went really wrong!
                state = 0;
                break;
        }
    }

    return retval;
}


/*
 * the::version_number::to_astring
 */
the::astring the::version_number::to_astring(unsigned int num) const {
    THE_STACK_TRACE;

    if (num == 0) {
        num = 1;
        if (this->minor_version > 0) num = 2;
        if (this->build_number > 0) num = 3;
        if (this->revision > 0) num = 4;
    }

    const char * fmt = "%u.%u.%u.%u";
    switch (num) {
    case 1: fmt = "%u"; break;
    case 2: fmt = "%u.%u"; break;
    case 3: fmt = "%u.%u.%u"; break;
    }

    return the::text::astring_builder::format(fmt,
        static_cast<unsigned int>(this->major_version),
        static_cast<unsigned int>(this->minor_version),
        static_cast<unsigned int>(this->build_number),
        static_cast<unsigned int>(this->revision));
}


/*
 * the::version_number::to_wstring
 */
the::wstring the::version_number::to_wstring(unsigned int num) const {
    THE_STACK_TRACE;

    if (num == 0) {
        num = 1;
        if (this->minor_version > 0) num = 2;
        if (this->build_number > 0) num = 3;
        if (this->revision > 0) num = 4;
    }

    const wchar_t * fmt = L"%u.%u.%u.%u";
    switch (num) {
    case 1: fmt = L"%u"; break;
    case 2: fmt = L"%u.%u"; break;
    case 3: fmt = L"%u.%u.%u"; break;
    }

    return the::text::wstring_builder::format(fmt,
        static_cast<unsigned int>(this->major_version),
        static_cast<unsigned int>(this->minor_version),
        static_cast<unsigned int>(this->build_number),
        static_cast<unsigned int>(this->revision));
}
