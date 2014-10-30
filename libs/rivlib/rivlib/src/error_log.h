/*
 * rivlib
 * error_log.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_ERROR_LOG_H_INCLUDED
#define VICCI_RIVLIB_ERROR_LOG_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include <string>
#include <stdarg.h>
#include "the/string.h"
#include "the/system/threading/critical_section.h"


namespace eu_vicci {
namespace rivlib {

    /**
     * Simple error log facade
     */
    class error_log {
    public:

        /** possible message types */
        enum class msg_type {
            error,
            warning,
            info
        };

        /** The null_log object not to be used */
        static error_log null_log;

        /**
         * Ctor
         *
         * @param enable The enable flag
         */
        error_log(bool enable = false);

        /** Dtor */
        ~error_log(void);

        /**
         * Writes a log message
         *
         * @param t The log message type
         * @param msg The log message
         */
        inline void message(msg_type t, const char* msg, ...) {
            va_list args;
            va_start(args, msg);
            this->message_va(t, msg, args);
            va_end(args);
        }

        /**
         * Writes a log message
         *
         * @param t The log message type
         * @param msg The log message
         */
        inline void message(msg_type t, const wchar_t* msg, ...) {
            va_list args;
            va_start(args, msg);
            this->message_va(t, msg, args);
            va_end(args);
        }

        /**
         * Writes an error log message
         *
         * @param msg The log message
         */
        inline void error(const char* msg, ...) {
            va_list args;
            va_start(args, msg);
            this->message_va(msg_type::error, msg, args);
            va_end(args);
        }

        /**
         * Writes an error log message
         *
         * @param msg The log message
         */
        inline void error(const wchar_t* msg, ...) {
            va_list args;
            va_start(args, msg);
            this->message_va(msg_type::error, msg, args);
            va_end(args);
        }

        /**
         * Writes a warning log message
         *
         * @param msg The log message
         */
        inline void warn(const char* msg, ...) {
            va_list args;
            va_start(args, msg);
            this->message_va(msg_type::warning, msg, args);
            va_end(args);
        }

        /**
         * Writes a warning log message
         *
         * @param msg The log message
         */
        inline void warn(const wchar_t* msg, ...) {
            va_list args;
            va_start(args, msg);
            this->message_va(msg_type::warning, msg, args);
            va_end(args);
        }

        /**
         * Writes a info log message
         *
         * @param msg The log message
         */
        inline void info(const char* msg, ...) {
            va_list args;
            va_start(args, msg);
            this->message_va(msg_type::info, msg, args);
            va_end(args);
        }

        /**
         * Writes a info log message
         *
         * @param msg The log message
         */
        inline void info(const wchar_t* msg, ...) {
            va_list args;
            va_start(args, msg);
            this->message_va(msg_type::info, msg, args);
            va_end(args);
        }

    private:

        /** The message string type */
        typedef the::astring msg_string_type;

        /**
         * message
         */
        class msg {
        public:

            /** Ctor */
            msg(void);

            /**
             * Copy ctor
             *
             * @param src The object to clone from
             */
            msg(const msg& src);

            /** Dtor */
            ~msg(void);

            /**
             * Assignment operator
             *
             * @param rhs The right hand side operand
             *
             * @return A reference to this
             */
            msg& operator=(const msg& rhs);

            /**
             * Test for equality
             *
             * @param rhs The right hand side operand
             *
             * @return true if this and rhs are equal
             */
            bool operator==(const msg& rhs) const;

            /**
             * Fixes the message text
             */
            void fix_text(void);

            /**
             * Sets the current thread id
             */
            void set_thread_id(void);

            /**
             * Sets 'now' as time stamp
             */
            void set_time_stamp(void);

            /**
             * Prints the message to a stream
             *
             * @param stream The stream to write to
             */
            void print(FILE *stream);

            /** the message type */
            msg_type type;

            /** the message text */
            msg_string_type text;

            /** the id of the tread */
            msg_string_type thread_id;

            /** the message time_stamp */
            msg_string_type time_stamp;

        };

        /**
         * Writes a log message
         *
         * @param t The log message type
         * @param msg The log message
         * @param agrs The message string format arguments
         */
        void message_va(msg_type t, const char* msg, va_list args);

        /**
         * Writes a log message
         *
         * @param t The log message type
         * @param msg The log message
         * @param agrs The message string format arguments
         */
        void message_va(msg_type t, const wchar_t* msg, va_list args);

        /** the enable flag */
        bool enable;

        /** The thread-lock object */
        the::system::threading::critical_section lock_obj;

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */


#endif /* VICCI_RIVLIB_ERROR_LOG_H_INCLUDED */
