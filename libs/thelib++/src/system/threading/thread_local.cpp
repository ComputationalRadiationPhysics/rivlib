/*
 * the/system/threading/thread_local.cpp
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

#include "the/system/threading/thread_local.h"

//#include <winnt.h>
//
//
////http://www.codeproject.com/Articles/8113/Thread-Local-Storage-The-C-Way
//
//#if defined(THE_WINDOWS32)
//#pragma comment(linker, "/INCLUDE:_tls_used")
//#elif defined(THE_WINDOWS64)
//#pragma comment(linker, "/INCLUDE:__tls_used")
//#endif /* defined(THE_WINDOWS32) */

//#ifdef THE_WINDOWS
//void NTAPI the_tls_native_callback(HMODULE hModule, DWORD reason, PVOID val) {
//
//}
//
//#if (defined(_MSC_VER) && (_MSC_VER >= 1310))
//#pragma data_seg(push, old_seg)
//#endif /* (defined(_MSC_VER) && (_MSC_VER >= 1310)) */
//
//#pragma data_seg(".CRT$XLB")
//PIMAGE_TLS_CALLBACK p_thread_callback = ::the_tls_native_callback;
//#pragma data_seg()
//
//#if (defined(_MSC_VER) && (_MSC_VER >= 1310))
//#pragma data_seg(pop, old_seg)
//#endif /* (defined(_MSC_VER) && (_MSC_VER >= 1310)) */
//
//#endif /* THE_WINDOWS */
//
//
//#ifdef THE_LINUX
////void the_tls_native_destructor(void *val);
//#endif /* THE_LINUX */
