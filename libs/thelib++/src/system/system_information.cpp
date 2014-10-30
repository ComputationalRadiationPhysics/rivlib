/*
 * src\system\system_information.cpp
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
 * SystemInformation.cpp
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */


#include "the/system/system_information.h"

#include <climits>

#ifdef THE_WINDOWS
#include <windows.h>
#include <Lmcons.h>
#endif /* THE_WINDOWS */

#ifdef THE_LINUX
#include <cstring>
#include <cstdio>
#include <pwd.h>
#include <unistd.h>

#include <sys/utsname.h>
#include <sys/sysinfo.h>
#endif /* THE_LINUX */

#include "the/assert.h"
#include "the/memory.h"
#include "the/stack_trace.h"

#include "the/system/system_exception.h"

#include "the/text/string_converter.h"


/*
 * the::system::system_information::allocation_granularity
 */
the::system::system_information::memory_size_type
the::system::system_information::allocation_granularity(void) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    SYSTEM_INFO si;
    ::GetSystemInfo(&si);
    return si.dwAllocationGranularity;

#elif (THE_LINUX)
    return system_information::page_size();

#endif /* defined(THE_WINDOWS) */
}


/*
 * the::system::system_information::available_memory
 */
the::system::system_information::memory_size_type
the::system::system_information::available_memory(void) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    MEMORYSTATUSEX memoryStatus;

    if (::GlobalMemoryStatusEx(&memoryStatus) == FALSE) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

    return static_cast<memory_size_type>(memoryStatus.ullAvailPhys);

#elif (THE_LINUX)
    struct sysinfo info;

    if (sysinfo(&info) != 0) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

    if (sizeof(info._f) != (sizeof(char) * (20 - 2 * sizeof(long)
            - sizeof(int)))) {
        /* a fucking old kernel is used */
        return static_cast<memory_size_type>(info.freeram);
    }
    return static_cast<memory_size_type>(info.freeram) 
        * static_cast<memory_size_type>(info.mem_unit);

#endif /* defined(THE_WINDOWS) */
}


/*
 * the::system::system_information::computer_name
 */
the::astring& the::system::system_information::computer_name(
        the::astring& outName) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    unsigned long oldBufSize = MAX_COMPUTERNAME_LENGTH; // used for paranoia test
    unsigned long bufSize = MAX_COMPUTERNAME_LENGTH;
    char *buf = new char[bufSize];

    bufSize++;
    while (!::GetComputerNameA(buf, &bufSize)) {
        the::safe_array_delete(buf);

        unsigned int le = ::GetLastError();
        bufSize--;

        if ((le == ERROR_BUFFER_OVERFLOW) && (oldBufSize != bufSize)) {
            oldBufSize = bufSize;
            buf = new char[bufSize];

        } else {
            THROW_THE_SYSTEM_EXCEPTION;

        }
        bufSize++;
    }

    outName = buf;
    the::safe_array_delete(buf);

#elif (THE_LINUX)
    struct utsname names;
    if (uname(&names) != 0) {
        THROW_THE_SYSTEM_EXCEPTION;
    }
    outName = names.nodename;

#endif /* defined(THE_WINDOWS) */

    return outName;
}


/*
 * the::system::system_information::computer_name
 */
the::wstring& the::system::system_information::computer_name(
        the::wstring& outName) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    unsigned long oldBufSize = MAX_COMPUTERNAME_LENGTH; // used for paranoia test
    unsigned long bufSize = MAX_COMPUTERNAME_LENGTH;
    wchar_t *buf = new wchar_t[bufSize];

    bufSize++;
    while (!::GetComputerNameW(buf, &bufSize)) {
        the::safe_array_delete(buf);

        unsigned int le = ::GetLastError();
        bufSize--;

        if ((le == ERROR_BUFFER_OVERFLOW) && (oldBufSize != bufSize)) {
            oldBufSize = bufSize;
            buf = new wchar_t[bufSize];

        } else {
            THROW_THE_SYSTEM_EXCEPTION;

        }
        bufSize++;
    }

    outName = buf;
    the::safe_array_delete(buf);

#elif (THE_LINUX)
    the::astring tmp;
    system_information::computer_name(tmp);
    the::text::string_converter::convert(outName, tmp);

#endif /* defined(THE_WINDOWS) */

    return outName;
}


///* 
// * vislib::sys::SystemInformation::DisplayDeviceCount
// */
//DWORD vislib::sys::SystemInformation::DisplayDeviceCount(void) {
//#ifdef _WIN32
//    DISPLAY_DEVICE dpyDev;
//    dpyDev.cb = sizeof(DISPLAY_DEVICE);
//    DWORD retval = 0;
//
//    while (::EnumDisplayDevices(NULL, retval, &dpyDev, 0)) {
//        retval++;
//    }
//
//    return retval; 
//
//#else /* _WIN32 */
//    return 1;
//#endif /* _WIN32 */
//}

/*
 * the::system::system_information::executable_word_size
 */
the::system::system_information::memory_size_type
the::system::system_information::executable_word_size(void) {
    THE_STACK_TRACE;
#ifdef _WIN32
#ifdef _WIN64
    return 64;
#else
    return 32;
#endif /* _WIN64 */
#else /* _WIN 32 */
#if ((defined(__LP64__) || defined(_LP64) || defined(__x86_64__)) \
    && ((__LP64__ != 0) || (_LP64 != 0) || (__x86_64__ != 0)))
    return 64;
#else 
    return 32;
#endif
#endif /* _WIN32 */
}


///*
// * vislib::sys::SystemInformation::MonitorRects
// */
//DWORD vislib::sys::SystemInformation::MonitorRects(
//        MonitorRectArray& outMonitorRects) {
//    outMonitorRects.Clear();
//
//#ifdef _WIN32
//    if (!::EnumDisplayMonitors(NULL, NULL, SystemInformation::monitorEnumProc,
//            reinterpret_cast<LPARAM>(&outMonitorRects))) {
//        throw SystemException(__FILE__, __LINE__);
//    }
//
//#else /* _WIN32 */
//    int cntScreens = 0;         // # of attached screens.
//    Display *dpy = NULL;        // The display.
//    StringA errorDesc;          // For formatting an error message.
//    
//    if ((dpy = ::XOpenDisplay(NULL)) == NULL) {
//        errorDesc.Format("Could not open display \"%s\".", 
//            ::XDisplayName(NULL));
//        throw Exception(errorDesc, __FILE__, __LINE__);
//    }
//
//    cntScreens = ScreenCount(dpy);
//    for (int i = 0; i < cntScreens; i++) {
//        outMonitorRects.Append(SystemInformation::getRootWndRect(dpy, i));
//    }
//    
//    ::XCloseDisplay(dpy);
//
//#endif /* _WIN32 */
//
//    return static_cast<DWORD>(outMonitorRects.Count());
//}


/*
 * the::system::system_information::page_size
 */
the::system::system_information::memory_size_type
the::system::system_information::page_size(void) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    SYSTEM_INFO si;
    ::GetSystemInfo(&si);
    return si.dwPageSize;

#elif (THE_LINUX)
    int retval = ::sysconf(_SC_PAGESIZE);

    if (retval == -1) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

    return static_cast<memory_size_type>(retval);

#endif /* defined(THE_WINDOWS) */
}


/*
 * the::system::system_information::physical_memory
 */
the::system::system_information::memory_size_type
the::system::system_information::physical_memory(void) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    MEMORYSTATUSEX memoryStatus;

    if (::GlobalMemoryStatusEx(&memoryStatus) == FALSE) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

    return static_cast<memory_size_type>(memoryStatus.ullTotalPhys);

#elif (THE_LINUX)
    struct sysinfo info;

    if (sysinfo(&info) != 0) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

    if (sizeof(info._f) != (sizeof(char) * (20 - 2 * sizeof(long)
            - sizeof(int)))) {
        /* a fucking old kernel is used */
        return static_cast<memory_size_type>(info.totalram);
    }
    return static_cast<memory_size_type>(info.totalram) 
        * static_cast<memory_size_type>(info.mem_unit);

#endif /* defined(THE_WINDOWS) */
}


///*
// * vislib::sys::SystemInformation::PrimaryMonitorRect
// */
//vislib::sys::SystemInformation::MonitorRect 
//vislib::sys::SystemInformation::PrimaryMonitorRect(void) {
//    MonitorRect retval;
//
//#ifdef _WIN32
//    if (!::EnumDisplayMonitors(NULL, NULL, 
//            SystemInformation::findPrimaryMonitorProc,
//            reinterpret_cast<LPARAM>(&retval))) {
//        throw SystemException(__FILE__, __LINE__);
//    }
//
//    if (retval.IsEmpty()) {
//        /* Enumeration was not successful in finding primary display. */
//        throw SystemException(ERROR_NOT_FOUND, __FILE__, __LINE__);
//    }
//
//#else /* _WIN32 */
//    Display *dpy = NULL;
//    StringA errorDesc;
//
//    if ((dpy = ::XOpenDisplay(NULL)) == NULL) {
//        errorDesc.Format("Could not open display \"%s\".", 
//            ::XDisplayName(NULL));
//        throw Exception(errorDesc, __FILE__, __LINE__);
//    }
//
//    retval = SystemInformation::getRootWndRect(dpy, DefaultScreen(dpy));
//    ::XCloseDisplay(dpy);
//#endif /* _WIN32 */
//
//    return retval;
//}



/*
 * the::system::system_information::processors
 */
int the::system::system_information::processors(void) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    SYSTEM_INFO si;
    ::GetSystemInfo(&si);
    return static_cast<int>(si.dwNumberOfProcessors);

#elif (THE_LINUX)
#if defined(_SC_NPROCESSORS_ONLN)
    int retval = ::sysconf(_SC_NPROCESSORS_ONLN);

    if (retval == -1) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

    return static_cast<int>(retval);

#else /* defined(_SC_NPROCESSORS_ONLN) */
    // TODO: Rewrite as soon as better support classes are available
    FILE *cpuinfo = fopen("/proc/cpuinfo", "rt");
    if (cpuinfo) {
        unsigned int countCPUs = 0;
        const unsigned int lineSize = 1024;
        char line[lineSize + 1];
        line[lineSize] = 0;

        while (!feof(cpuinfo)) {
            fgets(line, lineSize, cpuinfo);

            // case sensitive check necessary
            // see /proc/cpuinfo on afro or mmoovis?? for more information
            if (strstr(line, "processor") != NULL) { 
                countCPUs++;
            }
        }

        fclose(cpuinfo);
        return countCPUs;
    }

    // errno is set by failed fopen
    THROW_THE_SYSTEM_EXCEPTION;
#endif
#endif /* defined(THE_WINDOWS) */
}


///*
// * vislib::sys::SystemInformation::SelfSystemType
// */
//vislib::sys::SystemInformation::OSType vislib::sys::SystemInformation::SelfSystemType(void) {
//#ifdef _WIN32
//    return OSTYPE_WINDOWS;
//#else
//    return OSTYPE_LINUX;
//#endif
//}
//

//
//
///*
// * vislib::sys::SystemInformation::SystemType
// */
//vislib::sys::SystemInformation::OSType vislib::sys::SystemInformation::SystemType(void) {
//    /* I'm currently very sure that the system type can be determined by the application type */
//#ifdef _WIN32
//    return OSTYPE_WINDOWS;
//#else
//    return OSTYPE_LINUX;
//#endif
//}


///*
// * vislib::sys::SystemInformation::SystemVersion
// */
//void vislib::sys::SystemInformation::SystemVersion(DWORD& outMajor, 
//                                                   DWORD& outMinor) {
//#ifdef _WIN32
//    OSVERSIONINFO ver;
//    ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
//    
//    if (::GetVersionEx(&ver) != TRUE) {
//        throw SystemException(__FILE__, __LINE__);
//    }
//
//    outMajor = ver.dwMajorVersion;
//    outMinor = ver.dwMinorVersion;
//
//#else /* _WIN32 */
//    const int BUFFER_SIZE = 512;
//    char buffer[BUFFER_SIZE];
//    int majorVersion = 0;
//    int minorVersion = 0;
//    size_t cnt = 0;
//    FILE *fp = NULL;
//
//    // TODO: Use some shell abstraction class instead of popen.
//    if ((fp = ::popen("uname -r", "r")) == NULL) {
//        throw SystemException(__FILE__, __LINE__);
//    }
//
//    cnt = ::fread(buffer, 1, sizeof(buffer) - 1, fp);
//    ::pclose(fp);
//
//    if (cnt == 0)  {
//        throw SystemException(__FILE__, __LINE__);
//    }
//
//    if (::sscanf(buffer, "%d.%d", &majorVersion, &minorVersion) != 2) {
//        VLTRACE(Trace::LEVEL_ERROR, "sscanf on version string failed.");
//        throw SystemException(ENOTSUP, __FILE__, __LINE__);
//    }
//
//    outMajor = majorVersion;
//    outMinor = minorVersion;
//#endif /* _WIN32 */
//}


/*
 * the::system::system_information::system_word_size
 */
the::system::system_information::memory_size_type
the::system::system_information::system_word_size(void) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    SYSTEM_INFO si;
    ::GetNativeSystemInfo(&si);

    switch (si.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_INTEL:
            return 32;

        case PROCESSOR_ARCHITECTURE_IA64:
            /* Falls through. */
        case PROCESSOR_ARCHITECTURE_AMD64:
            return 64;

        case PROCESSOR_ARCHITECTURE_UNKNOWN:
            /* Falls through. */
        default:
            return system_information::executable_word_size();
    }

    THE_ASSERT(false);

#elif (THE_LINUX)
    struct utsname names;

    if (uname(&names) != 0) {
        return system_information::executable_word_size();
    }

    return (::strstr(names.machine, "64") == NULL) ? 32 : 64;

#endif /* defined(THE_WINDOWS) */
}


/*
 * the::system::system_information::user_name
 */
the::astring& the::system::system_information::user_name(
        the::astring& outName) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    unsigned long oldBufSize = UNLEN; // used for paranoia test
    unsigned long bufSize = UNLEN;
    char *buf = new char[bufSize];

    bufSize++;
    while (!::GetUserNameA(buf, &bufSize)) {
        the::safe_array_delete(buf);

        unsigned int le = ::GetLastError();
        bufSize--;

        if ((le == ERROR_INSUFFICIENT_BUFFER) && (oldBufSize != bufSize)) {
            oldBufSize = bufSize;
            buf = new char[bufSize];

        } else {
            THROW_THE_SYSTEM_EXCEPTION;

        }
        bufSize++;
    }

    outName = buf;
    the::safe_array_delete(buf);

#elif (THE_LINUX)
    /* I hate linux because it's completely impossible to write backward-compatible code */
    uid_t uid = geteuid();

    struct passwd *passwd = getpwuid(uid);
    if (passwd == NULL) {
        THROW_THE_EXCEPTION(system_exception, ENOENT);
    }
    outName = passwd->pw_name;

#endif /* defined(THE_WINDOWS) */

    return outName;
}


/*
 * the::system::system_information::user_name
 */
the::wstring& the::system::system_information::user_name(
        the::wstring& outName) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    unsigned long oldBufSize = UNLEN; // used for paranoia test
    unsigned long bufSize = UNLEN;
    wchar_t *buf = new wchar_t[bufSize];

    bufSize++;
    while (!::GetUserNameW(buf, &bufSize)) {
        the::safe_array_delete(buf);

        unsigned int le = ::GetLastError();
        bufSize--;

        if ((le == ERROR_INSUFFICIENT_BUFFER) && (oldBufSize != bufSize)) {
            oldBufSize = bufSize;
            buf = new wchar_t[bufSize];

        } else {
            THROW_THE_SYSTEM_EXCEPTION;

        }
        bufSize++;
    }

    outName = buf;
    the::safe_array_delete(buf);

#elif (THE_LINUX)
    the::astring tmp;
    system_information::computer_name(tmp);
    the::text::string_converter::convert(outName, tmp);

#endif /* defined(THE_WINDOWS) */

    return outName;
}


///*
// * vislib::sys::SystemInformation::VirtualScreen
// */
//vislib::sys::SystemInformation::MonitorRect 
//vislib::sys::SystemInformation::VirtualScreen(void) {
//    MonitorRect retval(LONG_MAX, LONG_MAX, LONG_MIN, LONG_MIN);
//
//#ifdef _WIN32
//    if (!::EnumDisplayMonitors(NULL, NULL, 
//            SystemInformation::calcVirtualScreenProc, 
//            reinterpret_cast<LPARAM>(&retval))) {
//        throw SystemException(__FILE__, __LINE__);
//    }
//
//#else /* _WIN32 */
//    MonitorRectArray monitors;
//    SystemInformation::MonitorRects(monitors);
//
//    for (SIZE_T i = 0; i < monitors.Count(); i++) {
//        const MonitorRect& monitor = monitors[i];
//        if (monitor.Left() < retval.Left()) {
//            retval.SetLeft(monitor.Left());
//        }
//        if (monitor.Bottom() < retval.Bottom()) {
//            retval.SetBottom(monitor.Bottom());
//        }
//        if (monitor.Right() > retval.Right()) {
//            retval.SetRight(monitor.Right());
//        }
//        if (monitor.Top() > retval.Top()) {
//            retval.SetTop(monitor.Top());
//        }
//    }
//#endif /* _WIN32 */
//
//    return retval;
//}



/*
 * the::system::system_information::~system_information
 */
the::system::system_information::~system_information(void) {
    THE_STACK_TRACE;
}


//#ifdef _WIN32
///*
// * vislib::sys::SystemInformation::calcVirtualScreenProc
// */
//BOOL CALLBACK vislib::sys::SystemInformation::calcVirtualScreenProc(
//        HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
//    MonitorRect *vs = reinterpret_cast<MonitorRect *>(dwData);    
//
//    if (lprcMonitor->left < vs->Left()) {
//        vs->SetLeft(lprcMonitor->left);
//    }
//    if (lprcMonitor->bottom < vs->Bottom()) {
//        vs->SetBottom(lprcMonitor->bottom);
//    }
//    if (lprcMonitor->right > vs->Right()) {
//        vs->SetRight(lprcMonitor->right);
//    }
//    if (lprcMonitor->top > vs->Top()) {
//        vs->SetTop(lprcMonitor->top);
//    }
//
//    return TRUE;
//}
//#endif /* _WIN32 */
//
//
//#ifndef _WIN32
///*
// * vislib:sys::SystemInformation::getRootWndRect
// */
//vislib::sys::SystemInformation::MonitorRect 
//vislib::sys::SystemInformation::getRootWndRect(Display *dpy, int screen) {
//    XWindowAttributes attribs;  // Attributes of screen root window.
//    Window wnd;                 // The per-screen root window.
//    
//    wnd = RootWindow(dpy, screen);
//    ::XGetWindowAttributes(dpy, wnd, &attribs);
//    // TODO: Error handling.
//
//    return MonitorRect(attribs.x, attribs.y, attribs.width, attribs.height);
//}
//#endif /* !_WIN32 */
//
//
//#ifdef _WIN32
///*
// * vislib::sys::SystemInformation::monitorEnumProc
// */
//BOOL CALLBACK vislib::sys::SystemInformation::monitorEnumProc(HMONITOR hMonitor,
//        HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
//    ASSERT(hdcMonitor == NULL);
//    MonitorRectArray *da = reinterpret_cast<MonitorRectArray *>(dwData);
//    
//    da->Append(MonitorRect(lprcMonitor->left, lprcMonitor->bottom,
//        lprcMonitor->right, lprcMonitor->top));
//
//    return TRUE;
//}
//
//
///*
// * vislib::sys::SystemInformation::findPrimaryMonitorProc
// */
//BOOL CALLBACK vislib::sys::SystemInformation::findPrimaryMonitorProc(
//        HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
//    MONITORINFO mi;
//    MonitorRect *ma = reinterpret_cast<MonitorRect *>(dwData);
//
//    ::ZeroMemory(&mi, sizeof(MONITORINFO));
//    mi.cbSize = sizeof(MONITORINFO);
//    
//    if (::GetMonitorInfo(hMonitor, &mi) != FALSE) {
//        if ((mi.dwFlags & MONITORINFOF_PRIMARY) != 0) {
//            ma->Set(lprcMonitor->left, lprcMonitor->bottom, lprcMonitor->right, 
//                lprcMonitor->top);
//            //return FALSE;
//            // Stopping the enumeration by returning FALSE does not work at
//            // least on Vista.
//        }
//    } else {
//        throw SystemException(__FILE__, __LINE__);
//    }
// 
//    return TRUE;
//}
//#endif /*_ WIN32 */
