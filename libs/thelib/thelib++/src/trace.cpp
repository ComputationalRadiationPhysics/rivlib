/*
 * the/trace.cpp
 *
 * Copyright (c) 2012, TheLib Team (http://www.thelib.org/license)
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
#include "the/trace.h"
#if !defined(THELIB_SYMBOL_IMPORT)
#include <climits>
#include <ctime>
#include "the/assert.h"
#include "the/string.h"


/*
 * the::trace::CHANNEL_DEFAULT
 */
const unsigned int the::trace::CHANNEL_DEFAULT = 0;


/*
 * the::trace::CHANNEL_THELIB_COMMON
 */
const unsigned int the::trace::CHANNEL_THELIB_COMMON = 1;


/*
 * the::trace::CHANNEL_THELIB_FIRST
 */
const unsigned int the::trace::CHANNEL_THELIB_FIRST = 1;


/*
 * the::trace::CHANNEL_THELIB_LAST
 */
const unsigned int the::trace::CHANNEL_THELIB_LAST = 1;


/*
 * the::trace::CHANNEL_USER_FIRST
 */
const unsigned int the::trace::CHANNEL_USER_FIRST
    = the::trace::CHANNEL_THELIB_LAST + 1;


/*
 * the::trace::DEFAULT_PREFIX
 */
const char *the::trace::DEFAULT_PREFIX = "TRACE: ";


/*
 * the::trace::DEFAULT_LEVEL
 */
const unsigned int the::trace::DEFAULT_LEVEL = the::trace::LEVEL_ERROR;


/*
 * the::trace::get_instance
 */
the::trace& the::trace::get_instance(void) {
    static the::trace inst;
    return inst;
}


/*
 * the::trace::LEVEL_ALL
 */
const unsigned int the::trace::LEVEL_ALL = UINT_MAX;


/*
 * the::trace::LEVEL_ERROR
 */
const unsigned int the::trace::LEVEL_ERROR = 1;


/*
 * the::trace::LEVEL_INFO
 */
const unsigned int the::trace::LEVEL_INFO = 200;


/*
 * the::trace::LEVEL_NONE
 */
const unsigned int the::trace::LEVEL_NONE = 0;


/*
 * the::trace::LEVEL_WARN
 */
const unsigned int the::trace::LEVEL_WARN = 100;


/*
 * the::trace::enable_console_output
 */
bool the::trace::enable_console_output(bool enable) {
    this->use_console_out = enable;
    return true;
}


/*
 * the::trace::enable_debug_output
 */
bool the::trace::enable_debug_output(bool enable) {
    this->use_debugger_out = enable;
    return true;
}


/*
 * the::trace::enable_file_output
 */
bool the::trace::enable_file_output(const char *filename) {
    if (this->file_out != NULL) {
        ::fclose(this->file_out);
        this->file_out = NULL;
    }

    if (filename != NULL) {
#ifdef _WIN32
#pragma warning(disable: 4996)
#endif /* _WIN32 */
        this->file_out = ::fopen(filename, "at");

        if (this->file_out != NULL) {
            time_t now;
            ::time(&now);
            ::fprintf(this->file_out, "Trace file opened at %s",
                ::asctime(::localtime(&now)));
        }

#ifdef _WIN32
#pragma warning(default: 4996)
#endif /* _WIN32 */

        return (this->file_out != NULL);
    } else {
        return true;
    }

}


/*
 * the::trace::enable_file_output
 */
bool the::trace::enable_file_output(const wchar_t *filename) {
    if (this->file_out != NULL) {
        ::fclose(this->file_out);
        this->file_out = NULL;
    }

#if THE_WINDOWS

    if (filename != NULL) {
#ifdef _WIN32
#pragma warning(disable: 4996)
#endif /* _WIN32 */
        this->file_out = ::_wfopen(filename, L"at");

        if (this->file_out != NULL) {
            time_t now;
            ::time(&now);
            ::fprintf(this->file_out, "Trace file opened at %s",
                ::asctime(::localtime(&now)));
        }

#ifdef _WIN32
#pragma warning(default: 4996)
#endif /* _WIN32 */

        return (this->file_out != NULL);
    } else {
        return true;
    }

#else /* THE_WINDOWS */

    if (filename != NULL) {
        the::astring fn;
        the::text::string_converter::convert(fn, filename);
        return this->enable_file_output(fn.c_str());
    }

#endif /* THE_WINDOWS */

}


/*
 * the::trace::get_default_level
 */
unsigned int the::trace::get_default_level(void) const {
    return this->default_level;
}


/*
 * the::trace::get_level
 */
unsigned int the::trace::get_level(unsigned int channel) const {
    std::map<unsigned int, unsigned int>::const_iterator iter = this->levels.find(channel);
    return (iter != this->levels.end())
        ? (iter->second)
        : this->default_level;
}


/*
 * the::trace::is_level_set
 */
bool the::trace::is_level_set(unsigned int channel) const {
    return (this->levels.find(channel) != this->levels.end());
}


/*
 * the::trace::reset_all
 */
void the::trace::reset_all(void) {
    this->levels.clear();
}


/*
 * the::trace::reset_level
 */
void the::trace::reset_level(unsigned int channel) {
    this->levels.erase(channel);
}


/*
 * the::trace::set_default_level
 */
void the::trace::set_default_level(unsigned int level) {
    this->default_level = level;
}


/*
 * the::trace::set_level
 */
void the::trace::set_level(unsigned int channel, unsigned int level) {
    this->levels[channel] = level;
}


/*
 * the::trace::set_prefix
 */
void the::trace::set_prefix(const char *prefix) {
    the::safe_array_delete(this->prefix);

    if (prefix != NULL) {
        size_t len = ::strlen(prefix) + 1;
        char *buf = new char[len];
        ::memcpy(buf, prefix, len * sizeof(char));
        this->prefix = buf;
    }
}


/*
 * the::trace::operator ()
 */
void the::trace::operator ()(const unsigned int channel, const unsigned int level,
        const char *fmt, ...) throw() {

    if ((level == 0) || (fmt == NULL)
        || ((!this->use_console_out)
            && (!this->use_debugger_out)
            && (this->file_out == NULL))) return;


    std::map<unsigned int, unsigned int>::const_iterator iter = this->levels.find(channel);
    if (iter != this->levels.end()) {
        if (level > iter->second) return;
    } else {
        if (level > this->default_level) return;
    }

    if (this->use_console_out) {
        va_list list;
        va_start(list, fmt);
        ::fprintf(stderr, "%s", this->prefix);
        ::vfprintf(stderr, fmt, list);
        ::fflush(stderr);
        va_end(list);
    }

    if (this->use_debugger_out) {
#if THE_WINDOWS
        va_list list;
        va_start(list, fmt);

        try {
            int cnt = ::_vscprintf(fmt, list) + 1;
            va_end(list);
            va_start(list, fmt);
            char *tmp = new char[cnt];

#if (_MSC_VER >= 1400)
            ::_vsnprintf_s(tmp, cnt, cnt, fmt, list);
#else /* (_MSC_VER >= 1400) */
            ::vsnprintf(tmp, cnt, fmt, list);
#endif /* (_MSC_VER >= 1400) */
                
            ::OutputDebugStringA(tmp);
            safe_array_delete(tmp);
        } catch (std::bad_alloc) {
            ::fprintf(stderr, "OutputDebugStringA failed because of "
                "insufficient system memory\n");
            ::fflush(stderr);
        }

        va_end(list);
#endif /* THE_WINDOWS */
    }

    if (this->file_out != NULL) {
        va_list list;
        va_start(list, fmt);
        ::vfprintf(this->file_out, fmt, list);
        ::fflush(this->file_out);
        va_end(list);
    }
}


/*
 * the::trace::trace
 */
the::trace::trace(void) : default_level(trace::DEFAULT_LEVEL), file_out(NULL),
        levels(), prefix(NULL), use_debugger_out(true),
        use_console_out(true) {
    ASSERT(trace::CHANNEL_USER_FIRST == trace::CHANNEL_THELIB_LAST + 1);
    ASSERT(trace::DEFAULT_LEVEL == trace::LEVEL_ERROR);
    this->set_prefix(trace::DEFAULT_PREFIX);
}


/*
 * the::trace::~trace
 */
the::trace::~trace(void) {
    this->set_prefix(NULL);
    this->disable_file_output();
}

#endif /* !defined(THELIB_SYMBOL_IMPORT) */
