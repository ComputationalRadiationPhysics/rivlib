/*
 * java_vm.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "jni/java_vm.h"

#if defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0)

#include "the/assert.h"
#include "the/exception.h"
#include "the/text/string_builder.h"

using namespace eu_vicci::rivlib;


/*
 * java_vm::java_vm
 */
java_vm::java_vm(const java_vm_config& cfg) : jvm(nullptr), env(nullptr) {
    JavaVMInitArgs args;
    JavaVMOption options[2];

    args.version = JNI_VERSION_1_6;

    the::text::astring_builder class_path;
    size_t cnt = cfg.get_class_paths().size();
    for (size_t i = 0; i < cnt; i++) {
        class_path.append(cfg.get_class_paths()[i]);
        class_path.append(";");
    }
    class_path.prepend("-Djava.class.path=");

    std::string class_paths(class_path.to_string());

    args.nOptions = 1;
    options[0].optionString = const_cast<char*>(class_paths.c_str());
    args.options = options;

    // http://docs.oracle.com/javase/1.4.2/docs/guide/jni/jni-12.html#JNI_CreateJavaVM
    args.ignoreUnrecognized = JNI_TRUE;

    JNI_CreateJavaVM(&this->jvm, (void **)&this->env, &args);
    if (env == NULL) {
        // java vm did not start
        throw the::exception("Unable to start JNI VM", __FILE__, __LINE__);
    }

}


/*
 * java_vm::~java_vm
 */
java_vm::~java_vm(void) {
    THE_ASSERT(this->jvm != nullptr);
    this->jvm->DestroyJavaVM();
    this->jvm = nullptr; // do not further delete the jvm object!
    this->env = nullptr;
}


/*
 * java_vm::find_class
 */
jclass java_vm::find_class(const char *name, bool exception_on_error) {
    this->env->ExceptionClear();

    jclass c = this->env->FindClass(name);

    jthrowable ex = env->ExceptionOccurred();
    if (ex != NULL) {
        if (exception_on_error) {
            std::string msg(this->fetch_exception_string(ex));
            this->env->DeleteLocalRef(ex);
            this->env->ExceptionClear();
            throw the::exception(msg.c_str(), __FILE__, __LINE__);
        } else {
            c = nullptr;
        }
    }

    return c;
}


/*
 * java_vm::find_method
 */
jmethodID java_vm::find_method(jclass type, const char *name, const char *sign, bool exception_on_error) {
    this->env->ExceptionClear();

    jmethodID m = this->env->GetMethodID(type, name, sign);

    jthrowable ex = env->ExceptionOccurred();
    if (ex != NULL) {
        if (exception_on_error) {
            std::string msg(this->fetch_exception_string(ex));
            this->env->DeleteLocalRef(ex);
            this->env->ExceptionClear();
            throw the::exception(msg.c_str(), __FILE__, __LINE__);
        } else {
            m = nullptr;
        }
    }

    return m;
}


/*
 * java_vm::str
 */
jstring java_vm::str(const char *s) {
    if (s == nullptr) return NULL;
    bool isASCII7 = true;
    for (const char *c = s; *c != '\0'; c++) {
        if (*c >= 128) {
            isASCII7 = false;
            break;
        }
    }
    if (isASCII7) {
        return this->env->NewStringUTF(s);
    } else {
        return this->str(the::text::string_converter::to_w(s));
    }
}


/*
 * java_vm::str
 */
jstring java_vm::str(const std::string& s) {
    return this->str(s.c_str());
}


/*
 * java_vm::str
 */
jstring java_vm::str(const wchar_t *s) {
    return this->str(std::wstring(s));
}


/*
 * java_vm::str
 */
jstring java_vm::str(const std::wstring& s) {
#ifdef _WIN32
    return this->env->NewString((const jchar*)s.c_str(), static_cast<jsize>(s.size()));
#else /* _WIN32 */
    std::string xs;
    the::text::string_converter::convert(xs, s);
    size_t len = xs.size();
    for (size_t i = 0; i < len; i++) {
        if (xs[i] >= 128u) {
            xs[i] = '?';
        }
    }
#endif /* _WIN32 */
}


/*
 * java_vm::delete_local_ref
 */
void java_vm::delete_local_ref(jobject o) {
    this->env->ExceptionClear();

    this->env->DeleteLocalRef(o);

    jthrowable ex = env->ExceptionOccurred();
    if (ex != NULL) {
        std::string msg(this->fetch_exception_string(ex));
        this->env->DeleteLocalRef(ex);
        this->env->ExceptionClear();
        throw the::exception(msg.c_str(), __FILE__, __LINE__);
    }
}


/*
 * java_vm::call_void_method
 */
void java_vm::call_void_method(jobject obj, jmethodID name, ...) {
    this->env->ExceptionClear();

    va_list args;
    va_start(args, name);

    this->env->CallVoidMethodV(obj, name, args);

    va_end(args);

    jthrowable ex = env->ExceptionOccurred();
    if (ex != NULL) {
        std::string msg(this->fetch_exception_string(ex));
        this->env->DeleteLocalRef(ex);
        this->env->ExceptionClear();
        throw the::exception(msg.c_str(), __FILE__, __LINE__);
    }
}


/*
 * java_vm::new_object
 */
jobject java_vm::new_object(jclass type, jmethodID ctor, ...) {
    this->env->ExceptionClear();

    va_list args;
    va_start(args, ctor);

    jobject o = this->env->NewObjectV(type, ctor, args);

    va_end(args);

    jthrowable ex = env->ExceptionOccurred();
    if (ex != NULL) {
        std::string msg(this->fetch_exception_string(ex));
        this->env->DeleteLocalRef(ex);
        this->env->ExceptionClear();
        throw the::exception(msg.c_str(), __FILE__, __LINE__);
    }
    if (o == NULL) {
        throw the::exception("failed to create object", __FILE__, __LINE__);
    }

    return o;
}


/*
 * java_vm::fetch_exception_string
 */
std::string java_vm::fetch_exception_string(jthrowable ex) throw() {
    jclass c = NULL;
    jmethodID ts = NULL;
    jstring str = NULL;
    std::string msg = "Unknown exception";

    try {
        c = this->env->GetObjectClass(ex);
        if (c == NULL) return msg;
        ts = this->env->GetMethodID(c, "getMessage", "()Ljava/lang/String;");
        if (ts == NULL) return msg;
        jstring str = (jstring)this->env->CallObjectMethod(ex, ts);
        if (str == NULL) return msg;

#ifdef _WIN32
        const jchar *str_chars = NULL;
#else /* _WIN32 */
        const char *str_chars = NULL;
#endif /* _WIN32 */
        jsize str_len = 0;
        try {
#ifdef _WIN32
            str_chars = this->env->GetStringChars(str, NULL);
            jsize str_len = this->env->GetStringLength(str);
#else /* _WIN32 */
            str_chars = this->env->GetStringUTFChars(str, NULL);
            str_len = this->env->GetStringUTFLength(str);
#endif /* _WIN32 */

            if ((str_chars != NULL) && (str_len > 0)) {

#ifdef _WIN32
                the::text::string_converter::convert(msg, std::wstring((const wchar_t*)str_chars, str_len));
#else /* _WIN32 */
                msg = std::string(str_chars, str_len);
#endif /* _WIN32 */
                // TODO: Implementy

            }

            if (str_chars != NULL) {
#ifdef _WIN32
                this->env->ReleaseStringChars(str, str_chars);
#else /* _WIN32 */
                this->env->ReleaseStringUTFChars(str, str_chars);
#endif /* _WIN32 */
                str_chars = NULL;
            }
        } catch(...) {
            try {
                if (str_chars != NULL) {
#ifdef _WIN32
                    this->env->ReleaseStringChars(str, str_chars);
#else /* _WIN32 */
                    this->env->ReleaseStringUTFChars(str, str_chars);
#endif /* _WIN32 */
                    str_chars = NULL;
                }
            } catch(...) {
            }
            throw;
        }

        ts = NULL;
        this->env->DeleteLocalRef(c);
        c = NULL;

        this->env->DeleteLocalRef(str);
        str = NULL;

    } catch(...) {
    }

    if (c != NULL) {
        try {
            this->env->DeleteLocalRef(c);
        } catch(...) {
        }
    }
    //if (ts != NULL) {
    //    try {
    //        this->env->DeleteLocalRef(ts); // because ts is not an object
    //    } catch(...) {
    //    }
    //}
    if (str != NULL) {
        try {
            this->env->DeleteLocalRef(str);
        } catch(...) {
        }
    }

    return msg;
}

#endif /* defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0) */
