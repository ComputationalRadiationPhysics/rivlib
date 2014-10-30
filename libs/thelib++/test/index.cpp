/*
 * index.cpp
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
#include "stdafx.h"
#include "index.h"
#include "testhelper.h"
#include "the/config.h"
#include "the/exception.h"

#include "testcollection.h"
#include "testdelegate.h"
#include "teststring.h"
#include "testtrace.h"
#include "testpoint.h"
#include "testvector.h"
#include "testmatrix.h"
#include "testquaternion.h"
#include "testpolynom.h"
#include "test_size_and_rect.h"
#include "testtls.h"
#include "testfile.h"
#include "testipaddress.h"
#include "testthread.h"
#include "testperformancecounter.h"
#include "testthreadpool.h"
#include "test_ascii_file_buffer.h"
#include "testipresolution.h"
#include "testipendpoint.h"
#include "testmathfunctions.h"
#include "testsocket.h"
#include "testserver.h"
#include "testdispatcher.h"
#include "testmultisz.h"


/*
 * TestAll
 */
void TestAll(void);


/*
 * TheTestIndex
 */
TheTestIndexEntry TheTestIndex[] = {
    {_T("all"), ::TestAll, _T("Runs all tests"), false},

    {_T("String"), ::TestString, _T("Test the string classes and utility functions"), true},
    {_T("delegate"), ::TestDelegate, _T("Tests the delegate"), true},
    {_T("multicast_delegate"), ::TestMulticastDelegate, _T("Tests the multicast_delegate"), true},
    {_T("array"), ::TestArray, _T("Tests the array collection"), true},
    {_T("fflist"), ::TestFastForwardList, _T("Test the fast_forward_list collection"), true},
    {_T("heap"), ::TestHeap, _T("Tests the heap collection"), true},
    {_T("trace"), ::test_trace, _T("Tests the::trace (not really)"), false},
    {_T("point"), ::TestPoint, _T("Tests the::math::point"), true},
    {_T("vector"), ::Testvector, _T("Tests the::math::vector"), true},
    {_T("matrix"), ::TestMatrix, _T("Tests the::math::matrix"), true},
    {_T("covariancematrix"), ::TestCovarianceMatrix, _T("Tests the::math::matrix for covariance matrices"), true},
    {_T("quaternion"), ::TestQuaternion, _T("Tests the::math::quaternion"), true},
    {_T("polynom"), ::Testpolynom, _T("Tests the::math::polynom"), true},
    {_T("tls"), ::TestTls, _T("Tests the::system::threading::thread_local and thread_local_ptr"), true},
    {_T("extent"), ::test_size, _T("Tests the::math::extent"), true},
    {_T("rectangle"), ::test_rectangle, _T("Tests the::math::rectangle"), true},
    {_T("file"), ::TestFile, _T("Tests the::system::io::file"), true},
    {_T("ipaddress"), ::TestIpAddress, _T("Tests the::system::net::ip_address"), true},
    {_T("ipresolution"), ::TestIpResolution, _T("Tests IP address parsing and DNS resolution"), true},
    {_T("ipendpoint"), ::TestIpEndPoint, _T("Tests the::system::net::ip_end_point"), true},
    {_T("thread"), ::TestThread, _T("Tests the::system::threading::thread"), true},
    {_T("threadpool"), ::TestThreadPool, _T("Tests the::system::threading::thread_pool"), true},
    {_T("performancecounter"), ::TestPerformanceCounter, _T("Tests the::system::performance_counter"), true},
    {_T("ascii_file_buffer"), ::test_ascii_file_buffer, _T("Tests the::system::io::ascii_file_buffer"), true},
    {_T("mathfunctions"), ::TestMathFunctions, _T("Tests functions in the/math/functions.h"), true},
    {_T("socket"), ::TestSocket, _T("Tests the::system::net::socket"), true},
    {_T("server"), ::TestServer, _T("Tests the::system::net::server"), true},
    {_T("dispatcher"), ::TestDispatcher, _T("Tests the::system::net::message_dispatcher"), true},
#ifdef THE_WINDOWS
    {_T("multisz"), ::TestMultiSz, _T("Tests the::multi_sz"), true},
#endif


    // Add your additional tests here!

    // end guard. Do not remove. Must be last entry.
    {NULL, NULL, NULL}
};


/*
 * TestAll
 */
void TestAll(void) {
    for (int i = 0; TheTestIndex[i].testName != NULL; i++) {
        if (TheTestIndex[i].testFunc == ::TestAll) continue;
        if (!TheTestIndex[i].isPartOfAll) continue;

#ifdef THE_WINDOWS
        _ftprintf(stderr, _T("Performing test: %s\n"), TheTestIndex[i].testName);
#else /* THE_WINDOWS */
        fprintf(stderr, "Performing test: %s\n", TheTestIndex[i].testName);
#endif /* THE_WINDOWS */
        try {
            TheTestIndex[i].testFunc();
        } catch (the::exception& e) {
#ifdef THE_WINDOWS
            _tprintf(_T("\nUnexpected the::exception: %s "), e.get_msg());
#else /* THE_WINDOWS */
            printf("\nUnexpected the::exception: %s ", e.get_msg_astr());
#endif /* THE_WINDOWS */
            ::AssertOutputFail(); // add a generic fail
        } catch(std::exception& stlE) {
            printf("\nexception: %s ", stlE.what());
            ::AssertOutputFail(); // add a generic fail
        } catch (...) {
            printf("\nUnexpected exception ");
            ::AssertOutputFail(); // add a generic fail
        }
    }

}
