/*
 * the/text/string_raw_data_utility.h
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
#ifndef THE_TEXT_STRINGRAWDATAUTILITY_H_INCLUDED
#define THE_TEXT_STRINGRAWDATAUTILITY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#ifndef THE_STRING_H_INCLUDED
#error "the/string" must be included before including "the/string_utility"
#endif /* THE_STRING_H_INCLUDED */
#include "the/types.h"

namespace the {
namespace text {

    /**
     * Utility class to convert between raw data and corresponding string
     * representations (HEX and Base64)
     *
     * @remark Do not use directly. Use 'string_utility' instead
     */
    class string_raw_data_utility {
    public:

        /**
         * Answer the size of the raw data encoded in 'str' in bytes, assuming
         * a Base64 encoding
         *
         * @param str The string holding the encoded data
         *
         * @return The size of the encoded data in bytes
         */
        template<class T>
        static size_t base64_string_length(const T& str);

        /**
         * Decodes raw data from 'str' which is assumed to be base 64 encoded
         *
         * @param data Pointer to the memory to receive the data
         * @param len The size of the reserved memory 'data' points to in bytes
         * @param str The string holding the encoded data
         *
         * @return true if the decoding was successful and 'data' provided
         *         enough space
         */
        template<class T>
        static bool from_base64_string(unsigned char *data, size_t len,
            const T& str);

        /**
         * Decodes raw data from 'str' which is assumed to be hex encoded
         *
         * @param data Pointer to the memory to receive the data
         * @param len The size of the reserved memory 'data' points to in bytes
         * @param str The string holding the encoded data
         *
         * @return true if the decoding was successful and 'data' provided
         *         enough space
         */
        template<class T>
        static bool from_hex_string(unsigned char *data, size_t len,
            const T& str);

        /**
         * Answer the size of the raw data encoded in 'str' in bytes, assuming
         * a hex encoding
         *
         * @param str The string holding the encoded data
         *
         * @return The size of the encoded data in bytes
         */
        template<class T>
        static size_t hex_string_length(const T& str);

        /**
         * Encodes a block of raw data in a string using base 64 encoding
         *
         * @param str the string to store the encoded data
         * @paran data pointer to the data to be stored
         * @param len the size of the data to be encoded in bytes
         */
        template<class T>
        static void to_base64_string(T& str, const unsigned char *data,
            size_t len);

        /**
         * Encodes a block of raw data in a string using hex encoding
         *
         * @param str the string to store the encoded data
         * @paran data pointer to the data to be stored
         * @param len the size of the data to be encoded in bytes
         */
        template<class T>
        static void to_hex_string(T& str, const unsigned char *data,
            size_t len);

        /**
         * Encodes a block of raw data in a string using base 64 encoding
         *
         * @paran data pointer to the data to be stored
         * @param len the size of the data to be encoded in bytes
         *
         * @return the string storing the encoded data
         */
        static inline astring to_base64_astring(const unsigned char *data,
                size_t len) {
            astring str;
            to_base64_string(str, data, len);
            return str;
        }

        /**
         * Encodes a block of raw data in a string using base 64 encoding
         *
         * @paran data pointer to the data to be stored
         * @param len the size of the data to be encoded in bytes
         *
         * @return the string storing the encoded data
         */
        static inline wstring to_base64_wstring(const unsigned char *data,
                size_t len) {
            wstring str;
            to_base64_string(str, data, len);
            return str;
        }

        /**
         * Encodes a block of raw data in a string using hex encoding
         *
         * @paran data pointer to the data to be stored
         * @param len the size of the data to be encoded in bytes
         *
         * @return the string storing the encoded data
         */
        static inline astring to_hex_astring(const unsigned char *data,
                size_t len) {
            astring str;
            to_hex_string(str, data, len);
            return str;
        }

        /**
         * Encodes a block of raw data in a string using hex encoding
         *
         * @paran data pointer to the data to be stored
         * @param len the size of the data to be encoded in bytes
         *
         * @return the string storing the encoded data
         */
        static inline wstring to_hex_wstring(const unsigned char *data,
                size_t len) {
            wstring str;
            to_hex_string(str, data, len);
            return str;
        }

    protected:

        /** forbidden Ctor */
        string_raw_data_utility(void);

        /** forbidden Copy Ctor */
        string_raw_data_utility(const string_raw_data_utility& src);

        /** forbidden Dtor */
        ~string_raw_data_utility(void);

    private:

        /** hex character helper */
        static const char hexHelp[16];

        /** base64 character helper */
        static const char base64Help[64];

    };


    /*
     * string_raw_data_utility::base64_string_length
     */
    template<class T>
    size_t string_raw_data_utility::base64_string_length(const T& str) {
        size_t len = str.length();
        size_t less = 0;
        for (size_t i = 0; i < len; i++) {
            if (!(((static_cast<typename T::value_type>('0') <= str[i])
                && (str[i] <= static_cast<typename T::value_type>('9')))
                || ((static_cast<typename T::value_type>('a') <= str[i])
                && (str[i] <= static_cast<typename T::value_type>('z')))
                || ((static_cast<typename T::value_type>('A') <= str[i])
                && (str[i] <= static_cast<typename T::value_type>('Z')))
                || (static_cast<typename T::value_type>('+') == str[i])
                || (static_cast<typename T::value_type>('/') == str[i])
                || (static_cast<typename T::value_type>('=') == str[i]))) {
                return (i / 4 * 3) - less;
            }
            if (static_cast<typename T::value_type>('=') == str[i]) {
                less++;
            }
        }
        return (len / 4 * 3) - less;
    }


    /*
     * string_raw_data_utility::from_base64_string
     */
    template<class T>
    bool string_raw_data_utility::from_base64_string(unsigned char *data,
            size_t len, const T& str) {
        size_t strLen = str.length();
        size_t less = 0;
        unsigned char v0, v1, v2, v3;

        for (size_t i = 0; i < strLen; i++) {
            if ((static_cast<typename T::value_type>('0') <= str[i])
                    && (str[i] <= static_cast<typename T::value_type>('9'))) {
                v3 = (str[i] - static_cast<typename T::value_type>('0')) + 52;
            } else if ((static_cast<typename T::value_type>('a') <= str[i])
                    && (str[i] <= static_cast<typename T::value_type>('z'))) {
                v3 = (str[i] - static_cast<typename T::value_type>('a')) + 26;
            } else if ((static_cast<typename T::value_type>('A') <= str[i])
                    && (str[i] <= static_cast<typename T::value_type>('Z'))) {
                v3 = (str[i] - static_cast<typename T::value_type>('A'));
            } else if (static_cast<typename T::value_type>('+') == str[i]) {
                v3 = 62;
            } else if (static_cast<typename T::value_type>('/') == str[i]) {
                v3 = 63;
            } else if (static_cast<typename T::value_type>('=') == str[i]) {
                v3 = 0;
                ++less;       
            } else {
                return false; // truncated
            }
            if ((i % 4) == 0) {
                v0 = v3;
            } else if ((i % 4) == 1) {
                v1 = v3;
            } else if ((i % 4) == 2) {
                v2 = v3;
            } else {
                if (len == 0) return false; // buffer depleted
                *data = v0 * 4 + v1 / 16;
                ++data;
                --len;
                if (len == 0) return (less == 2); // may be correct
                *data = v1 % 16 * 16 + v2 / 4;
                ++data;
                --len;
                if (len == 0) return (less == 1); // may be correct
                *data = v2 % 4 * 64 + v3;
                ++data;
                --len;
            }
        }

        return true; // all data successfully decoded
    }


    /*
     * string_raw_data_utility::from_hex_string
     */
    template<class T>
    bool string_raw_data_utility::from_hex_string(unsigned char *data,
            size_t len, const T& str) {
        size_t strLen = str.size();
        unsigned char v1, v2;
        for (size_t i = 0; i < strLen; i++) {
            if ((static_cast<typename T::value_type>('0') <= str[i])
                    && (str[i] <= static_cast<typename T::value_type>('9'))) {
                v1 = (str[i] - static_cast<typename T::value_type>('0'));
            } else if ((static_cast<typename T::value_type>('a') <= str[i])
                    && (str[i] <= static_cast<typename T::value_type>('f'))) {
                v1 = (str[i] - static_cast<typename T::value_type>('a')) + 10;
            } else if ((static_cast<typename T::value_type>('A') <= str[i])
                    && (str[i] <= static_cast<typename T::value_type>('F'))) {
                v1 = (str[i] - static_cast<typename T::value_type>('A')) + 10;
            } else {
                return false; // truncated
            }
            if ((i % 2) == 0) {
                v2 = v1 * 16;
            } else {
                if (len == 0) return false; // buffer depleted
                *data = v2 + v1;
                ++data;
                --len;
            }
        }

        return true; // all data successfully decoded
    }


    /*
     * string_raw_data_utility::hex_string_length
     */
    template<class T>
    size_t string_raw_data_utility::hex_string_length(const T& str) {
        size_t len = str.length();
        for (size_t i = 0; i < len; i++) {
            if (!(((static_cast<typename T::value_type>('0') <= str[i])
                && (str[i] <= static_cast<typename T::value_type>('9')))
                || ((static_cast<typename T::value_type>('a') <= str[i])
                && (str[i] <= static_cast<typename T::value_type>('f')))
                || ((static_cast<typename T::value_type>('A') <= str[i])
                && (str[i] <= static_cast<typename T::value_type>('F'))))) {
                return i / 2;
            }
        }
        return len / 2;
    }


    /*
     * string_raw_data_utility::to_base64_string
     */
    template<class T>
    void string_raw_data_utility::to_base64_string(T& str,
            const unsigned char *data, size_t len) {
        size_t fLen = len;
        while (fLen % 3 != 0) fLen++;
        str.resize(fLen * 4 / 3, 'x');
        unsigned char d1, d2, v0, v1, v2, v3;
        unsigned int fb = 0;

        for (size_t i = 0; i < len; i += 3) {
            const unsigned char &d0 = data[i];
            if (i + 1 >= len) {
                d1 = d2 = 0;
                fb = 2;
            } else if (i + 2 >= len) {
                d1 = data[i + 1];
                d2 = 0;
                fb = 1;
            } else {
                d1 = data[i + 1];
                d2 = data[i + 2];
                fb = 0;
            }

            v0 = d0 / 4;
            v1 = (d0 % 4) * 16 + d1 / 16;
            v2 = (d1 % 16) * 4 + d2 / 64;
            v3 = d2 % 64;

            str[i / 3 * 4 + 0] = static_cast<typename T::value_type>(
                base64Help[v0]);
            str[i / 3 * 4 + 1] = static_cast<typename T::value_type>(
                base64Help[v1]);
            str[i / 3 * 4 + 2] = static_cast<typename T::value_type>(
                (fb >= 2) ? '=' : base64Help[v2]);
            str[i / 3 * 4 + 3] = static_cast<typename T::value_type>(
                (fb >= 1) ? '=' : base64Help[v3]);
        }

    }


    /*
     * string_raw_data_utility::to_hex_string
     */
    template<class T>
    void string_raw_data_utility::to_hex_string(T& str,
            const unsigned char *data, size_t len) {
        str.resize(len * 2, 0);
        for (size_t i = 0; i < len; i++) {
            str[i * 2 + 0] = static_cast<typename T::value_type>(
                hexHelp[data[i] / 16]);
            str[i * 2 + 1] = static_cast<typename T::value_type>(
                hexHelp[data[i] % 16]);
        }
    }


} /* end namespace text */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TEXT_STRINGRAWDATAUTILITY_H_INCLUDED */
