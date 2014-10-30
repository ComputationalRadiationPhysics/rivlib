/*
 * include\the\graphics\directx\debug.h
 *
 * Copyright (C) 2012 TheLib Team (http://www.thelib.org/license)
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

#ifndef THE_GRAPHICS_DIRECTX_DEBUG_H_INCLUDED
#define THE_GRAPHICS_DIRECTX_DEBUG_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#ifdef WITH_THE_DIRECTX11
#include <D3D11.h>
#ifdef _MSC_VER
#pragma comment(lib, "dxguid.lib") 
#endif /* _MSC_VER */
#endif /* WITH_THE_DIRECTX11 */

#include "the/assert.h"
#include "the/stack_trace.h"
#include "the/trace.h"

#include "the/system/com_exception.h"
#include "the/system/com_ptr.h"

#include "the/text/string_builder.h"


namespace the {
namespace graphics {
namespace directx {

#ifdef WITH_THE_DIRECTX11
    /**
     * Sets the DirectX debug object name private data on the given object, 
     * which allows for identifying objects that are not released correctly by
     * their name.
     *
     * This function does nothing in release builds. It is only intended for 
     * debugging in actual debug builds.
     *
     * @param object The object to set the debug name for. This must be a 
     *               DirectX object with the SetPrivateData() method.
     * @param fmt    The format string of the name.
     * @param argptr The variable arguments for 'fmt'.
     *
     * @return S_OK in case of success, an error code otherwise. In release
     *         builds, the return value is always S_OK.
     */
    template<class T> inline HRESULT set_debug_object_name(T *object,
            const char *fmt, va_list argptr) {
        THE_STACK_TRACE;
#ifdef THE_DEBUG
        const char *n;
        astring name;

        name = text::astring_builder::formatVa(fmt, argptr);
        n = name.c_str();
        UINT l = name.length();

        return object->SetPrivateData(WKPDID_D3DDebugObjectName, l, n);
#else /* THE_DEBUG */
        return S_OK;
#endif /* THE_DEBUG */
    }

    /**
     * Sets the DirectX debug object name private data on the given object, 
     * which allows for identifying objects that are not released correctly by
     * their name.
     *
     * This function does nothing in release builds. It is only intended for 
     * debugging in actual debug builds.
     *
     * @param object The object to set the debug name for. This must be a 
     *               DirectX object with the SetPrivateData() method.
     * @param fmt    The format string of the name.
     * @param ...    The variable arguments for 'fmt'.
     *
     * @return S_OK in case of success, an error code otherwise. In release
     *         builds, the return value is always S_OK.
     */
    template<class T> inline HRESULT set_debug_object_name(T *object,
            const char *fmt, ...) {
        THE_STACK_TRACE;
        va_list argptr;
        va_start(argptr, fmt);
        HRESULT retval = set_debug_object_name(object, fmt, argptr);
        va_end(argptr);
        return retval;
    }

    /**
     * Sets the DirectX debug object name private data on the given object, 
     * which allows for identifying objects that are not released correctly by
     * their name.
     *
     * This function does nothing in release builds. It is only intended for 
     * debugging in actual debug builds.
     *
     * @param object The object to set the debug name for. This must be a 
     *               DirectX object with the SetPrivateData() method.
     * @param fmt    The format string of the name.
     * @param ...    The variable arguments for 'fmt'.
     *
     * @return S_OK in case of success, an error code otherwise. In release
     *         builds, the return value is always S_OK.
     */
    template<class T> inline HRESULT set_debug_object_name(
            system::com_ptr<T>& object, const char *fmt, ...) {
        THE_STACK_TRACE;
        va_list argptr;
        va_start(argptr, fmt);
        HRESULT retval = set_debug_object_name(static_cast<T *>(object), fmt,
            argptr);
        va_end(argptr);
        return retval;
    }
#endif /* WITH_THE_DIRECTX11 */

} /* end namespace directx */
} /* end namespace graphics */
} /* end namespace the */


/** The tracing level that thelib uses internally for DirectX messages. */
#define THE_TRCCHL_THELIB_DIRECTX (THE_TRCCHL_THELIB_COMMON + 16)


/**
 * Sets the trace level for the channel THE_TRCCHL_THELIB_DIRECTX to the
 * given value.
 *
 * @param lvl The new trace level.
 */
#define SET_THE_DIRECTX_TRACE_LEVEL(lvl)\
    the::trace::get_instance().set_level(THE_TRCCHL_THELIB_THREADING, lvl)


/**
 * Enables all tracing messages for the channel THE_TRCCHL_THELIB_DIRECTX.
 *
 * Furthermore, output on the console and the debug console will be enabled.
 */
#define SET_THE_DIRECTX_TRACE_ALL() SET_THE_THREADING_TRACE_LEVEL(\
    the::trace::LEVEL_ALL);\
    the::trace::get_instance().enable_console_output();\
    the::trace::get_instance().enable_debug_output()


/**
 * Writes a message with the given level to the tracing channel
 * THE_TRCCHL_THELIB_THREADING.
 *
 * @param lvl The level of the message to be traced.
 * @param fmt The format string to trace.
 * @param ... Variable arguments for 'fmt'.
 */
#define THE_DIRECTX_TRACE(lvl, fmt, ...) THE_TRACE(\
    (THE_TRCCHL_THELIB_DIRECTX), lvl, fmt, __VA_ARGS__)


/**
 * Writes a message with trace level THE_TRCLVL_ERROR to the tracing channel
 * THE_TRCCHL_THELIB_DIRECTX
 *
 * @param fmt The format string to trace.
 * @param ... Variable arguments for 'fmt'.
 */
#define THE_DIRECTX_TRACE_ERROR(fmt, ...) THE_DIRECTX_TRACE(\
    THE_TRCLVL_ERROR, fmt, __VA_ARGS__)


/**
 * Writes a message with trace level THE_TRCLVL_INFO to the tracing channel
 * THE_TRCCHL_THELIB_DIRECTX
 *
 * @param fmt The format string to trace.
 * @param ... Variable arguments for 'fmt'.
 */
#define THE_DIRECTX_TRACE_INFO(fmt, ...) THE_DIRECTX_TRACE(\
    THE_TRCLVL_INFO, fmt, __VA_ARGS__)


/**
 * Writes a message with trace level THE_TRCLVL_WARN to the tracing channel
 * THE_TRCCHL_THELIB_DIRECTX
 *
 * @param fmt The format string to trace.
 * @param ... Variable arguments for 'fmt'.
 */
#define THE_DIRECTX_TRACE_WARN(fmt, ...) THE_DIRECTX_TRACE(\
    THE_TRCLVL_WARN, fmt, __VA_ARGS__)


/**
 * Writes the given message 'fmt' to the tracing channel 
 * THE_TRCCHL_THELIB_DIRECTX with THE_TRCLVL_ERROR and throws a
 * the::system::com_exception with the same message.
 *
 * In Release builds, the tracing will be disabled, but the exception will
 * still be thrown.
 *
 * @param hr  The COM error code to be reported in the exception.
 * @param fmt The format string for the tracing message and custom exception
 *            message.
 * @param ... The variable arguments for 'fmt'.
 */
#define THE_DIRECTX_TRACE_AND_THROW(hr, fmt, ...)\
{\
    the::text::astring_builder sb;\
    sb.format(fmt, __VA_ARGS__);\
    THE_DIRECTX_TRACE_ERROR(fmt, __VA_ARGS__);\
    throw the::system::com_exception(hr, sb.to_string().c_str(),\
        __FILE__, __LINE__);\
}


#if (defined(THE_DEBUG) && defined(WITH_THE_DIRECTX11))
/**
 * Sets the debug name of 'obj' and asserts that the operation succeded.
 * 
 * @param object The object to set the debug name for. This must be a 
 *               DirectX object with the SetPrivateData() method.
 * @param fmt    The format string of the name.
 * @param ...    The variable arguments for 'fmt'.
 */
#define THE_DIRECTX_SET_DEBUG_NAME(object, fmt, ...) THE_ASSERT(SUCCEEDED(\
    the::graphics::directx::set_debug_object_name(object, fmt, __VA_ARGS__)))
#else /* (defined(THE_DEBUG) && defined(WITH_THE_DIRECTX11)) */
#define THE_DIRECTX_SET_DEBUG_NAME(obj, fmt, ...)
#endif /* (defined(THE_DEBUG) && defined(WITH_THE_DIRECTX11)) */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_GRAPHICS_DIRECTX_DEBUG_H_INCLUDED */
