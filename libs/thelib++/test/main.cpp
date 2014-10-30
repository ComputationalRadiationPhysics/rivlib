/*
 * main.cpp
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
#include "the/config.h"

#if defined(THE_WINDOWS) && (defined(DEBUG) || defined(_DEBUG))
#include <crtdbg.h>
#endif /* THE_WINDOWS && (DEBUG || _DEBUG) */

#include "index.h"
#include "testhelper.h"

#include "the/exception.h"
#include "the/stack_trace.h"
#include "the/trace.h"

#include "the/system/net/debug.h"
#include "the/system/threading/debug.h"


/**
 * Test program main function
 *
 * Note for Debugging in Visual Studio 2010:
 *  Set the Environment variable '_MSVC_STOP_AFTER_DEBUG_' on the property page
 * 'Debugging' of the project to 1 to keep the console window open.
 * i.e. set
 *  'Environment' '_MSVC_STOP_AFTER_DEBUG_=1\n$(LocalDebuggerEnvironment)'
 * The line break needs to be entered in the extra dialog window.
 *
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 *
 * @return 0
 */
#ifdef THE_WINDOWS
int _tmain(int argc, TCHAR **argv) {
#if defined(DEBUG) | defined(_DEBUG)
    ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    SET_THE_NETWORKING_TRACE_ALL();
    //SET_THE_THREADING_TRACE_ALL();
#endif /* defined(DEBUG) | defined(_DEBUG) */
#else /* THE_WINDOWS */
int main(int argc, char **argv) {
#endif /* THE_WINDOWS */
    printf("\n    TheLib Test Application\n    =======================\n\n");

    // check command line arguments
    if (argc <= 1) {
        // no tests specified, so show help text
        fprintf(stderr, "You must specify at least one test to be performed.\n\n");
        fprintf(stderr, "Syntax:\n\t");
#ifdef THE_WINDOWS
            _ftprintf(stderr, _T("%s"), argv[0]);
#else /* THE_WINDOWS */
            fprintf(stderr, "%s", argv[0]);
#endif /* THE_WINDOWS */
        fprintf(stderr, " testname [testname] ...\n\n");
        fprintf(stderr, "Available Testnames are:\n");
        for (unsigned int t = 0; TheTestIndex[t].testName != NULL; t++) {
            fprintf(stderr, "\t");
#ifdef THE_WINDOWS
            _ftprintf(stderr, _T("%s"), TheTestIndex[t].testName);
            _ftprintf(stderr, _T("\n\t\t%s\n"), TheTestIndex[t].testDesc);
#else /* THE_WINDOWS */
            fprintf(stderr, "%s", TheTestIndex[t].testName);
            fprintf(stderr, "\n\t\t%s\n", TheTestIndex[t].testDesc);
#endif /* THE_WINDOWS */
        }
        fprintf(stderr, "\n");

    } else {
        // at least one test specified

        // first check if test(s) exist
        for (int a = 1; a < argc; a++) {
            bool found = false;
            for (unsigned int t = 0; TheTestIndex[t].testName != NULL; t++) {
#ifdef THE_WINDOWS
#pragma warning(disable: 4996)
                if (_tcsicmp(argv[a], TheTestIndex[t].testName) == 0) {
#pragma warning(default: 4996)
#else /* THE_WINDOWS */
                if (strcasecmp(argv[a], TheTestIndex[t].testName) == 0) {
#endif /* THE_WINDOWS */
                    found = true;
                    break;
                }
            }
            if (!found) {
                fprintf(stderr, "Warning: No Test named ");
#ifdef THE_WINDOWS
                _ftprintf(stderr, _T("%s"), argv[a]);
#else /* THE_WINDOWS */
                fprintf(stderr, "%s", argv[a]);
#endif /* THE_WINDOWS */                
                fprintf(stderr, " found. Ignoring this argument.\n\n");
            }
        }

        // now run the tests
        for (int a = 1; a < argc; a++) {
            for (unsigned int t = 0; TheTestIndex[t].testName != NULL; t++) {
#ifdef THE_WINDOWS
#pragma warning(disable: 4996)
                if (_tcsicmp(argv[a], TheTestIndex[t].testName) == 0) {
#pragma warning(default: 4996)
#else /* THE_WINDOWS */
                if (strcasecmp(argv[a], TheTestIndex[t].testName) == 0) {
#endif /* THE_WINDOWS */
                    printf("Performing Test %d: ", a);
#ifdef THE_WINDOWS
                    _tprintf(_T("%s\n"), TheTestIndex[t].testName);
#else /* THE_WINDOWS */
                    printf("%s\n", TheTestIndex[t].testName);
#endif /* THE_WINDOWS */

                    try {
                        TheTestIndex[t].testFunc();
                    } catch (the::exception& e) {
#ifdef THE_WINDOWS
                        _tprintf(_T("\nUnexpected the::exception: %s "), e.get_msg());
#else /* THE_WINDOWS */
                        printf("\nUnexpected the::exception: %s ", e.get_msg_astr());
#endif /* THE_WINDOWS */
                        ::AssertOutputFail(); // add a generic fail
                    } catch (std::exception stlE) {
                        printf("\nexception: %s ", stlE.what());
                        ::AssertOutputFail(); // add a generic fail
                    } catch (...) {
                        printf("\nUnexpected exception ");
                        ::AssertOutputFail(); // add a generic fail
                    }
                    break;
                }
            }
        }

        // Output test statistics
        ::OutputAssertTestSummary();
    }

#if defined(THE_WINDOWS) && defined(_DEBUG) // VC Debugger Halt on Stop Crowbar
#pragma warning(disable: 4996)
    if (getenv("_MSVC_STOP_AFTER_DEBUG_") != NULL) system("pause");
#pragma warning(default: 4996)
#endif
    return 0;
}

