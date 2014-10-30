/*
 * testhelper.cpp
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
 * testhelper.cpp
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#include "stdafx.h"
#include "testhelper.h"
#include <iomanip>
//#include <vislib/Console.h>


/** Flag controlling the output of success notifications */
static bool _assertTrueShowSuccess = true;

/** Flag controlling the output of failure notification */
static bool _assertTrueShowFailure = true;

/** Number of succeeded tests */
static unsigned int testhelp_testSuccess = 0;

/** Number of failed tests */
static unsigned int testhelp_testFail = 0;


/*
 * AssertTrue
 */
bool AssertTrue(const char *desc, const bool cond) {
    if (cond) {
        if (::_assertTrueShowSuccess) {
            AssertOutput(desc);
            AssertOutputSuccess();
        } else {    // CROWBAR
            ::testhelp_testSuccess++;
        }
    } else {
        if (::_assertTrueShowFailure) {
            std::cout << std::endl;
            AssertOutput(desc);
            AssertOutputFail();
            std::cout << std::endl;
        } else {    // CROWBAR
            ::testhelp_testFail++;
        }
    }

    return cond;
}


/*
 * AssertFalse
 */
bool AssertFalse(const char *desc, const bool cond) {
    return ::AssertTrue(desc, !cond);
}


/*
 * AssertOutput
 */
void AssertOutput(const char *desc) {
    using namespace std;
//    using namespace the::sys;

//    Console::SetForegroundColor(Console::DARK_GRAY);
    cout << "[" << setw(4) << setfill('0') 
        << (testhelp_testSuccess + testhelp_testFail + 1) 
        << setw(0) << "] ";
//    Console::RestoreDefaultColors();
    cout << "\"" << desc << "\" ";
}


/*
 * AssertOutputSuccess
 */
void AssertOutputSuccess(void) {
    using namespace std;
//    using namespace the::sys;

    testhelp_testSuccess++;
//    Console::SetForegroundColor(Console::GREEN);
    cout << "succeeded.";
//    Console::RestoreDefaultColors();
    cout << endl;
    cout << flush;
}


/*
 * AssertOutputFail
 */
void AssertOutputFail(void) {
    using namespace std;
//    using namespace the::sys;

    testhelp_testFail++;
//    Console::SetForegroundColor(Console::RED);
    cout << "FAILED.";
//    Console::RestoreDefaultColors();
    cout << endl;
    cout << flush;
}


/*
 * OutputAssertTestSummary
 */
void OutputAssertTestSummary(void) {
    using namespace std;
    cout << endl << "Assert Test Summary:" << endl;
    unsigned int testAll = testhelp_testFail + testhelp_testSuccess;

    if (testAll == 0) {
        cout << "  No Test conducted." << endl << endl;
        return;
    }

    double persFail = double(testhelp_testFail) / double(testAll) * 100.0;
    double persSuccess = double(testhelp_testSuccess) / double(testAll) * 100.0;
    cout << "  " << testAll << " Assert Tests." << endl;

    //if (testAll > 0) {
    //    if (testhelp_testSuccess == testAll) {
    //        the::sys::Console::SetForegroundColor(the::sys::Console::GREEN);
    //    } else {
    //        the::sys::Console::SetForegroundColor(the::sys::Console::GRAY);
    //    }
    //}
    cout << setiosflags(ios::fixed) << setprecision(1);
    cout << "  " << testhelp_testSuccess << " Tests succeeded (" << persSuccess << "%)" << endl;
    //the::sys::Console::RestoreDefaultColors();

    //if ((testAll > 0) && (testhelp_testFail > 0)) {
    //    the::sys::Console::SetForegroundColor(the::sys::Console::RED);
    //}
    cout << "  " << testhelp_testFail << " Tests failed (" << persFail << "%)" << endl << endl;
    cout << resetiosflags(ios::fixed);

    //the::sys::Console::RestoreDefaultColors();
}


/*
 * EnableAssertSuccessOutput
 */
void EnableAssertSuccessOutput(const bool isEnabled) {
    ::_assertTrueShowSuccess = isEnabled;
}


/*
 * EnableAssertFailureOutput
 */
void EnableAssertFailureOutput(const bool isEnabled) {
    ::_assertTrueShowFailure = isEnabled;
}
