/*
 * the/text/string_utf8_converter.cpp
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
#include "the/string.h"
#include "the/text/string_utf8_converter.h"
#include "the/assert.h"
#include "the/invalid_operation_exception.h"
#include "the/text/char_utility.h"
#include "the/text/locale_utility.h"
#include "the/text/string_converter.h"
#include "the/types.h"

#define __SUPPORT_SIX_OCTETS__ 1


/*
 * the::text::string_utf8_converter::convert_from_utf8
 */
bool the::text::string_utf8_converter::convert_from_utf8(the::astring& dst,
        const char *src, size_t len, const char defChar) {
    if (the::text::locale_utility::is_utf8_locale()) {
        dst.assign(src, len);
        return true;
    }

    Size s = string_utf8_converter::string_length(src, len);
    if (s < 0) return false;

    dst.resize(s);
    char *buf = const_cast<char*>(dst.data());
    if (s > 0) {
        const unsigned char *ucb = reinterpret_cast<const unsigned char*>(src);
        uint32_t val = 0;
        int ro; // remaining octets

        while ((*ucb != 0) && (len > 0)) {
            if (*ucb < 128) {
                // singlebyte character 
                *(buf++) = *reinterpret_cast<const char*>(ucb);

            } else {
                // multibyte character

                if ((*ucb & 0xE0) == 0xC0) { 
                    val = *ucb & 0x1F;
                    ro = 1; 
                } else if ((*ucb & 0xF0) == 0xE0) { 
                    val = *ucb & 0x0F;
                    ro = 2; 
                } else if ((*ucb & 0xF8) == 0xF0) { 
                    val = *ucb & 0x07;
                    ro = 3; 
                } else if ((*ucb & 0xFC) == 0xF8) { 
                    val = *ucb & 0x03;
                    ro = 4; 
                } else if ((*ucb & 0xFE) == 0xFC) { 
                    val = *ucb & 0x01;
                    ro = 5; 
                } else {
                    return false; // invalid starting bit pattern
                }

                for (;ro > 0; ro--) {
                    ucb++;
                    len--;
                    if (len <= 0) {
                        return false; // data truncation detected
                    }
                    val = (val << 6) | (*ucb & 0x3F);
                    if ((*ucb & 0xC0) != 0x80) {
                        return false; // invalid block bit pattern.
                    }
                }

                *(buf++) = the::text::char_utility::to_ansi(
                    static_cast<wchar_t>(val), defChar);

            }
            ucb++;
            len--;
        }
        *buf = 0; // unsure if this is a good idea
    }

    return true;
}


/*
 * the::text::string_utf8_converter::convert_from_utf8
 */
bool the::text::string_utf8_converter::convert_from_utf8(the::wstring& dst,
        const char *src, size_t len) {
    Size s = string_utf8_converter::string_length(src, len);
    if (s < 0) return false;

    dst.resize(s);
    wchar_t *buf = const_cast<wchar_t*>(dst.data());
    if (s > 0) {
        const unsigned char *ucb = reinterpret_cast<const unsigned char*>(src);
        uint32_t val = 0;
        int ro; // remaining octets

        while ((*ucb != 0) && (len > 0)) {
            if (*ucb < 128) {
                // singlebyte character 
                *(buf++) = *reinterpret_cast<const char*>(ucb);

            } else {
                // multibyte character

                if ((*ucb & 0xE0) == 0xC0) { 
                    val = *ucb & 0x1F;
                    ro = 1; 
                } else if ((*ucb & 0xF0) == 0xE0) { 
                    val = *ucb & 0x0F;
                    ro = 2; 
                } else if ((*ucb & 0xF8) == 0xF0) { 
                    val = *ucb & 0x07;
                    ro = 3; 
                } else if ((*ucb & 0xFC) == 0xF8) { 
                    val = *ucb & 0x03;
                    ro = 4; 
                } else if ((*ucb & 0xFE) == 0xFC) { 
                    val = *ucb & 0x01;
                    ro = 5; 
                } else {
                    return false; // invalid starting bit pattern
                }

                for (;ro > 0; ro--) {
                    ucb++;
                    len--;
                    if (len <= 0) {
                        return false; // data truncation detected
                    }
                    val = (val << 6) | (*ucb & 0x3F);
                    if ((*ucb & 0xC0) != 0x80) {
                        return false; // invalid block bit pattern.
                    }
                }

                *(buf++) = static_cast<wchar_t>(val);

            }
            ucb++;
            len--;
        }
        *buf = 0; // unsure if this is a good idea
    }

    return true;
}


/*
 * the::text::string_utf8_converter::convert_to_utf8
 */
bool the::text::string_utf8_converter::convert_to_utf8(the::astring& dst,
        const char *src, size_t len) {
    if (the::text::locale_utility::is_utf8_locale()) {
        dst.assign(src, len);
        return true;
    }

    Size size = string_utf8_converter::calc_utf8_size(src, len);
    if (size <= 0) return false; // not representable characters included

    dst.resize(size - 1, 0);
    unsigned char *cb = reinterpret_cast<unsigned char*>(
        const_cast<char*>(dst.data()));

    if (size > 1) {
        uint32_t val;

        while ((*src != 0) && (len > 0)) {

            if (*reinterpret_cast<const unsigned char*>(src) < 128) {
                // 7 Bit : 2^7=128
                *(cb++) = static_cast<unsigned char>(*src);

            } else {
                val = static_cast<uint32_t>(
                    the::text::char_utility::to_unicode(*src));

                if (val < 2048) { // 5 + 6 = 11 Bit : 2^11=2048
                    // 110xxxxx 10xxxxxx
                    cb[1] = 0x80 | (val & 0x3F);
                    cb[0] = 0xC0 | (val >> 6);
                    cb += 2;

                } else if (val < 65536) { // 4 + 6 + 6 = 16 Bit : 2^16=65536
                    // 1110xxxx 10xxxxxx 10xxxxxx
                    cb[2] = 0x80 | (val & 0x3F);
                    cb[1] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[0] = 0xE0 | (val >> 6);
                    cb += 3;

                } else if (val < 2097152) { // 3 + 6 + 6 + 6 = 21 Bit :
                                            //  2^21=2097152
                    // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                    cb[3] = 0x80 | (val & 0x3F);
                    cb[2] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[1] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[0] = 0xF0 | (val >> 6);
                    cb += 4;

#ifdef __SUPPORT_SIX_OCTETS__

                } else if (val < 67108864) { // 2 + 6 + 6 + 6 + 6 = 26 Bit :
                                             //  2^26=67108864
                    // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
                    cb[4] = 0x80 | (val & 0x3F);
                    cb[3] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[2] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[1] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[0] = 0xF8 | (val >> 6);
                    cb += 5;

                } else if (val < 2097152) { // 1 + 6 + 6 + 6 + 6 + 6 = 31 Bit :
                                            //  2^31=2147483648
                    // 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
                    cb[5] = 0x80 | (val & 0x3F);
                    cb[4] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[3] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[2] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[1] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[0] = 0xFC | (val >> 6);
                    cb += 6;

#endif /* __SUPPORT_SIX_OCTETS__ */

                } else { 
                    return false; // error, not representable character

                }
            }

            src++;
            len--;
        }
    }

    return true;
}


/*
 * the::text::string_utf8_converter::convert_to_utf8
 */
bool the::text::string_utf8_converter::convert_to_utf8(the::astring& dst,
        const wchar_t *src, size_t len) {

    Size size = string_utf8_converter::calc_utf8_size(src, len);
    if (size <= 0) return false; // not representable characters included

    dst.resize(size - 1, 0);
    unsigned char *cb = reinterpret_cast<unsigned char*>(
        const_cast<char*>(dst.data()));

    if (size > 1) {
        uint32_t val;

        while ((*src != 0) && (len > 0)) {

            if (*src < 128) {
                // 7 Bit : 2^7=128
                *(cb++) = static_cast<unsigned char>(*src);

            } else {
                val = static_cast<uint32_t>(*src);

                if (val < 2048) { // 5 + 6 = 11 Bit : 2^11=2048
                    // 110xxxxx 10xxxxxx
                    cb[1] = 0x80 | (val & 0x3F);
                    cb[0] = 0xC0 | (val >> 6);
                    cb += 2;

                } else if (val < 65536) { // 4 + 6 + 6 = 16 Bit : 2^16=65536
                    // 1110xxxx 10xxxxxx 10xxxxxx
                    cb[2] = 0x80 | (val & 0x3F);
                    cb[1] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[0] = 0xE0 | (val >> 6);
                    cb += 3;

                } else if (val < 2097152) { // 3 + 6 + 6 + 6 = 21 Bit :
                                            //  2^21=2097152
                    // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                    cb[3] = 0x80 | (val & 0x3F);
                    cb[2] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[1] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[0] = 0xF0 | (val >> 6);
                    cb += 4;

#ifdef __SUPPORT_SIX_OCTETS__

                } else if (val < 67108864) { // 2 + 6 + 6 + 6 + 6 = 26 Bit :
                                             //  2^26=67108864
                    // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
                    cb[4] = 0x80 | (val & 0x3F);
                    cb[3] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[2] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[1] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[0] = 0xF8 | (val >> 6);
                    cb += 5;

                } else if (val < 2097152) { // 1 + 6 + 6 + 6 + 6 + 6 = 31 Bit :
                                            //  2^31=2147483648
                    // 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
                    cb[5] = 0x80 | (val & 0x3F);
                    cb[4] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[3] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[2] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[1] = 0x80 | ((val >>= 6) & 0x3F);
                    cb[0] = 0xFC | (val >> 6);
                    cb += 6;

#endif /* __SUPPORT_SIX_OCTETS__ */

                } else { 
                    return false; // error, not representable character

                }
            }

            src++;
            len--;
        }
    }

    return true;
}


/*
 * the::text::string_utf8_converter::string_utf8_converter
 */
the::text::string_utf8_converter::string_utf8_converter(void) {
    throw the::invalid_operation_exception("string_utf8_converter::ctor",
        __FILE__, __LINE__);
}


/*
 * the::text::string_utf8_converter::string_utf8_converter
 */
the::text::string_utf8_converter::string_utf8_converter(
        const the::text::string_utf8_converter& src) {
    throw the::invalid_operation_exception("string_utf8_converter::ctor",
        __FILE__, __LINE__);
}


/*
 * the::text::string_utf8_converter::~string_utf8_converter
 */
the::text::string_utf8_converter::~string_utf8_converter(void) {
    throw the::invalid_operation_exception("string_utf8_converter::dtor",
        __FILE__, __LINE__);
}


/*
 * the::text::string_utf8_converter::calc_utf8_size
 */
the::text::string_utf8_converter::Size
the::text::string_utf8_converter::calc_utf8_size(const char *str, size_t len) {
    ASSERT(!the::text::locale_utility::is_utf8_locale());
    Size size = 1; // terminating zero

    if ((str != NULL) && (len > 0)) {
        const unsigned char *ucb = reinterpret_cast<const unsigned char*>(str);

        while ((*ucb != 0) && (len > 0)) {
            if (*ucb < 128) {
                // std ANSI needs a single byte
                size += 1; 

            } else {
                // extended ANSI must be treated like Unicode-16
                wchar_t w = the::text::char_utility::to_unicode(
                    static_cast<const char>(*ucb));

                if (w < 128) {              // 7 Bit : 
                                            //  2^7=128
                    size += 1; // should never be called
                } else if (w < 2048) {      // 5 + 6 = 11 Bit :
                                            //  2^11=2048
                    size += 2;
                } else if (w < 65536) {     // 4 + 6 + 6 = 16 Bit :
                                            //  2^16=65536
                    size += 3;
                } else if (w < 2097152) {   // 3 + 6 + 6 + 6 = 21 Bit :
                                            //  2^21=2097152
                    size += 4;
#ifdef __SUPPORT_SIX_OCTETS__
                } else if (w < 67108864) {  // 2 + 6 + 6 + 6 + 6 = 26 Bit :
                                            //  2^26=67108864
                    size += 5; // Not too unicode compatible!
                } else if (w < 2097152) {   // 1 + 6 + 6 + 6 + 6 + 6 = 31 Bit :
                                            //  2^31=2147483648
                    size += 6; // Not too unicode compatible!
#endif /* __SUPPORT_SIX_OCTETS__ */
                } else { 
                    return -size; // error, not representable character
                }

            }

            ucb++;
            len--;
        }
    }

    return size;
}


/*
 * the::text::string_utf8_converter::calc_utf8_size
 */
the::text::string_utf8_converter::Size
the::text::string_utf8_converter::calc_utf8_size(const wchar_t *str, size_t len) {
    Size size = 1; // terminating zero

    if ((str != NULL) && (len > 0)) {
        while ((*str != 0) && (len > 0)) {

            if (*str < 128) {               // 7 Bit :
                                            //  2^7=128
                size += 1; 
            } else if (*str < 2048) {       // 5 + 6 = 11 Bit :
                                            //  2^11=2048
                size += 2;
            } else if (*str < 65536) {      // 4 + 6 + 6 = 16 Bit :
                                            //  2^16=65536
                size += 3;
            } else if (*str < 2097152) {    // 3 + 6 + 6 + 6 = 21 Bit :
                                            //  2^21=2097152
                size += 4;
#ifdef __SUPPORT_SIX_OCTETS__
            } else if (*str < 67108864) {   // 2 + 6 + 6 + 6 + 6 = 26 Bit :
                                            //  2^26=67108864
                size += 5; // Not too unicode compatible!
            } else if (*str < 2097152) {    // 1 + 6 + 6 + 6 + 6 + 6 = 31 Bit :
                                            //  2^31=2147483648
                size += 6; // Not too unicode compatible!
#endif /* __SUPPORT_SIX_OCTETS__ */
            } else { 
                return -size; // error, not representable character
            }

            str++;
            len--;
        }
    }

    return size;
}


/*
 * the::text::string_utf8_converter::string_length
 */
the::text::string_utf8_converter::Size
the::text::string_utf8_converter::string_length(const char *str, size_t len) {
    Size size = 0;

    if ((str != NULL) && (len > 0)) {
        const unsigned char *ucb = reinterpret_cast<const unsigned char*>(str);

        while ((*ucb != 0) && (len > 0)) {
            if (*ucb > 127) {
                // multibyte character
                int ro; // remaining octets

                if ((*ucb & 0xE0) == 0xC0) { 
                    ro = 1; 
                } else if ((*ucb & 0xF0) == 0xE0) { 
                    ro = 2; 
                } else if ((*ucb & 0xF8) == 0xF0) { 
                    ro = 3; 
                } else if ((*ucb & 0xFC) == 0xF8) { 
                    ro = 4; 
                } else if ((*ucb & 0xFE) == 0xFC) { 
                    ro = 5; 
                } else {
                    return -1; // invalid starting bit pattern
                }

                for (;ro > 0; ro--) {
                    ucb++;
                    len--;
                    if (len == 0) {
                        return -3; // data truncation detected
                    }
                    if ((*ucb & 0xC0) != 0x80) {
                        return -2; // invalid block bit pattern.
                    }
                }
            }

            size++;
            ucb++;
            len--;
        }
    }

    return size;
}
