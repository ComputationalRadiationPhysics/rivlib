/*
 * the/trace.h
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
/*
 * Trace.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de). Alle Rechte vorbehalten.
 */
#ifndef THE_TRACE_H_INCLUDED
#define THE_TRACE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include <cstdarg>
#include <cstdio>
#include <map>
#include "the/config.h"
#include "the/types.h"


namespace the {

    /**
     * This is a utility for tracing debugging output.
     *
     * Trace message are organized in channels and are using levels.
     *
     * The channels are used to separate sources/purposes for the messages,
     * e.g. separating internal TheLib messages from user application messages.
     * User application channels should use values from 'CHANNEL_USER_FIRST'
     * and above.
     *
     * Levels are used to define the severeness of the message. The smaller
     * the value, the more important the message is.
     * A value of 0 disables the message. This is used to disable channels.
     * A value of 1 denotes error.
     * Values between 2 and 100 denote warnings.
     * Values between 101 and 200 denote information messages.
     * Values above 200 denote less important information messages.
     *
     * For each channel the messages can be filtered according to their level.
     *
     * @remarks 'trace' is not thread safe, meaning message from different
     *  threads will all be printed, but might mix up.
     *  The 'trace' output is not locked for performance reasons.
     *
     * @remarks
     *  IMPLEMENTATION NOTE: Do not use 'THE_STACK_TRACE' in any member
     *  function of 'trace' as this might generate cyclic function calls
     */
    class THELIB_SYMBOLEXPORT trace {
    public:

        /**
         * Channel number of the generic default channel.
         * The value is 0
         */
        static const unsigned int CHANNEL_DEFAULT;

        /**
         * Channel number of common TheLib trace channel.
         * The value is 1
         */
        static const unsigned int CHANNEL_THELIB_COMMON;

        /**
         * Channel number of the first internal TheLib trace channel.
         * The value is 1
         */
        static const unsigned int CHANNEL_THELIB_FIRST;

        /**
         * Channel number of the last internal TheLib trace channel.
         * The value is 1
         */
        static const unsigned int CHANNEL_THELIB_LAST;

        /**
         * Channel number for the first user application trace channel.
         * The value is 'CHANNEL_THE_LIB_LAST + 1'
         */
        static const unsigned int CHANNEL_USER_FIRST;

        /**
         * The default prefix string for console output of trace messages
         */
        static const char *DEFAULT_PREFIX;

        /**
         * The default level used to initialize channels when they are used
         * for the first time. The default value is 'LEVEL_ERROR'
         */
        static const unsigned int DEFAULT_LEVEL;

        /**
         * Answer the only instance of this class
         *
         * @return The only instance of this class
         */
        static trace& get_instance(void);

        /** 
         * Set this level to display all tracing information. If you use this
         * constant for tracing itself, the messages will only be output, if
         * LEVEL_ALL is also set as current tracing level.
         */
        static const unsigned int LEVEL_ALL;

        /**
         * Use this for logging errors. The value of this constant is 1, i. e.
         * messages with LEVEL_ERROR will always be printed, if any logging is
         * enabled.
         */
        static const unsigned int LEVEL_ERROR;

        /**
         * Use this for informative messages. The value of this constant 
         * is 200. 
         */
        static const unsigned int LEVEL_INFO;

        /** 
         * Use this for disabling tracing. The value is 0. It cannot be used
         * for tracing itself, but only for the current tracing level.
         */
        static const unsigned int LEVEL_NONE;

        /**
         * Use this for warning messages. The value of this constant 
         * is 100. 
         */
        static const unsigned int LEVEL_WARN;

        /**
         * Disables the output of tracer messages to the standard console
         * streams (stderr).
         *
         * This output is enabled by default.
         *
         * @return True if the output was successfully disabled
         */
        bool disable_console_output(void) {
            return this->enable_console_output(false);
        }

        /**
         * Disables the output of tracer messages to the application or 
         * system debugger, i. e. to the Output window of Visual Studio.
         *
         * This output is enabled by default.
         *
         * @return True if the output was successfully disabled
         */
        bool disable_debug_output(void) {
            return this->enable_debug_output(false);
        }

        /**
         * Disables the output of tracer messages to a file.
         *
         * This output is disabled by default.
         *
         * @return True if the output was successfully disabled
         */
        bool disable_file_output(void) {
            return this->enable_file_output(static_cast<const char*>(NULL));
        }

        /**
         * Enables the output of the tracer messages to the standard console
         * streams (stderr).
         *
         * This output is enabled by default.
         * 
         * @param enable true for enabling the console output, false for
         *                    disabling it.
         *
         * @return true, if the console output was successfully enabled or
         *         disabled, false otherwise.
         */
        bool enable_console_output(bool enable = true);

        /**
         * Enables the output of the tracer messages to the application or 
         * system debugger, i. e. to the Output window of Visual Studio.
         *
         * This output is enabled by default.
         *
         * This setting has currently no effect on non-Windows systems.
         *
         * @param enable true for enabling the debugger output, false for
         *                    disabling it.
         *
         * @return true, if the debugger output was successfully enabled or
         *         disabled, false otherwise.
         */
        bool enable_debug_output(bool enable = true);

        /**
         * Enables the output of the tracer messages to the file with the 
         * specified name. Trace messages will be appended if the file already
         * exists.
         *
         * This output is disabled by default.
         *
         * @param filename The name of the file. If NULL, file output is 
         *                 disabled.
         *
         * @return true, if the log file was successfully opened, false 
         *         otherwise. If ('filename' == NULL), return true always.
         */
        bool enable_file_output(const char *filename);

        /**
         * Enables the output of the tracer messages to the file with the 
         * specified name. Trace messages will be appended if the file already
         * exists.
         *
         * This output is disabled by default.
         *
         * @param filename The name of the file. If NULL, file output is 
         *                 disabled.
         *
         * @return true, if the log file was successfully opened, false 
         *         otherwise. If ('filename' == NULL), return true always.
         */
        bool enable_file_output(const wchar_t *filename);

        /**
         * Answer the default level
         *
         * @return The default level
         */
        unsigned int get_default_level(void) const;

        /**
         * Gets the level set for a specific channel. If no level is set for
         * this channel expliclity, the default level is returned.
         *
         * @param channel The channel to query
         *
         * @return The set level
         */
        unsigned int get_level(unsigned int channel) const;

        /**
         * Answer whether or not a level is set for this channel
         *
         * @param channel The channel to query
         *
         * @return true if a level is set of this channel
         */
        bool is_level_set(unsigned int channel) const;

        /**
         * Resets the level information of all channels. The channels will be 
         * initialized to the default level the next time they are used.
         */
        void reset_all(void);

        /**
         * Resets the level information for the specified channel. The channel
         * will be initialized to the default level the next time it is used.
         *
         * @param channel The channel to be reset
         */
        void reset_level(unsigned int channel);

        /**
         * Sets the default level to be used to initialize channels which are
         * used for the first time
         *
         * @param level The new value for the default level
         */
        void set_default_level(unsigned int level);

        /**
         * Sets the level for a specific channel
         *
         * @param channel The channel to set the level for
         * @param level   The new value for the level
         */
        void set_level(unsigned int channel, unsigned int level);

        /**
         * Set the trace prefix for console output. If NULL, no prefix will be 
         * added. The default prefix is "TRACE: ".
         *
         * @param prefix The new prefix or NULL for disabling prefixing.
         */
        void set_prefix(const char *prefix);

        /**
         * Trace the message 'fmt', if an appropriate tracing level was set for
         * the specified channel
         *
         * @param channel The trace channel
         * @param level   The trace level for the message
         * @param fmt     The format string for the trace message
         */
        void operator ()(const unsigned int channel, const unsigned int level,
            const char *fmt, ...) throw();

    private:

        /** forbidden Ctor */
        trace(void);

        /** forbidden Dtor */
        ~trace(void);

        /**
         * The default level used for channels which were not initialized
         * explicitly
         */
        unsigned int default_level;

        /** The file handle for trace files */
        FILE *file_out;

#ifdef THE_WINDOWS
#pragma warning(disable: 4251)
#endif /* THE_WINDOWS */
        /** The levels set for the different channels */
        std::map<unsigned int, unsigned int> levels;
#ifdef THE_WINDOWS
#pragma warning(default: 4251)
#endif /* THE_WINDOWS */

        /** The trace message prefix */
        const char *prefix;

        /** Flag to enable debug output */
        bool use_debugger_out;

        /** Flag to enable console stream output */
        bool use_console_out;

    };

} /* end namespace the */


#if defined THE_DEBUG

/**
 * The trace macro uses 3+ parameters:
 *
 * @param channel The trace channel
 * @param level   The trace level for the message
 * @param fmt     The format string for the trace message
 * @param ...     Additional arguments to be interpreted according to 'fmt'
 */
#define THE_TRACE the::trace::get_instance()

#else /* THE_DEBUG */

/**
 * The trace macro uses 3+ parameters:
 *
 * @param channel The trace channel
 * @param level   The trace level for the message
 * @param fmt     The format string for the trace message (omitted, because macro-ellipsis must not be empty)
 * @param ...     Additional arguments to be interpreted according to 'fmt'
 */
#define THE_TRACE(ch, lvl, ...)

#endif /* THE_DEBUG */

/** Short names for trace channels */
#define THE_TRCCHL_DEFAULT       (the::trace::CHANNEL_DEFAULT)
#define THE_TRCCHL_THELIB_FIRST  (the::trace::CHANNEL_THELIB_FIRST)
#define THE_TRCCHL_THELIB_COMMON (the::trace::CHANNEL_THELIB_COMMON)
#define THE_TRCCHL_THELIB_LAST   (the::trace::CHANNEL_THELIB_LAST)
#define THE_TRCCHL_USER_FIRST    (the::trace::CHANNEL_USER_FIRST)

/** Short names for trace levels */
#define THE_TRCLVL_ERROR (the::trace::LEVEL_ERROR)
#define THE_TRCLVL_INFO  (the::trace::LEVEL_INFO)
#define THE_TRCLVL_WARN  (the::trace::LEVEL_WARN)

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TRACE_H_INCLUDED */
