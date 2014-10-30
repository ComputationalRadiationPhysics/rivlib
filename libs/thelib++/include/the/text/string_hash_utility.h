/*
 * the/text/string_hash_utility.h
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
#ifndef THE_TEXT_STRING_HASH_UTILITY_H_INCLUDED
#define THE_TEXT_STRING_HASH_UTILITY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/types.h"


namespace the {
namespace text {

    /**
     * Utility class to compute hash values of strings
     *
     * @remark Do not use directly. Use 'StringUtility' instead
     */
    class string_hash_utility {
    public:

        /**
         * Answer a hash code of the string.
         * Uses the DJB2 hash function to calculate the hash code.
         *
         * @param str The string to be hashed
         *
         * @return A hash code of the string.
         */
        template<class T>
        static uint32_t hash_code(const T& str);

        /**
         * Answer a hash code of the string.
         * Uses the DJB2 hash function to calculate the hash code.
         *
         * @param str The string to be hashed
         *
         * @return A hash code of the string.
         */
        template<class T>
        static uint32_t hash_code(const T *str);

    protected:

        /** forbidden Ctor */
        string_hash_utility(void);

        /** forbidden Copy Ctor */
        string_hash_utility(const string_hash_utility& src);

        /** forbidden Dtor */
        ~string_hash_utility(void);

    private:

    };


    /*
     * string_hash_utility::hash_code
     */
    template<class T>
    uint32_t string_hash_utility::hash_code(const T& str) {
        // DJB2 hash function
        uint32_t hash = 0;
        size_t len = str.length();
        for (size_t i = 0; i < len; i++) {
            hash = ((hash << 5) + hash) + static_cast<uint32_t>(str[i]);
        }
        return hash;
    }


    /*
     * string_hash_utility::hash_code
     */
    template<class T>
    uint32_t string_hash_utility::hash_code(const T* str) {
        // DJB2 hash function
        uint32_t hash = 0;
        T c;
        T* i = str;
        while ((c = *i++) != 0) {
            hash = ((hash << 5) + hash) + static_cast<uint32_t>(c);
        }
        return hash;
    }

} /* end namespace text */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TEXT_STRING_HASH_UTILITY_H_INCLUDED */
