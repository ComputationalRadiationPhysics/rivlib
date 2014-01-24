/*
 * the\system\cmd_line_provider.h
 *
 * Copyright (C) 2013 TheLib Team (http://www.thelib.org/license)
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

#ifndef THE_SYSTEM_CMD_LINE_PROVIDER_H_INCLUDED
#define THE_SYSTEM_CMD_LINE_PROVIDER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/memory.h"
#include "the/stack_trace.h"
#include "the/string.h"
#include "the/tchar.h"
#include "the/text/char_utility.h"
#include "the/text/string_converter.h"
#include "the/text/string_utility.h"


namespace the {
namespace system {

    /**
     * Helper class to split a single string command line into an array of zero
     * terminated c strings.
     *
     * @param T the character type (char or wchar_t)
     */
    template<class T>
    class cmd_line_provider {
    public:

        /** character type */
        typedef T char_type;

        /** string type */
        typedef std::basic_string<T> string_type;

        /**
         * Convenience method for creating a command line provider from 
         * standard main ANSI input parameters regardless of the type of the
         * command line parser.
         *
         * This is primarily intended for Linux.
         *
         * @param argc The number of elements in 'argv'.
         * @param argv The array of command line arguments. The first should be
         *             the application name.
         *
         * @return A cmd_line_provider representing the specified command
         *         line arguments.
         */
        static cmd_line_provider create(int argc, char **argv);

        /**
         * Initialises a new instance.
         */
        cmd_line_provider(void);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        cmd_line_provider(const cmd_line_provider& rhs);

        /**
         * Create command line provider from C style parameters.
         *
         * @param argc An integer that contains the count of arguments that 
         *             follow in 'argv'. 
         * @param argv An array of null-terminated strings representing 
         *             command-line arguments entered by the user of the program. 
         */
        cmd_line_provider(int argc, char_type **argv);

        /**
         * Ctor with initialization
         * See: create_cmd_line for further Information
         *
         * @param cmd_line The single string command line including the 
         *                application name.
         */
        cmd_line_provider(const char_type *cmd_line);

        /**
         * Ctor with initialization
         * See: create_cmd_line for further Information
         *
         * @param cmd_line The single string command line including the 
         *                application name.
         */
        cmd_line_provider(const string_type& cmd_line);

        /**
         * Ctor with initialization
         * See: create_cmd_line for further Information
         *
         * @param app_name The application name.
         * @param cmd_line The single string command line excluding the 
         *                 application name.
         */
        cmd_line_provider(const char_type *app_name, const char_type *cmd_line);

        /**
         * Ctor with initialization
         * See: create_cmd_line for further Information
         *
         * @param app_name The application name.
         * @param cmd_line The single string command line excluding the 
         *                 application name.
         */
        cmd_line_provider(const string_type& app_name, const char_type *cmd_line);

        /**
         * Ctor with initialization
         * See: create_cmd_line for further Information
         *
         * @param app_name The application name.
         * @param cmd_line The single string command line excluding the 
         *                 application name.
         */
        cmd_line_provider(const char_type *app_name, const string_type& cmd_line);

        /**
         * Ctor with initialization
         * See: create_cmd_line for further Information
         *
         * @param app_name The application name.
         * @param cmd_line The single string command line excluding the 
         *                 application name.
         */
        cmd_line_provider(const string_type& app_name, const string_type& cmd_line);

        /** Dtor. */
        virtual ~cmd_line_provider(void);

        /**
         * Splits the provided single string command line into an array of 
         * strings. 
         * See: create_cmd_line for further Information
         *
         * @param cmd_line The single string command line including the 
         *                 application name.
         */
        inline void create_cmd_line(const char_type *cmd_line) {
            THE_STACK_TRACE;
            this->create_cmd_line(nullptr, cmd_line);
        }

        /**
         * Splits the provided single string command line into an array of 
         * strings. 
         * See: create_cmd_line for further Information
         *
         * @param cmd_line The single string command line including the 
         *                 application name.
         */
        inline void create_cmd_line(const string_type& cmd_line) {
            THE_STACK_TRACE;
            this->create_cmd_line(nullptr, cmd_line.c_str());
        }

        /**
         * Splits the provided single string command line into an array of 
         * strings. Each string represents one command line argument. The
         * strings are splitted at whitespace characters outside of double 
         * qouted sections. Single double qout characters will be removed, 
         * since they either start or end a qouted section. If the command line
         * ends within a double qouted section, the last parameter will begin
         * with a qout character.
         *
         * Remarks: Since single and double qout characters are important 
         * input in the following paragraphs, input command lines are placed 
         * in angle brackets < >.
         *
         * To produce as double qout character inside a double qouted section, 
         * escape the character by another double qout character 
         * (i.e.: "1 "" 2" will be parsed as a single parameter string holding
         * <1 " 2>).
         * 
         * if app_name is not nullptr, this string becomes the first command 
         * line argument, followed by the strings created from cmdLine. The 
         * splitted substrings are copied into the new array.
         *
         * @warning 
         *  When using windows powershell there will be problem when parsing
         *  command lines holding parameters containing escaped double qout
         *  characters.
         *
         *  When using a single string command line provided by the 
         *  operating system (e.g. cmd.exe or powershell.exe calling a windows
         *  application using WinMain) there may be problems with escaped
         *  double qout characters. I.e. calling <prog.exe p1 "p2 `" p3" p4> in
         *  the windows power shell will produce the command line string 
         *  <p1 "p2 " p3" p4>, which does not hold any information that the 
         *  second double qout character was escaped. Therefore CreateCmdLine
         *  will produce a non intuitive result: {"p1", "p2 ", "p3 p4"}.
         *
         *  Windows power shell also provides the possibility to mark 
         *  parameters containing spaces using single qouts <'>. This must also
         *  be considered with care, since these will be repaced with qouble 
         *  qout characters by the power shell. I.e. calling <prog.exe '1 " 2'>
         *  will produce <prog.exe "1 " 2">, which will then be parsed into the
         *  following result: {"1 ", "2"}.
         *
         * @param app_name The application name.
         * @param cmd_line The single string command line excluding the 
         *                 application name.
         */
        void create_cmd_line(const char_type *app_name, const char_type *cmd_line);

        /**
         * Splits the provided single string command line into an array of 
         * strings. 
         * See: create_cmd_line for further Information
         *
         * @param app_name The application name.
         * @param cmd_line The single string command line excluding the 
         *                 application name.
         */
        inline void create_cmd_line(const string_type& app_name, const char_type *cmd_line) {
            THE_STACK_TRACE;
            this->create_cmd_line(app_name.c_str(), cmd_line);
        }

        /**
         * Splits the provided single string command line into an array of 
         * strings. 
         * See: create_cmd_line for further Information
         *
         * @param app_name The application name.
         * @param cmd_line The single string command line excluding the 
         *                 application name.
         */
        inline void create_cmd_line(const char_type *app_name, const string_type& cmd_line) {
            THE_STACK_TRACE;
            this->create_cmd_line(app_name, cmd_line.c_str());
        }

        /**
         * Splits the provided single string command line into an array of 
         * strings. 
         * See: create_cmd_line for further Information
         *
         * @param app_name The application name.
         * @param cmd_line The single string command line excluding the 
         *                 application name.
         */
        inline void create_cmd_line(const string_type& app_name, const string_type& cmd_line) {
            THE_STACK_TRACE;
            this->create_cmd_line(app_name.c_str(), cmd_line.c_str());
        }

        /**
         * Returns a reference to the number of arguments in the arguemnt list.
         * The value should be changed by the caller to reflect any changes 
         * made to array of arguments returned by 'argv'
         *
         * @return Reference to the number of arguments
         */
        inline int& argc(void) {
            THE_STACK_TRACE;
            return this->use_argc;
        }

        /**
         * Returns the argument list as array of zero terminated strings. The
         * caller may change the values of the array of strings, but must not
         * free the memory the array values points to, even if the pointers
         * are lost, since the memory will be handled by the cmd_line_provider
         * object. 
         *
         * The value of the reference returned by 'argc' should also be changed
         * to reflect any changes to the array returned by 'argv'.
         *
         * @return The argument list. Might be nullptr, if 'argc' returned Zero.
         */
        inline T **argv(void) {
            THE_STACK_TRACE;
            return this->use_argv;
        }

        /**
         * Appends an argument to the command line.
         *
         * @param arg The argument to be appenden to the command line.
         */
        void append(const char_type *arg);

        /**
         * Appends an argument to the command line.
         *
         * @param arg The argument to be appenden to the command line.
         */
        inline void append(const string_type& arg) {
            THE_STACK_TRACE;
            this->append(arg.c_str());
        }

        /**
         * Prepends an argument to the command line.
         *
         * @param arg The argument to be prependen to the command line.
         */
        void prepend(const char_type *arg);

        /**
         * Prepends an argument to the command line.
         *
         * @param arg The argument to be prependen to the command line.
         */
        inline void prepend(const string_type& arg) {
            THE_STACK_TRACE;
            this->prepend(arg.c_str());
        }

        /**
         * Generates and returns a single string containing the whole command
         * line. Calling 'create_cmd_line()' with this string, will generate
         * an internal data structure equal to the one of this object.
         *
         * This member works with the same variables which are returned by
         * 'argc' and 'argv'. So if the caller has changes these values in any
         * inconsitent way, the return value of this member is undefined.
         *
         * @param include_first Flag whether to include the first argument of
         *            the argument list. Useful to create a single string
         *            command line excluding the application name from a
         *            argument list containing the application name.
         *
         * @return The generated single string command line.
         */
        string_type single_string_cmd_line(bool include_first = true);

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        cmd_line_provider& operator =(const cmd_line_provider& rhs);

    private:

        /** clears the argument list */
        void clear_arg_list(void);

        /**
         * Creates an argument from two given pointers.
         *
         * @param left The left pointer.
         * @param right The right pointer.
         *
         * @return The new created argument string.
         */
        char_type *create_arg(char_type *left, char_type *right);

        /**
         * Reflects all changes done to the pointers returned by 'argc' and 
         * 'argv' by updating 'memory_argc' and 'memory_argv'.
         */
        void reflect_caller_changes(void);

        /** 
         * Number of created arguments. This is not unsigned to be more 
         * compliant to the specification of 'main'.
         */
        int use_argc;

        /** list of created arguments */
        char_type **use_argv;

        /**
         * Number of created arguments. This is not unsigned to be more 
         * compliant to the specification of 'main'. This member is
         * necessary to reflect changes to the argument pointer list returned 
         * by 'argv'.
         */
        int memory_argc;

        /** 
         * memory anchor of the list of created arguments. This member is 
         * necessary to aviod memory leaks if the argument pointer list 
         * returned by 'argv' is changed.
         */
        char_type **memory_argv[2];

    };


    /*
     * cmd_line_provider<T>::create
     */
    template<class T>
    cmd_line_provider<T> cmd_line_provider<T>::create(int argc, char **argv) {
        THE_STACK_TRACE;
        cmd_line_provider<T> retval;
        for (int i = 0; i < argc; i++) {
            string_type str;
            the::text::string_converter::convert(str, argv[i]);
            retval.append(str);
        }
        return retval;
    }


#pragma warning(disable: 4351)

    /*
     * cmd_line_provider<T>::cmd_line_provider
     */
    template<class T>
    cmd_line_provider<T>::cmd_line_provider(void) : use_argc(0), use_argv(nullptr), memory_argc(0), memory_argv() {
        THE_STACK_TRACE;
        this->memory_argv[0] = nullptr;
        this->memory_argv[1] = nullptr;
    }


    /*
     * cmd_line_provider<T>::cmd_line_provider
     */
    template<class T>
    cmd_line_provider<T>::cmd_line_provider(const cmd_line_provider& rhs) : use_argc(0), use_argv(nullptr), memory_argc(0), memory_argv() {
        THE_STACK_TRACE;
        this->memory_argv[0] = nullptr;
        this->memory_argv[1] = nullptr;
        *this = rhs;
    }


    /*
     * cmd_line_provider<T>::cmd_line_provider
     */
    template<class T>
    cmd_line_provider<T>::cmd_line_provider(int argc, char_type **argv) : use_argc(0), use_argv(nullptr), memory_argc(0), memory_argv() {
        THE_STACK_TRACE;
        this->use_argc = this->memory_argc = argc;
        this->memory_argv[0] = new char_type *[this->use_argc];
        this->memory_argv[1] = new char_type *[this->use_argc];
        this->use_argv = this->memory_argv[1];

        THE_ASSERT(this->use_argc == this->memory_argc);
        for (int i = 0; i < this->use_argc; i++) {
            size_t len = the::text::string_utility::c_str_len(argv[i]) + 1;
            this->use_argv[i] = this->memory_argv[0][i] = new char_type[len];
            ::memcpy(this->memory_argv[0][i], argv[i], len * sizeof(char_type));
        }
    }


    /*
     * cmd_line_provider<T>::cmd_line_provider
     */
    template<class T>
    cmd_line_provider<T>::cmd_line_provider(const char_type *cmd_line) : use_argc(0), use_argv(nullptr), memory_argc(0), memory_argv() {
        THE_STACK_TRACE;
        this->memory_argv[0] = nullptr;
        this->memory_argv[1] = nullptr;
        this->create_cmd_line(nullptr, cmd_line);
    }


    /*
     * cmd_line_provider<T>::cmd_line_provider
     */
    template<class T>
    cmd_line_provider<T>::cmd_line_provider(const string_type& cmd_line) : use_argc(0), use_argv(nullptr), memory_argc(0), memory_argv() {
        THE_STACK_TRACE;
        this->memory_argv[0] = nullptr;
        this->memory_argv[1] = nullptr;
        this->create_cmd_line(nullptr, cmd_line.c_str());
    }


    /*
     * cmd_line_provider<T>::cmd_line_provider
     */
    template<class T>
    cmd_line_provider<T>::cmd_line_provider(const char_type *app_name, const char_type *cmd_line) : use_argc(0), use_argv(nullptr), memory_argc(0), memory_argv() {
        THE_STACK_TRACE;
        this->memory_argv[0] = nullptr;
        this->memory_argv[1] = nullptr;
        this->create_cmd_line(app_name, cmd_line);
    }


    /*
     * cmd_line_provider<T>::cmd_line_provider
     */
    template<class T>
    cmd_line_provider<T>::cmd_line_provider(const string_type& app_name, const char_type *cmd_line) : use_argc(0), use_argv(nullptr), memory_argc(0), memory_argv() {
        THE_STACK_TRACE;
        this->memory_argv[0] = nullptr;
        this->memory_argv[1] = nullptr;
        this->create_cmd_line(app_name.c_str(), cmd_line);
    }


    /*
     * cmd_line_provider<T>::cmd_line_provider
     */
    template<class T>
    cmd_line_provider<T>::cmd_line_provider(const char_type *app_name, const string_type& cmd_line) : use_argc(0), use_argv(nullptr), memory_argc(0), memory_argv() {
        THE_STACK_TRACE;
        this->memory_argv[0] = nullptr;
        this->memory_argv[1] = nullptr;
        this->create_cmd_line(app_name, cmd_line.c_str());
    }


    /*
     * cmd_line_provider<T>::cmd_line_provider
     */
    template<class T>
    cmd_line_provider<T>::cmd_line_provider(const string_type& app_name, const string_type& cmd_line) : use_argc(0), use_argv(nullptr), memory_argc(0), memory_argv() {
        THE_STACK_TRACE;
        this->memory_argv[0] = nullptr;
        this->memory_argv[1] = nullptr;
        this->create_cmd_line(app_name.c_str(), cmd_line.c_str());
    }


#pragma warning(default: 4351)

    /*
     * cmd_line_provider<T>::~cmd_line_provider
     */
    template<class T>
    cmd_line_provider<T>::~cmd_line_provider(void) {
        THE_STACK_TRACE;
        this->clear_arg_list();
    }


    /*
     * cmd_line_provider<T>::create_cmd_line
     */
    template<class T>
    void cmd_line_provider<T>::create_cmd_line(const char_type *app_name, const char_type *cmd_line) {
        THE_STACK_TRACE;
        char_type *ci;
        char_type *start = nullptr;
        unsigned int state;

        // clear old argument list
        this->clear_arg_list();

        // count strings in command line
        this->memory_argc = (app_name == nullptr) ? 0 : 1;

        ci = const_cast<char_type *>(cmd_line);
        state = 1;
        while (state > 0) {
            switch (state) {
                case 0: break; // end of string
                case 1:
                    if (*ci == 0) { // end of string
                        state = 0;
                    } else if (*ci == static_cast<char_type>('"')) { // start of qouted parameter
                        this->memory_argc++; 
                        state = 3;
                    } else if (!text::char_utility::is_space(*ci)) { // start of parameter
                        this->memory_argc++; 
                        state = 2;
                    }
                    break;
                case 2:
                    if (*ci == 0) { // end of parameter
                        state = 0;
                    } else if (*ci == static_cast<char_type>('"')) { // start of qouted section
                        state = 3;
                    } else if (text::char_utility::is_space(*ci)) { // end of parameter
                        state = 1;
                    }
                    break;
                case 3:
                    if (*ci == 0) { // end of parameter
                        state = 0;
                    } else if (*ci == static_cast<char_type>('"')) { // possible end of qouted section/parameter
                        state = 4;
                    } 
                    break;
                case 4:
                    if (*ci == 0) { // end of parameter
                        state = 0;
                    } else if (*ci == static_cast<char_type>('"')) { // escaped qout!
                        state = 3;
                    } else if (text::char_utility::is_space(*ci)) { // end of parameter
                        state = 1; // truncate last space!
                    } else { // end of qouted section
                        state = 2;
                    }
                    break;
            }
            ci++;
        }

        // create argument list
        this->memory_argv[0] = new char_type*[this->memory_argc];
        this->memory_argv[1] = new char_type*[this->memory_argc];
        this->use_argv = this->memory_argv[1];

        if (app_name != nullptr) {
            size_t len = text::string_utility::c_str_len(app_name) + 1; // 1 because including the terminating zero
            this->memory_argv[0][0] = new char_type[len];
            ::memcpy(this->memory_argv[0][0], app_name, len * sizeof(char_type));
            this->use_argc = 1;
        } else {
            this->use_argc = 0;
        }

        ci = const_cast<char_type *>(cmd_line);
        state = 1;
        while (state > 0) {
            switch (state) {
                case 0: break; // end of string
                case 1:
                    if (*ci == 0) { // end of string
                        state = 0;
                    } else if (*ci == static_cast<char_type>('"')) { // start of qouted parameter
                        start = ci + 1;
                        state = 3;
                    } else if (!text::char_utility::is_space(*ci)) { // start of parameter
                        start = ci;
                        state = 2;
                    }
                    break;
                case 2:
                    if (*ci == 0) { // end of parameter
                        state = 0;
                        this->memory_argv[0][this->use_argc++] = this->create_arg(start, (ci - 1));
                    } else if (*ci == static_cast<char_type>('"')) { // start of qouted section
                        state = 3;
                    } else if (text::char_utility::is_space(*ci)) { // end of parameter
                        state = 1;
                        this->memory_argv[0][this->use_argc++] = this->create_arg(start, (ci - 1));
                    }
                    break;
                case 3:
                    if (*ci == 0) { // end of parameter
                        state = 0;
                        this->memory_argv[0][this->use_argc++] = this->create_arg(start, (ci - 1));
                    } else if (*ci == static_cast<char_type>('"')) { // possible end of qouted section/parameter
                        state = 4;
                    } 
                    break;
                case 4:
                    if (*ci == 0) { // end of parameter
                        state = 0;
                        this->memory_argv[0][this->use_argc++] = this->create_arg(start, (ci - 2));
                    } else if (*ci == static_cast<char_type>('"')) { // escaped qout!
                        state = 3;
                    } else if (text::char_utility::is_space(*ci)) { // end of parameter
                        state = 1; // truncate last space!
                        this->memory_argv[0][this->use_argc++] = this->create_arg(start, (ci - 2));
                    } else { // end of qouted section
                        state = 2;
                    }
                    break;
            }
            ci++;
        }

        THE_ASSERT(this->memory_argc == this->use_argc);

        for (this->use_argc = 0; this->use_argc < this->memory_argc; this->use_argc++) {
            this->use_argv[this->use_argc] = this->memory_argv[0][this->use_argc];
        }
    }


    /*
     * cmd_line_provider<T>::append
     */
    template<class T>
    void cmd_line_provider<T>::append(const char_type *arg) {
        THE_STACK_TRACE;
        this->reflect_caller_changes();
        
        // and now, because i know that memory anchor and active argument 
        // list are equal ...
        delete[] this->memory_argv[0];
        this->memory_argv[0] = new char_type*[this->memory_argc + 1];

        // copy old pointers
        ::memcpy(this->memory_argv[0], this->use_argv, sizeof(char_type*) * this->memory_argc);
        
        // copy new argument
        size_t len = text::string_utility::c_str_len(arg) + 1;
        this->memory_argv[0][this->memory_argc] = new char_type[len];
        this->memory_argv[0][this->memory_argc][0] = static_cast<char_type>(0);
        this->memory_argv[0][this->memory_argc][len - 1] = static_cast<char_type>(0);
        ::memcpy(this->memory_argv[0][this->memory_argc], arg, sizeof(char_type) * len);

        this->memory_argc++;

        // update active argument list
        delete[] this->memory_argv[1];
        this->memory_argv[1] = new char_type*[this->memory_argc + 1];
        this->arguments = this->memory_argv[1];
        ::memcpy(this->use_argv, this->memory_argv[0], sizeof(char_type*) * this->memory_argc);

        this->use_argc++;
    }


    /*
     * cmd_line_provider<T>::prepend
     */
    template<class T>
    void cmd_line_provider<T>::prepend(const char_type *arg) {
        THE_STACK_TRACE;
        this->reflect_caller_changes();
        
        // and now, because i know that memory anchor and active argument 
        // list are equal ...
        delete[] this->memory_argv[0];
        this->memory_argv[0] = new char_type*[this->memory_argc + 1];

        // copy old pointers
        ::memcpy(this->memory_argv[0] + 1, this->use_argv, sizeof(char_type*) * this->memory_argc);

        // copy new argument
        size_t len = text::string_utility::c_str_len(arg) + 1;
        this->memory_argv[0][0] = new char_type[len];
        this->memory_argv[0][0][0] = 0;
        this->memory_argv[0][0][len - 1] = 0;
        ::memcpy(this->memory_argv[0][0], arg, sizeof(char_type) * len);

        this->memory_argc++;

        // update active argument list
        delete[] this->memory_argv[1];
        this->memory_argv[1] = new char_type*[this->memory_argc + 1];
        this->use_argv = this->memory_argv[1];
        ::memcpy(this->use_argv, this->memory_argv[0], sizeof(char_type*) * this->memory_argc);

        this->use_argc++;
    }


    /*
     * cmd_line_provider<T>::single_string_cmd_line
     */
    template<class T>
    typename cmd_line_provider<T>::string_type cmd_line_provider<T>::single_string_cmd_line(bool include_first) {
        THE_STACK_TRACE;
        string_type retval;
        int start_index = include_first ? 0 : 1;

        if (this->use_argc > start_index) {
            // calculate length of string to be returned
            unsigned int len = this->use_argc - (1 + start_index); // separating spaces

            for (int i = start_index; i < this->use_argc; i++) {
                bool need_quots = (*this->use_argv[i] == 0);
                for (char_type *ci = this->use_argv[i]; *ci != 0; ci++) {
                    len++; // character
                    if (text::char_utility::is_space(*ci)) need_quots = true; // parameter with spaces
                    if (*ci == static_cast<char_type>('"')) {
                        need_quots = true;
                        len++; // escape qouts
                    }
                }
                if (need_quots) len += 2;
            }

            // build string
            char_type *data = new char_type[len];
            char_type *data_anchor = data;

            for (int i = start_index; i < this->use_argc; i++) {
                if (i > start_index) *(data++) = static_cast<char_type>(' '); // seperating space

                // remember start if quots are needed
                bool need_quots = (*this->use_argv[i] == 0);
                char_type *cs = data;

                for (char_type *ci = this->use_argv[i]; *ci != 0; ci++) {
                    *(data++) = *ci;

                    if ((text::char_utility::is_space(*ci)) || (*ci == static_cast<char_type>('"'))) {
                        // quots are needed!
                        need_quots = true;
                        break;
                    }
                }

                if (need_quots) {
                    data = cs;
                    *(data++) = static_cast<char_type>('"'); // starting quot
                    for (char_type *ci = this->use_argv[i]; *ci != 0; ci++) {
                        if (*ci == static_cast<char_type>('"')) { // escape quot
                            *(data++) = static_cast<char_type>('"');
                        }
                        *(data++) = *ci;
                    }
                    *(data++) = static_cast<char_type>('"'); // ending quot
                }
            }

            *(data++) = 0;

            retval = data_anchor;
    
        } else {
            // no arguments, so return empty string
            retval.clear();

        }

        return retval;
    }


    /*
     * cmd_line_provider<T>::operator =
     */
    template<class T>
    cmd_line_provider<T>& cmd_line_provider<T>::operator =(const cmd_line_provider& rhs) {
        THE_STACK_TRACE;
        if (this != &rhs) {
            this->clear_arg_list();

            this->memory_argc = rhs.memory_argc;
            this->use_argc = rhs.memory_argc; // Maybe restore the (changed) arguments

            this->memory_argv[0] = new char_type*[this->memory_argc];
            this->memory_argv[1] = new char_type*[this->memory_argc];
            this->use_argv = this->memory_argv[1];

            for (int i = 0; i < this->memory_argc; i++) {
                size_t len = text::string_utility::c_str_len(rhs.memory_argv[0][i]) + 1;

                this->memory_argv[0][i] = new char_type[len];
                ::memcpy(this->memory_argv[0][i], rhs.memory_argv[0][i], len * sizeof(char_type));

                this->use_argv[i] = this->memory_argv[0][i]; // Maybe restore the (changed) arguments
            }
        }
        return *this;
    }


    /*
     * cmd_line_provider<T>::single_string_cmd_line
     */
    template<class T>
    void cmd_line_provider<T>::clear_arg_list(void) {
        THE_STACK_TRACE;
        if (this->memory_argv[0] != NULL) {
            for (int i = 0; i < this->memory_argc; i++) {
                the::safe_array_delete(this->memory_argv[0][i]);
            }
            
            the::safe_array_delete(this->memory_argv[0]);
            the::safe_array_delete(this->memory_argv[1]);
            this->use_argv = NULL;
        }

        this->memory_argc = 0;
        this->use_argc = 0;
    }


    /*
     * cmd_line_provider<T>::single_string_cmd_line
     */
    template<class T>
    typename cmd_line_provider<T>::char_type *cmd_line_provider<T>::create_arg(char_type *left, char_type *right) {
        THE_STACK_TRACE;
        char_type *buf = NULL;
        size_t len = static_cast<size_t>(right - left) + 1;

        if (len <= 0) {
            buf = new char_type[1];
            buf[0] = static_cast<char_type>(0);
        } else {
            char_type *ci = buf = new char_type[len + 1];

            // copy characters individual to unescape quots
            for (ci = buf; left <= right; left++) {
                if ((*left != static_cast<char_type>('"'))
                        || (*(left - 1) != static_cast<char_type>('"'))) {
                    *(ci++) = *left;
                }
            }
            *ci = static_cast<char_type>(0);
        }        

        return buf;
    }


    /*
     * cmd_line_provider<T>::single_string_cmd_line
     */
    template<class T>
    void cmd_line_provider<T>::reflect_caller_changes(void) {
        THE_STACK_TRACE;

        // delete all entries in mV which are missing in aV
        for (int i = 0; i < this->memory_argc; i++) {
            bool found = false;
            for (int j = 0; j < this->use_argc; j++) {
                if (this->memory_argv[0][i] == this->use_argv[j]) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                delete[] this->memory_argv[0][i];
            }
        }

        // allocate new memory for the memory anchor
        if (this->memory_argc != this->use_argc) {
            delete[] this->memory_argv[0];
            this->memory_argc = this->use_argc;
            this->memory_argv[0] = new char_type*[this->storeCount];
        }

        // copy remaining elements
        for (int i = 0; i < this->memory_argc; i++) {
            this->memory_argv[0][i] = this->use_argv[i];
        }

        // refresh arguments / memoryAnchor[1]
        delete[] this->memory_argv[1];
        this->memory_argv[1] = new char_type*[this->memory_argc];
        this->use_argv = this->memory_argv[1];
        for (int i = 0; i < this->memory_argc; i++) {
            this->use_argv[i] = this->memory_argv[0][i];
        }

    }


    /** Template instantiation for ANSI strings. */
    typedef cmd_line_provider<char> cmd_line_provider_a;

    /** Template instantiation for wide strings. */
    typedef cmd_line_provider<wchar_t> cmd_line_provider_w;

    /** Template instantiation for TCHARs. */
    typedef cmd_line_provider<TCHAR> cmd_line_provider_t;

} /* end namespace system */
} /* end namespace the */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_CMD_LINE_PROVIDER_H_INCLUDED */
