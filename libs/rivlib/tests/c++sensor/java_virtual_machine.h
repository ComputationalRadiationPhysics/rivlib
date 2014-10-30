#pragma once

#include <jni.h>


class java_virtual_machine {
public:
    java_virtual_machine(void);
    ~java_virtual_machine(void);
    jclass find_class(const char *name, bool exception_on_error = true);
    jmethodID find_method(jclass type, const char *name, const char *sign, bool exception_on_error = true);
    jstring str(const char *utf8 /* warning: This is modified UTF8; Not standard! Use NewString instead! */);
    inline JNIEnv* get_env() const {
        return this->env;
    }
    void delete_local_ref(jobject o);
    //void delete_global_ref(jobject o);
    void call_void_method(jobject obj, jmethodID name, ...);
    jobject new_object(jclass type, jmethodID ctor, ...);
private:
    JavaVM* jvm;
    JNIEnv* env;
};

