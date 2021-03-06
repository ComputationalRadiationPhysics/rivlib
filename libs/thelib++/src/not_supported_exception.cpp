/*
 * the/not_supported_exception.cpp
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
 * not_supported_exception.cpp
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de). Alle Rechte vorbehalten.
 */


#include "the/not_supported_exception.h"


/*
 * the::not_supported_exception::not_supported_exception
 */
the::not_supported_exception::not_supported_exception(
        const char *funcName, const char *file, const int line) 
        : exception(file, line) {
    exception::format_msg("'%s' is an unsupported operation.", funcName);
}


/*
 * the::not_supported_exception::not_supported_exception
 */
the::not_supported_exception::not_supported_exception(
        const wchar_t *funcName, const char *file, const int line) 
        : exception(file, line) {
    exception::format_msg(L"'%s' is an unsupported operation.", funcName);
}


/*
 * the::not_supported_exception::not_supported_exception
 */
the::not_supported_exception::not_supported_exception(
        const not_supported_exception& rhs) 
        : exception(rhs) {
}


/*
 * the::not_supported_exception::~not_supported_exception
 */
the::not_supported_exception::~not_supported_exception(void) throw() {
}


/*
 * the::not_supported_exception::operator =
 */
the::not_supported_exception& 
        the::not_supported_exception::operator =(
        const not_supported_exception& rhs) {
    exception::operator =(rhs);
    return *this;
}

