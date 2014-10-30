/*
 * the\system\net\debug.h
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

#ifndef THE_SYSTEM_NET_DEBUG_H_INCLUDED
#define THE_SYSTEM_NET_DEBUG_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/assert.h"
#include "the/stack_trace.h"
#include "the/trace.h"


/** The tracing level that thelib uses internally for networking messages. */
#define THE_TRCCHL_THELIB_NETWORKING (THE_TRCCHL_THELIB_COMMON + 2)


/**
 * Sets the trace level for the channel THE_TRCCHL_THELIB_NETWORKING to the
 * given value.
 *
 * @param lvl The new trace level.
 */
#define SET_THE_NETWORKING_TRACE_LEVEL(lvl)\
    the::trace::get_instance().set_level(THE_TRCCHL_THELIB_NETWORKING, lvl)


/**
 * Enables all tracing messages for the channel THE_TRCCHL_THELIB_NETWORKING.
 *
 * Furthermore, output on the console and the debug console will be enabled.
 */
#define SET_THE_NETWORKING_TRACE_ALL() SET_THE_NETWORKING_TRACE_LEVEL(\
    the::trace::LEVEL_ALL);\
    the::trace::get_instance().enable_console_output();\
    the::trace::get_instance().enable_debug_output()


/**
 * Writes a message with the given level to the tracing channel
 * THE_TRCCHL_THELIB_NETWORKING.
 *
 * @param lvl The level of the message to be traced.
 * @param fmt The format string to trace.
 * @param ... Variable arguments for 'fmt'.
 */
#define THE_NETWORKING_TRACE(lvl, fmt, ...) THE_TRACE(\
    (THE_TRCCHL_THELIB_NETWORKING), lvl, fmt, __VA_ARGS__)


/**
 * Writes a message with trace level 2 * THE_TRCLVL_INFO to the tracing channel
 * THE_TRCCHL_THELIB_NETWORKING.
 *
 * @param fmt The format string to trace.
 * @param ... Variable arguments for 'fmt'.
 */
#define THE_NETWORKING_TRACE_DEBUG(fmt, ...) THE_NETWORKING_TRACE(\
    (2 * (THE_TRCLVL_INFO)), fmt, __VA_ARGS__)


/**
 * Writes a message with trace level THE_TRCLVL_ERROR to the tracing channel
 * THE_TRCCHL_THELIB_NETWORKING.
 *
 * @param fmt The format string to trace.
 * @param ... Variable arguments for 'fmt'.
 */
#define THE_NETWORKING_TRACE_ERROR(fmt, ...) THE_NETWORKING_TRACE(\
    THE_TRCLVL_ERROR, fmt, __VA_ARGS__)


/**
 * Writes a message with trace level THE_TRCLVL_INFO to the tracing channel
 * THE_TRCCHL_THELIB_NETWORKING.
 *
 * @param fmt The format string to trace.
 * @param ... Variable arguments for 'fmt'.
 */
#define THE_NETWORKING_TRACE_INFO(fmt, ...) THE_NETWORKING_TRACE(\
    THE_TRCLVL_INFO, fmt, __VA_ARGS__)


/**
 * Writes a message with trace level THE_TRCLVL_WARN to the tracing channel
 * THE_TRCCHL_THELIB_NETWORKING.
 *
 * @param fmt The format string to trace.
 * @param ... Variable arguments for 'fmt'.
 */
#define THE_NETWORKING_TRACE_WARN(fmt, ...) THE_NETWORKING_TRACE(\
    THE_TRCLVL_WARN, fmt, __VA_ARGS__)

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_DEBUG_H_INCLUDED */
