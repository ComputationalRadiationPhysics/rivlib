/*
 * include\the\system\system_information.h
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
/*
 * SystemInformation.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_SYSTEM_INFORMATION_H_INCLUDED
#define THE_SYSTEM_SYSTEM_INFORMATION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/not_instantiable.h"
#include "the/string.h"


namespace the {
namespace system {

    /**
     * Utility class for informations about the local system.
     */
    class system_information : public the::not_instantiable {

    public:

        /** The type used to specify memory sizes. */
#ifdef THE_WINDOWS
        typedef DWORDLONG memory_size_type;
#else /* THE_WINDOWS */
        typedef size_t memory_size_type;
#endif /* THE_WINDOWS */

        /**
         * Answer the the granularity of page protection and commitment.
         *
         * Until we know better on Linux this is equivalent to page_size().
         *
         * @return The allocation granularity in bytes.
         *
         * @throws the::system::system_exception If the page size could not
         *                                       be retrieved (Linux only).
         */
        static memory_size_type allocation_granularity(void);

        /** 
         * Return the size of available physical memory in bytes, not including
         * virtual memory from swap files.
         *
         * @return The size of available memory.
         *
         * @throws the::system::system_exception On failure.
         */
        static memory_size_type available_memory(void);

        /**
         * Returns an ANSI string with the local computers name.
         *
         * @param outName The ANSI string with the local computers name. The 
         *                previous content of the string might be destroied, 
         *                even if the function fails.
         *
         * @return outName.
         *
         * @throws the::system::system_exception On failure.
         */
        static the::astring& computer_name(the::astring& outName);

        /**
         * Returns an Unicode string with the local computers name.
         *
         * @param outName The Unicode string with the local computers name. The
         *                previous content of the string might be destroied, 
         *                even if the function fails.
         *
         * @return outName.
         *
         * @throws the::system::system_exception On failure.
         */
        static the::wstring& computer_name(the::wstring& outName);

        /**
         * Returns the size of a word in bits of the current executable.
         * 
         * The return value is a constant value depending on the compile 
         * targets used when building THElib.
         *
         * @return The word size of the current vislib application.
         */
        static memory_size_type executable_word_size(void);

        ///**
        // * Answer the size and location in the virtual desktop of all monitors 
        // * attached to the system.
        // *
        // * @param outMonitorRects An array receiving the monitor sizes.
        // *
        // * @return The number of entries in 'outMonitorRects'.
        // *
        // * @throws SystemException If a system call required for retrieving
        // *                         the information failed.
        // * @throws Exception       On Linux, if the X11 display could not be
        // *                         opened.
        // */
        //static DWORD MonitorRects(MonitorRectArray& outMonitorRects);

        /**
         * Answer the page size of the system.
         *
         * @return The page size in bytes.
         *
         * @throws the::system::system_exception If the page size could not
         *                                       be retrieved (Linux only).
         */
        static memory_size_type page_size(void);

        /**
         * Return the size of the total physical memory in bytes.
         *
         * @return The size of physical memory.
         *
         * @throws the::system::system_exception On failure.
         */
        static memory_size_type physical_memory(void);

        ///**
        // * Answer the size and origin of the primary monitor. 
        // *
        // * @return The dimension of the primary monitor.
        // *
        // * @throws SystemException If a system call required for retrieving
        // *                         the information failed or no monitor was
        // *                         found.
        // * @throws Exception       On Linux, if the X11 display could not be
        // *                         opened.
        // */
        //static MonitorRect PrimaryMonitorRect(void);

        /**
         * Return the number of processors in the local machine.
         *
         * @return The number of processors.
         *
         * @throws the::system::system_exception On failure.
         */
        static int processors(void);

        ///**
        // * Returns the type of the operating system this vislib application is
        // * built for. Will usually the same value as the return value of
        // * "GetSystemType".
        // *
        // * @return The system type of the current vislib application.
        // */
        //static OSType SelfSystemType(void);

        ///**
        // * Returns the endianness of the system running this vislib application.
        // *
        // * @return The endianness of the machine.
        // */
        //inline static Endianness SystemEndianness(void) {
        //    UINT32 endianTestInt = 0x12345678;
        //    UINT8 endianTestBytes[4];
        //    ::memcpy(endianTestBytes, &endianTestInt, 4);
        //    bool machineBigEndian = ((endianTestBytes[0] == 0x12)
        //        && (endianTestBytes[1] == 0x34)
        //        && (endianTestBytes[2] == 0x56)
        //        && (endianTestBytes[3] == 0x78));
        //    bool machineMiddleEndian = ((endianTestBytes[0] == 0x34)
        //        && (endianTestBytes[1] == 0x12)
        //        && (endianTestBytes[2] == 0x78)
        //        && (endianTestBytes[3] == 0x56));
        //    bool machineLittleEndian = ((endianTestBytes[0] == 0x78)
        //        && (endianTestBytes[1] == 0x56)
        //        && (endianTestBytes[2] == 0x34)
        //        && (endianTestBytes[3] == 0x12));
        //    if (machineBigEndian) {
        //        return ENDIANNESS_BIG_ENDIAN;
        //    } else if (machineLittleEndian) {
        //        return ENDIANNESS_LITTLE_ENDIAN;
        //    } else if (machineMiddleEndian) {
        //        return ENDIANNESS_MIDDLE_ENDIAN;
        //    } else {
        //        return ENDIANNESS_UNKNOWN;
        //    }
        //}

        ///**
        // * Returns the type of the operating system currently running this 
        // * vislib application.
        // *
        // * @return The type of the operating system.
        // */
        //static OSType SystemType(void);
    
        ///**
        // * Answer the version of the operating system.
        // *
        // * @param outMajor Receives the major version.
        // * @param outMinor Receives the minor version.
        // *
        // * @throws SystemException If the version could not be retrieved.
        // */
        //static void SystemVersion(DWORD& outMajor, DWORD& outMinor);

        /**
         * Returns the size of a word in bits of current operating system.
         *
         * This value may differ from the word size of the current application,
         * since 64 bit operating systems are able to run 32 bit applications.
         *
         * @return The word size of the operating system.
         */
        static memory_size_type system_word_size(void);

        /**
         * Returns an ansi string with the local user name running this vislib
         * application.
         *
         * @param outName The ansi string with the local user name. The 
         *                previous content of the string might be destroied, 
         *                even if the function fails.
         *
         * @throws the::system::system_exception On failure.
         */
        static the::astring& user_name(the::astring& outName);

        /**
         * Returns an unicode string with the local user name running this
         * vislib application
         *
         * @param outName The unicode string with the local user name. The
         *                previous content of the string might be destroied, 
         *                even if the function fails.
         *
         * @throws the::system::system_exception On failure.
         */
        static the::wstring& user_name(the::wstring& outName);

        ///**
        // * Computes the extents of the whole virtual screen formed by all
        // * monitors attached to the system. 
        // *
        // * Please note that the screen must not fill the whole rectangle 
        // * returned, but there might be holes in the screen. The returned
        // * rectangle is the bounding rectangle of the virtual screen.
        // *
        // * @return The bounding rectangle of the virtual screen.
        // */
        //static MonitorRect VirtualScreen(void);

        /** Dtor. */
        virtual ~system_information(void);

    protected:

        /** Super class typedef. */
        typedef the::not_instantiable base;

//
//#ifdef _WIN32
//        /** 
//         * Callback method for computing the total virtual screen size on
//         * Windows.
//         *
//         * @param hMonitor    Handle to display monitor.
//         * @param hdcMonitor  Handle to monitor DC.
//         * @param lprcMonitor Monitor intersection rectangle.
//         * @param dwData      Pointer to a MonitorRect to store the dimension 
//         *                    to.
//         *
//         * @return TRUE if the enumeration should be continued, FALSE otherwise.
//         */
//        static BOOL CALLBACK calcVirtualScreenProc(HMONITOR hMonitor, 
//            HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
//#endif /* _WIN32 */
//
//#ifndef _WIN32
//        /**
//         * Open the root window of the specified X11 screen and answer its 
//         * location and dimension.
//         *
//         * @param dpy    The display.
//         * @param screen The screen number.
//         *
//         * @return The window rectangle.
//         */
//        static MonitorRect getRootWndRect(Display *dpy, int screen);
//#endif /* !_WIN32 */
//
//#ifdef _WIN32
//        /** 
//         * Callback method for enumerating the available monitors on Windows.
//         *
//         * @param hMonitor    Handle to display monitor.
//         * @param hdcMonitor  Handle to monitor DC.
//         * @param lprcMonitor Monitor intersection rectangle.
//         * @param dwData      Pointer to the MonitorRectArray to fill.
//         *
//         * @return TRUE if the enumeration should be continued, FALSE otherwise.
//         */
//        static BOOL CALLBACK monitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, 
//            LPRECT lprcMonitor, LPARAM dwData);
//
//        /** 
//         * Callback method for finding the primary monitor on windows.
//         *
//         * @param hMonitor    Handle to display monitor.
//         * @param hdcMonitor  Handle to monitor DC.
//         * @param lprcMonitor Monitor intersection rectangle.
//         * @param dwData      Pointer to a MonitorRect to store the dimension 
//         *                    to. Nothing will be written, if the primary 
//         *                    monitor could not be found.
//         *
//         * @return TRUE if the enumeration should be continued, FALSE otherwise.
//         *
//         * @throws SystemException If it was not possible to determine whether 
//         *                         'hMonitor' designates the primary monitor.
//         */
//        static BOOL CALLBACK findPrimaryMonitorProc(HMONITOR hMonitor, 
//            HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
//#endif /* _WIN32 */

    };

} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_SYSTEM_INFORMATION_H_INCLUDED */
