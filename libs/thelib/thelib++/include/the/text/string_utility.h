/*
 * the/text/string_utility.h
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
#ifndef THE_TEXT_STRING_UTILITY_H_INCLUDED
#define THE_TEXT_STRING_UTILITY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#ifndef THE_STRING_H_INCLUDED
#error "the/string" must be included before including "the/string_utility"
#endif /* THE_STRING_H_INCLUDED */
#include "the/assert.h"
#include "the/config.h"
#include "the/text/locale_utility.h"
#include "the/text/string_case_utility.h"
#include "the/text/string_compare_utility.h"
#include "the/text/string_double_invariant_utility.h"
#include "the/text/string_find_utility.h"
#include "the/text/string_format_utility.h"
#include "the/text/string_hash_utility.h"
#include "the/text/string_levenshtein_distance.h"
#include "the/text/string_parse_utility.h"
#include "the/text/string_raw_data_utility.h"
#ifdef THE_WINDOWS
#include "the/text/string_resource_utility.h"
#endif /* THE_WINDOWS */
#include "the/text/string_trim_utility.h"
#include "the/text/string_utf8_converter.h"
#include <utility>


namespace the {
namespace text {

    /**
     * Static class providing utility functions for the string classes
     */
    class string_utility : public string_case_utility,
        public string_compare_utility, public string_double_invariant_utility,
        public string_find_utility, public string_format_utility,
        public string_hash_utility, public string_levenshtein_distance,
        public string_parse_utility, public string_raw_data_utility,
#ifdef THE_WINDOWS
        public string_resource_utility,
#endif /* THE_WINDOWS */
        public string_trim_utility {
    public:

        /** empty ANSI string constant */
        static const astring empty_astring;

        /** empty unicode string constant */
        static const wstring empty_wstring;

        /**
         * Answer the length of a zero-terminated c string
         *
         * @param str The zero-terminate c string
         *
         * @return The length of the string in character, excluding the
         *         terminating zero
         *
         * @remarks: When using a multi-byte string, e.g. a char-string with
         *           utf8 locale under linux, this method returns the number
         *           of chars required to store the string, which might be
         *           larger than the actual characters.
         */
        template<class T>
        static inline size_t c_str_len(T *str) {
            if (str == nullptr) return 0;
            size_t len = 0;
            for (;*str != static_cast<T>(0); len++, str++);
            return len;
        }

        /**
         * Obfuscate the string by applying Rot48. This works only on
         * characters between 32 and 127.
         *
         * @param str The string to be rotated
         *
         * @return A reference to 'str'
         */
        template<class T>
        static T& rot48(T& str);

        /**
         * Replaces the following xml entities to their corresponding
         * characters:
         *  &amp;  '&'
         *  &lt;   '<'
         *  &gt;   '>'
         *  &quot; '"'
         *  &apos; '''
         *
         * @param inOutStr The string in which the replace operations will
         *                 occure
         */
        template<class T>
        static void decode_xml_entities(T& inOutStr);


        /**
         * Replaces the following characters with their corresponding xml
         * entities:
         *  '&'  &amp;
         *  '<'  &lt;
         *  '>'  &gt;
         *  '"'  &quot;
         *  '''  &apos;
         *
         * @param inOutStr The string in which the replace operations will
         *                 occure
         */
        template<class T>
        static void encode_xml_entities(T& inOutStr);

        /**
         * Escapes a set of characters within the string with a given escape
         * character.
         *
         * Example:
         *  EscapeCharacters('\\', "\n\r\t", "nrt");
         *
         * @param str The string to escape the characters in
         * @param ec The escape character. Must not be present in the 
         *           parameters 'normChars' and 'escpdChars'. Must not be zero.
         * @param normChars This string represents the list of characters to
         *                  be escaped. The character 'ec' must not be
         *                  included, since it is implicitly included. Each
         *                  character must only be present once.
         * @param escpdChars This string represents the list of escaped 
         *                   characters. The string must be of same length
         *                   as 'normChars', must not contain any character
         *                   more than once, and must not contain 'ec'.
         *
         * @return 'true' if the string was successfully escaped.
         *         'false' if there was an error.
         */
        template<class T>
        static bool escape_characters(T& str, const typename T::value_type ec,
            const T& normChars, const T& escpdChars);

        /**
         * Escapes a set of characters within the string with a given escape
         * character.
         *
         * Example:
         *  EscapeCharacters('\\', "\n\r\t", "nrt");
         *
         * @param str The string to escape the characters in
         * @param ec The escape character. Must not be present in the 
         *           parameters 'normChars' and 'escpdChars'. Must not be zero.
         * @param normChars This string represents the list of characters to
         *                  be escaped. The character 'ec' must not be
         *                  included, since it is implicitly included. Each
         *                  character must only be present once.
         * @param escpdChars This string represents the list of escaped 
         *                   characters. The string must be of same length
         *                   as 'normChars', must not contain any character
         *                   more than once, and must not contain 'ec'.
         *
         * @return 'true' if the string was successfully escaped.
         *         'false' if there was an error.
         */
        template<class T>
        static inline bool escape_characters(T& str,
                const typename T::value_type ec,
                const typename T::value_type *normChars,
                const typename T::value_type *escpdChars) {
            return escape_characters(str, ec, T(normChars), T(escpdChars));
        }

        /**
         * Unescapes a set of characters within the escaped string with a 
         * given escape character.
         *
         * Example:
         *  UnescapeCharacters('\\', "\n\r\t", "nrt");
         *
         * @param str The string to unescape the characters in
         * @param ec The escape character. Must not be present in the 
         *           parameters 'normChars' and 'escpdChars'. Must not be zero.
         * @param normChars This string represents the list of characters to
         *                  be restored by unescaping. The character 'ec' must
         *                  not be included, since it is implicitly included.
         *                  Each character must only be present once.
         * @param escpdChars This string represents the list of escaped 
         *                   characters. The string must be of same length
         *                   as 'normChars', must not contain any character
         *                   more than once, and must not contain 'ec'.
         *
         * @return 'true' if the string was successfully unescaped.
         *         'false' if there was an error.
         */
        template<class T>
        static bool unescape_characters(T& str, 
            const typename T::value_type ec, const T& normChars,
            const T& escpdChars);

        /**
         * Unescapes a set of characters within the escaped string with a 
         * given escape character.
         *
         * Example:
         *  UnescapeCharacters('\\', "\n\r\t", "nrt");
         *
         * @param str The string to unescape the characters in
         * @param ec The escape character. Must not be present in the 
         *           parameters 'normChars' and 'escpdChars'. Must not be zero.
         * @param normChars This string represents the list of characters to
         *                  be restored by unescaping. The character 'ec' must
         *                  not be included, since it is implicitly included.
         *                  Each character must only be present once.
         * @param escpdChars This string represents the list of escaped 
         *                   characters. The string must be of same length
         *                   as 'normChars', must not contain any character
         *                   more than once, and must not contain 'ec'.
         *
         * @return 'true' if the string was successfully unescaped.
         *         'false' if there was an error.
         */
        template<class T>
        static inline bool unescape_characters(T& str,
                const typename T::value_type ec,
                const typename T::value_type *normChars,
                const typename T::value_type *escpdChars) {
            return unescape_characters(str, ec, T(normChars), T(escpdChars));
        }

        /**
         * Returns a string where all sequences with percent (%) signs
         * followed by two hex digits have been replaced with literal
         * characters (according to RFC 3986).
         *
         * @param str The input string (7 bit ASCII)
         *
         * @return The reconstructed string
         */
        template<class T>
        static T url_decode(const std::string& str);

        /**
         * Returns a string where all sequences with percent (%) signs
         * followed by two hex digits have been replaced with literal
         * characters (according to RFC 3986).
         *
         * @param str The input string (7 bit ASCII)
         *
         * @return The reconstructed string
         */
        static inline the::astring url_decode_astr(const std::string& str) {
            return url_decode<the::astring>(str);
        }

        /**
         * Returns a string where all sequences with percent (%) signs
         * followed by two hex digits have been replaced with literal
         * characters (according to RFC 3986).
         *
         * @param str The input string (7 bit ASCII)
         *
         * @return The reconstructed string
         */
        static inline the::wstring url_decode_wstr(const std::string& str) {
            return url_decode<the::wstring>(str);
        }

        /**
         * Returns a string where all sequences with percent (%) signs
         * followed by two hex digits have been replaced with literal
         * characters (according to RFC 3986).
         *
         * @param str The input string (7 bit ASCII)
         *
         * @return The reconstructed string
         */
        static inline the::tstring url_decode_tstr(const std::string& str) {
            return url_decode<the::tstring>(str);
        }

        /**
         * Returns a string where all non-alphanumeric characters except
         * '-_.~' have been replaced with a percent (%) sign followed by two
         * hex digits (according to RFC 3986).
         *
         * @param str The input string
         *
         * @return The encoded string (7 bit ASCII)
         */
        template<class T>
        static std::string url_encode(const T& str);

    private:

        /** forbidden ctor */
        string_utility(void);

        /** forbidden copy ctor */
        string_utility(const string_utility& src);

        /** forbidden dtor */
        ~string_utility(void);

    };


    /*
     * string_utility::rot48
     */
    template<class T>
    T& string_utility::rot48(T& str) {
        size_t len = str.length();
        typename T::value_type *c = &str[0];
        for (size_t i = 0; i < len; i++, c++) {
            if ((*c > 31) && (*c < 128)) {
                *c = static_cast<typename T::value_type>(((*c - 32 + 48) % 96) + 32);
            }
        }
        return str;
    }


    /*
     * string_utility::decode_xml_entities
     */
    template<class T> void string_utility::decode_xml_entities(T& inOutStr) {
        using the::collections::array;
        using std::pair;

        array<pair<T, T>> m;
        m.set_size(5);
        string_converter::convert(m[0].first, "&apos;");
        string_converter::convert(m[0].second, "\'");
        string_converter::convert(m[1].first, "&quot;");
        string_converter::convert(m[1].second, "\"");
        string_converter::convert(m[2].first, "&gt;");
        string_converter::convert(m[2].second, ">");
        string_converter::convert(m[3].first, "&lt;");
        string_converter::convert(m[3].second, "<");
        string_converter::convert(m[4].first, "&amp;");
        string_converter::convert(m[4].second, "&");

        multi_replace(inOutStr, m);
    }


    /*
     * string_utility::encode_xml_entities
     */
    template<class T> void string_utility::encode_xml_entities(T& inOutStr) {
        using the::collections::array;
        using std::pair;

        array<pair<T, T> > m;
        m.set_size(5);
        string_converter::convert(m[0].first, "\'");
        string_converter::convert(m[0].second, "&apos;");
        string_converter::convert(m[1].first, "\"");
        string_converter::convert(m[1].second, "&quot;");
        string_converter::convert(m[2].first, ">");
        string_converter::convert(m[2].second, "&gt;");
        string_converter::convert(m[3].first, "<");
        string_converter::convert(m[3].second, "&lt;");
        string_converter::convert(m[4].first, "&");
        string_converter::convert(m[4].second, "&amp;");

        multi_replace(inOutStr, m);
    }


    /*
     * string_utility::escape_characters
     */
    template<class T>
    bool string_utility::escape_characters(T& str,
            const typename T::value_type ec, const T& normChars,
            const T& escpdChars) {
        using the::collections::array;
        using std::pair;
        ASSERT(ec != 0);
        ASSERT(normChars.length() == escpdChars.length());

        array<pair<T, T> > repMap;
        size_t len = normChars.length();
        repMap.set_size(1 + len);
        repMap[0].first = ec;
        repMap[0].second = ec;
        repMap[0].second += ec;
        for (size_t i = 0; i < len; ++i) {
            repMap[i + 1].first = normChars[i];
            repMap[i + 1].second = ec;
            repMap[i + 1].second += escpdChars[i];
        }

        multi_replace(str, repMap);

        return true;
    }


    /*
     * string_utility::unescape_characters
     */
    template<class T>
    bool string_utility::unescape_characters(T& str,
            const typename T::value_type ec, const T& normChars,
            const T& escpdChars) {
        using the::collections::array;
        using std::pair;
        ASSERT(ec != 0);
        ASSERT(normChars.length() == escpdChars.length());

        // we must first check for unrecognized escape sequences
        size_t i = str.find(ec, 0);
        while (i != T::npos) {
            const typename T::value_type& c = str[i + 1];
            if ((escpdChars.find(c) == T::npos) && (c != ec)) {
                // unrecognized escape sequence
                return false;
            }
            i = str.find(ec, i + 2);
        }

        array<pair<T, T> > repMap;
        size_t len = normChars.length();
        repMap.set_size(1 + len);
        repMap[0].first = ec;
        repMap[0].first += ec;
        repMap[0].second = ec;
        for (size_t i = 0; i < len; ++i) {
            repMap[i + 1].first = ec;
            repMap[i + 1].first += escpdChars[i];
            repMap[i + 1].second = normChars[i];
        }

        multi_replace(str, repMap);

        return true;
    }


    /*
     * string_utility::url_decode
     */
    template<class T>
    T string_utility::url_decode(const std::string& str) {
        size_t len = str.length();
        char *s = new char[len];
        ::memcpy(s, str.c_str(), len);
        size_t slen = 0;
        for (size_t i = 0; i < len; i++, slen++) {
            if (s[i] == '%') {
                if (i + 2 >= len) {
                    // data truncated
                    delete s;
                    throw new the::argument_exception("str",
                        __FILE__, __LINE__);
                }
                unsigned int v = 0;
                i++;
                if ((s[i] >= '0') && (s[i] <= '9')) {
                    v += (s[i] - '0');
                } else if ((s[i] >= 'a') && (s[i] <= 'f')) {
                    v += (10 + s[i] - 'a');
                } else if ((s[i] >= 'A') && (s[i] <= 'F')) {
                    v += (10 + s[i] - 'A');
                } else {
                    // non-hex escape sequence
                    delete s;
                    throw new the::argument_exception("str",
                        __FILE__, __LINE__);
                }
                v *= 16;
                i++;
                if ((s[i] >= '0') && (s[i] <= '9')) {
                    v += (s[i] - '0');
                } else if ((s[i] >= 'a') && (s[i] <= 'f')) {
                    v += (10 + s[i] - 'a');
                } else if ((s[i] >= 'A') && (s[i] <= 'F')) {
                    v += (10 + s[i] - 'A');
                } else {
                    // non-hex escape sequence
                    delete s;
                    throw new the::argument_exception("str",
                        __FILE__, __LINE__);
                }
                THE_ASSERT(v <= 255);
                s[slen] = static_cast<char>(static_cast<unsigned char>(v));
            } else {
                s[slen] = s[i];
            }
        }

        T res;
        string_utf8_converter::convert_from_utf8(res, s, slen);
        delete s;
        return res;
    }


    /*
     * string_utility::url_encode
     */
    template<class T>
    std::string string_utility::url_encode(const T& str) {
        std::string buf;
        string_utf8_converter::convert_to_utf8(buf, str);
        size_t len = buf.length();
        char format_buf[4] = { '%', 'x', 'x', '\0' };
        for (size_t i = 0; i < len; i++) {
            char c = buf[i];
            if (((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'z'))
                    || ((c >= 'A') && (c <= 'Z')) || (c == '-') || (c == '_')
                    || (c == '.') || (c == '~')) {
                // legal char
                continue;
            }
            unsigned char v = static_cast<unsigned char>(c);
            unsigned char lv = v / 16;
            if (lv > 15) lv = 15;
            format_buf[1] = (lv < 10) ? ('0' + lv) : ('a' + (lv - 10));
            lv = v % 16;
            if (lv > 15) lv = 15;
            format_buf[2] = (lv < 10) ? ('0' + lv) : ('a' + (lv - 10));
            buf.replace(i, 1, format_buf);
            i += 2;
            len += 2;
        }
        return buf;
    }


} /* end namesapce text */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TEXT_STRING_UTILITY_H_INCLUDED */
