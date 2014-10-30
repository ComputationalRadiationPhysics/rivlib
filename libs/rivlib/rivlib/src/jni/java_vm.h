/*
 * java_vm.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_JAVA_VM_H_INCLUDED
#define VICCI_RIVLIB_JAVA_VM_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "rivlib/common.h"

#if defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0)

#include <jni.h>
#include "jni/java_vm_config.h"


namespace eu_vicci {
namespace rivlib {

    /**
     * Implementation of the core class
     */
    class java_vm {
    public:

        /**
         * ctor
         *
         * @param cfg The configuration for the virtual machine
         */
        java_vm(const java_vm_config& cfg);

        /** dtor */
        ~java_vm(void);

        /**
         * Finds a java class with the specified name in this java vm
         * environment
         *
         * @paran name The name of the class to find
         * @param exception_on_error If set to true, an exception is thrown
         *            when the specified class is not found. If the value is
         *            false, a return value of 'nullptr' indicates the error.
         *
         * @return The found java class
         */
        jclass find_class(const char *name, bool exception_on_error = true);

        /**
         * Finds a method within a java class
         *
         * @remarks For further information on java method signatures consult
         *     the JNI api documentation or web resources, e.g.
         *     http://journals.ecs.soton.ac.uk/java/tutorial/native1.1/implementing/method.html
         * @param type The java class
         * @param name The name of the method to find
         * @param sign The method signature
         * @param exception_on_error If set to true, an exception is thrown
         *            when the specified class is not found. If the value is
         *            false, a return value of 'nullptr' indicates the error.
         *
         * @return The found method id
         */
        jmethodID find_method(jclass type, const char *name, const char *sign, bool exception_on_error = true);

        /**
         * Converts a string to a java string
         *
         * @param s The input string (zero terminated)
         *
         * @return The new java string object
         */
        jstring str(const char *s);

        /**
         * Converts a string to a java string
         *
         * @param s The input string
         *
         * @return The new java string object
         */
        jstring str(const std::string& s);

        /**
         * Converts a string to a java string
         *
         * @param s The input string (zero terminated)
         *
         * @return The new java string object
         */
        jstring str(const wchar_t *s);

        /**
         * Converts a string to a java string
         *
         * @param s The input string
         *
         * @return The new java string object
         */
        jstring str(const std::wstring& s);

        /**
         * Answer the java vm environment
         *
         * @return The java vm environment
         */
        inline JNIEnv* get_env() const {
            return this->env;
        }

        /**
         * Deletes a local reference to an object and deletes the object
         * itself as soon as the last reference is removed
         *
         * @param o The java object to be deleted
         */
        void delete_local_ref(jobject o);

        /**
         * Calls the method 'meth' on the object 'obj'. The method must not
         * have a return value
         *
         * @param obj The object to call the method on
         * @param meth The method ide of the method to call
         */
        void call_void_method(jobject obj, jmethodID meth, ...);

        /**
         * Creates a new object instance of the specified type
         *
         * @param type The class to create an object of
         * @param ctor The method id of the ctor to call
         */
        jobject new_object(jclass type, jmethodID ctor, ...);

    private:

        /**
         * Collects the error string of the current exception
         *
         * @param ex The current exception
         *
         * @return The error string of the current exception
         */
        std::string fetch_exception_string(jthrowable ex) throw();

        /** The java virtual machine */
        JavaVM* jvm;

        /** The java vm environment */
        JNIEnv* env;

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */

#endif /* defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0) */

#endif /* VICCI_RIVLIB_JAVA_VM_H_INCLUDED */
