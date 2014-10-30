/*
 * testhelper.h
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
 * testhelper.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THETEST_TESTHELPER_H_INCLUDED
#define THETEST_TESTHELPER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include <iostream>

#include "the/types.h"
#include "the/math/functions.h"
//#include "vislib/String.h"

/**
 * Tests a condition becoming true
 *
 * @param desc A human-readable description
 * @param cond The condition to be tested
 *
 * @return cond
 */
bool AssertTrue(const char *desc, const bool cond);

/**
 * Tests a condition becoming false
 *
 * @param desc A human-readable description
 * @param cond The condition to be tested
 *
 * @return cond
 */
bool AssertFalse(const char *desc, const bool cond);

template<class T> bool AssertEqual(const char *desc, 
                                   const T& lhs, 
                                   const T& rhs) {
    return ::AssertTrue(desc, (lhs == rhs));
}

template<class T> bool AssertEqualOrShow(const char *desc, 
                                   const T& lhs, 
                                   const T& rhs) {
    if (!AssertEqual(desc, lhs, rhs)) {
        std::cout << "\tlhs = " << lhs << std::endl
            << "\trhs = " << rhs << std::endl << std::endl;
        return false;
    } else {
        return true;
    }
}

//template<class T> bool AssertEqual(const char *desc, 
//                                   const char *lhs, 
//                                   const T& rhs) {
//    return ::AssertTrue(desc, the::StringA(lhs).Equals(the::StringA(rhs)));
//}
//
//template<class T> bool AssertEqual(const char *desc, 
//                                   const wchar_t *lhs, 
//                                   const T& rhs) {
//    return ::AssertTrue(desc, the::StringW(lhs).Equals(the::StringW(rhs)));
//}
//
//template<class T> bool AssertEqualCaseInsensitive(const char *desc, 
//                                   const char *lhs, 
//                                   const T& rhs) {
//    return ::AssertTrue(desc, the::StringA(lhs).Equals(the::StringA(rhs), false));
//}
//
//template<class T> bool AssertEqualCaseInsensitive(const char *desc, 
//                                   const wchar_t *lhs, 
//                                   const T& rhs) {
//    return ::AssertTrue(desc, the::StringW(lhs).Equals(the::StringW(rhs), false));
//}

template<class T> bool AssertNotEqual(const char *desc,
                                      const T& lhs,
                                      const T& rhs) {
    return ::AssertTrue(desc, (lhs != rhs));
}

template<class T> bool AssertNearlyEqual(const char *desc, 
                                         const T& lhs, 
                                         const T& rhs) {
    return ::AssertTrue(desc, the::math::is_equal<T>(lhs, rhs));
}

template<class T> bool AssertNotNearlyEqual(const char *desc,
                                            const T& lhs,
                                            const T& rhs) {
    return ::AssertFalse(desc, the::math::is_equal<T>(lhs, rhs));
}

/**
 * Starts output for a test
 *
 * @param desc A human-readable description
 */
void AssertOutput(const char *desc);

/**
 * Closes output for a test with a success notification
 */
void AssertOutputSuccess(void);

/**
 * Closes output for a test with a failure notification
 */
void AssertOutputFail(void);

/**
 * Outputs a test summary
 */
void OutputAssertTestSummary(void);

/**
 * this succeeds if exactly the specified exception is thrown.
 *
 * has no return value!
 *
 * @param desc A human-readable description
 * @param call The call of the test
 * @param exception The exception to be thrown on success
 */
#define AssertException(desc, call, exception) AssertOutput(desc); try { call; AssertOutputFail(); } catch(exception e) { AssertOutputSuccess(); } catch(...) { AssertOutputFail(); }

/**
 * this succeeds if NO exception is thrown.
 *
 * has no return value!
 *
 * @param desc A human-readable description
 * @param call The call of the test
 */
#define AssertNoException(desc, call) AssertOutput(desc); try { call; AssertOutputSuccess(); } catch(...) { AssertOutputFail(); }

/**
 * Enables or disables the output of success notifications.
 * Used only for internal functions
 *
 * @param isEnabled The new value for the flag
 */
void EnableAssertSuccessOutput(const bool isEnabled);

/**
 * Enables or disables the output of failure notifications.
 * Used only for internal functions
 *
 * @param isEnabled The new value for the flag
 */
void EnableAssertFailureOutput(const bool isEnabled);

#endif /* THETEST_TESTHELPER_H_INCLUDED */
