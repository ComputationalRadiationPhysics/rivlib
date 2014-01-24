/*
 * the/text/string_levenshtein_distance.h
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
 * String.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 - 2009 by Christoph Mueller. All rights reserved.
 */
#ifndef THE_TEXT_STRINGLEVENSHTEINDISTANCE_H_INCLUDED
#define THE_TEXT_STRINGLEVENSHTEINDISTANCE_H_INCLUDED
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
     * Utility class to compute the Levenshtein distance between strings
     * http://www.keldysh.ru/departments/dpt_10/lev.html
     *
     * @remark Be aware that the result may be erronous on Linux with UTF-8
     *         locale when using astring because of the mbs-type of the string
     *         encoding.
     *
     * @remark Do not use directly. Use 'StringUtility' instead
     */
    class string_levenshtein_distance {
    public:

        /**
         * Compute the Levenshtein 
         * (http://www.keldysh.ru/departments/dpt_10/lev.html) distance
         * between this string and 'rhs'. Note that this string is considered
         * the expected text and 'rhs' the actually found one.
         *
         * This operation has a memory consumption of 
         * (this->Length() + 1) * (ths.Length() + 1) * sizeof(Size).
         *
         * @param lhs        The first string to be compared
         * @param rhs        The second string to be compared.
         * @param costAdd    The cost for an insertion (default 1).
         * @param costDelete The cost for a deletion (default 1).
         * @param costChange The cost for a substitution (default 1).
         *
         * @return The Levenshtein distance using the given weights.
         *
         * @throws std::bad_alloc if the temporary buffer for computing the 
         *                        result cannot be allocated.
         */
        template <class T>
        static size_t levenshtein_distance(const T *lhs, const T *rhs,
            const size_t costAdd = 1, const size_t costDelete = 1,
            const size_t costChange = 1);

        /**
         * Compute the Levenshtein 
         * (http://www.keldysh.ru/departments/dpt_10/lev.html) distance
         * between this string and 'rhs'. Note that this string is considered
         * the expected text and 'rhs' the actually found one.
         *
         * This operation has a memory consumption of 
         * (this->Length() + 1) * (ths.Length() + 1) * sizeof(Size).
         *
         * @param lhs        The first string to be compared
         * @param rhs        The second string to be compared.
         * @param costAdd    The cost for an insertion (default 1).
         * @param costDelete The cost for a deletion (default 1).
         * @param costChange The cost for a substitution (default 1).
         *
         * @return The Levenshtein distance using the given weights.
         *
         * @throws std::bad_alloc if the temporary buffer for computing the 
         *                        result cannot be allocated.
         */
        template<class T>
        static inline size_t levenshtein_distance(const T& lhs, const T& rhs, 
                const size_t costAdd = 1, const size_t costDelete = 1, 
                const size_t costChange = 1) {
            return levenshtein_distance(lhs.c_str(), rhs.c_str(), costAdd, 
                costDelete, costChange);
        }

        /**
         * Compute the Levenshtein 
         * (http://www.keldysh.ru/departments/dpt_10/lev.html) distance
         * between this string and 'rhs'. Note that this string is considered
         * the expected text and 'rhs' the actually found one.
         *
         * This operation has a memory consumption of 
         * (this->Length() + 1) * (ths.Length() + 1) * sizeof(Size).
         *
         * @param lhs        The first string to be compared
         * @param rhs        The second string to be compared.
         * @param costAdd    The cost for an insertion (default 1).
         * @param costDelete The cost for a deletion (default 1).
         * @param costChange The cost for a substitution (default 1).
         *
         * @return The Levenshtein distance using the given weights.
         *
         * @throws std::bad_alloc if the temporary buffer for computing the 
         *                        result cannot be allocated.
         */
        template<class T1, class T2>
        static inline size_t levenshtein_distance(const T1 *lhs, const T2& rhs,
                const size_t costAdd = 1, const size_t costDelete = 1,
                const size_t costChange = 1) {
            return levenshtein_distance(lhs, rhs.c_str(), costAdd,
                costDelete, costChange);
        }

        /**
         * Compute the Levenshtein 
         * (http://www.keldysh.ru/departments/dpt_10/lev.html) distance
         * between this string and 'rhs'. Note that this string is considered
         * the expected text and 'rhs' the actually found one.
         *
         * This operation has a memory consumption of 
         * (this->Length() + 1) * (ths.Length() + 1) * sizeof(Size).
         *
         * @param lhs        The first string to be compared
         * @param rhs        The second string to be compared.
         * @param costAdd    The cost for an insertion (default 1).
         * @param costDelete The cost for a deletion (default 1).
         * @param costChange The cost for a substitution (default 1).
         *
         * @return The Levenshtein distance using the given weights.
         *
         * @throws std::bad_alloc if the temporary buffer for computing the 
         *                        result cannot be allocated.
         */
        template<class T1, class T2>
        static inline size_t levenshtein_distance(const T1& lhs, const T2 *rhs,
                const size_t costAdd = 1, const size_t costDelete = 1,
                const size_t costChange = 1) {
            return levenshtein_distance(lhs.c_str(), rhs, costAdd,
                costDelete, costChange);
        }

    protected:

        /** forbidden Ctor */
        string_levenshtein_distance(void);

        /** forbidden Copy Ctor */
        string_levenshtein_distance(const string_levenshtein_distance& src);

        /** forbidden Dtor */
        ~string_levenshtein_distance(void);

    private:

    };


    /*
     * string_levenshtein_distance::LevenshteinDistance
     */
    template <class T>
    size_t string_levenshtein_distance::levenshtein_distance(const T *lhs,
            const T *rhs, const size_t costAdd, const size_t costDelete,
            const size_t costChange) {
        // mueller: Adapted from wbcore. I am not completely sure whether this
        // works ...
#define VL_LVS_ARY_IDX(i, j) ((i) * (len2 + 1) + (j))
#define VL_LVS_MIN3(a, b, c) (((a) < (b))\
    ? (((c) < (a)) ? (c) : (a))\
    : (((c) < (b)) ? (c) : (b)))

        ASSERT(lhs != NULL);
        ASSERT(rhs != NULL);

        size_t len1 = std::char_traits<T>::length(lhs);
        size_t len2 = std::char_traits<T>::length(rhs);
        size_t retval = static_cast<size_t>(0);
        size_t *dist = new size_t[(len1 + 1) * (len2 + 1)];

        dist[0] = static_cast<size_t>(0);

        for (size_t i = 1; i <= len2; i++) {
            dist[i] = i * costAdd;
        }
   
        for (size_t i = 1; i <= len1; i++) {
            dist[VL_LVS_ARY_IDX(i, 0)] = i * costDelete;
        }

        for (size_t i = 1; i <= len1; i++) {
            for (size_t j = 1; j <= len2; j++) {
                dist[VL_LVS_ARY_IDX(i, j)] = VL_LVS_MIN3(
                    dist[VL_LVS_ARY_IDX(i - 1, j - 1)] 
                    + ((lhs[i - 1] == rhs[j - 1]) ? 0 : costChange),
                    dist[VL_LVS_ARY_IDX(i, j - 1)] + costAdd,
                    dist[VL_LVS_ARY_IDX(i - 1, j)] + costDelete);
            }
        }

        retval = dist[VL_LVS_ARY_IDX(len1, len2)];
        safe_array_delete(dist);
        return retval;

#undef VL_LVS_MIN3
#undef VL_LVS_ARY_IDX
    }


} /* end namespace text */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TEXT_STRINGLEVENSHTEINDISTANCE_H_INCLUDED */
