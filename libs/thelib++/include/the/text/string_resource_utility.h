/*
 * the/text/string_resource_utility.h
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
#ifndef THE_TEXT_STRING_RESOURCE_UTILITY_H_INCLUDED
#define THE_TEXT_STRING_RESOURCE_UTILITY_H_INCLUDED
#ifdef _WIN32
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#ifndef THE_STRING_H_INCLUDED
#error "the/string" must be included before including "the/string_converter"
#endif /* THE_STRING_H_INCLUDED */


namespace the {
namespace text {

    /**
     * Utility class to load strings form windows resources
     *
     * @remark Do not use directly. Use 'string_utility' instead
     */
    class string_resource_utility {
    public:

        /**
         * Loads a string from a windows resource
         *
         * @param str receives the loaded string
         * @param hInst the module instance handle
         * @param id the string resource id
         *
         * @return true on success, false on failure
         */
        static bool load_resource(
            astring& str, HINSTANCE hInst, const UINT id);

        /**
         * Loads a string from a windows resource
         *
         * @param str receives the loaded string
         * @param hInst the module instance handle
         * @param id the string resource id
         *
         * @return true on success, false on failure
         */
        static bool load_resource(
            wstring& str, HINSTANCE hInst, const UINT id);

        /**
         * Loads a string from a windows resource
         *
         * @param str receives the loaded string
         * @param id the string resource id
         *
         * @return true on success, false on failure
         */
        static inline bool load_resource(astring& str, const UINT id) {
            return load_resource(str, ::GetModuleHandleA(NULL), id);
        }

        /**
         * Loads a string from a windows resource
         *
         * @param str receives the loaded string
         * @param id the string resource id
         *
         * @return true on success, false on failure
         */
        static inline bool load_resource(wstring& str, const UINT id) {
            return load_resource(str, ::GetModuleHandleA(NULL), id);
        }

        /**
         * Loads a string for a windows resource
         *
         * @param hInst the module instance handle
         * @param id the string resource id
         *
         * @return the loaded string
         *
         * @throws exception on error
         */
        static inline astring load_resource_astr(HINSTANCE hInst, const UINT id) {
            astring str;
            if (!load_resource(str, hInst, id)) {
                throw exception(__FILE__, __LINE__);
            }
            return str;
        }

        /**
         * Loads a string for a windows resource
         *
         * @param id the string resource id
         *
         * @return the loaded string
         *
         * @throws exception on error
         */
        static inline astring load_resource_astr(const UINT id) {
            astring str;
            if (!load_resource(str, ::GetModuleHandleA(NULL), id)) {
                throw exception(__FILE__, __LINE__);
            }
            return str;
        }

        /**
         * Loads a string for a windows resource
         *
         * @param hInst the module instance handle
         * @param id the string resource id
         *
         * @return the loaded string
         *
         * @throws exception on error
         */
        static inline tstring load_resource_tstr(HINSTANCE hInst, const UINT id) {
            tstring str;
            if (!load_resource(str, hInst, id)) {
                throw exception(__FILE__, __LINE__);
            }
            return str;
        }

        /**
         * Loads a string for a windows resource
         *
         * @param id the string resource id
         *
         * @return the loaded string
         *
         * @throws exception on error
         */
        static inline tstring load_resource_tstr(const UINT id) {
            tstring str;
            if (!load_resource(str, ::GetModuleHandleA(NULL), id)) {
                throw exception(__FILE__, __LINE__);
            }
            return str;
        }

        /**
         * Loads a string for a windows resource
         *
         * @param hInst the module instance handle
         * @param id the string resource id
         *
         * @return the loaded string
         *
         * @throws exception on error
         */
        static inline wstring load_resource_wstr(HINSTANCE hInst, const UINT id) {
            wstring str;
            if (!load_resource(str, hInst, id)) {
                throw exception(__FILE__, __LINE__);
            }
            return str;
        }

        /**
         * Loads a string for a windows resource
         *
         * @param id the string resource id
         *
         * @return the loaded string
         *
         * @throws exception on error
         */
        static inline wstring load_resource_wstr(const UINT id) {
            wstring str;
            if (!load_resource(str, ::GetModuleHandleA(NULL), id)) {
                throw exception(__FILE__, __LINE__);
            }
            return str;
        }

    protected:

        /** forbidden Ctor */
        string_resource_utility(void);

        /** forbidden Copy Ctor */
        string_resource_utility(const string_resource_utility& src);

        /** forbidden Dtor */
        ~string_resource_utility(void);

    private:
    };

} /* end namespace text */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* _WIN32 */
#endif /* THE_TEXT_STRING_RESOURCE_UTILITY_H_INCLUDED */
