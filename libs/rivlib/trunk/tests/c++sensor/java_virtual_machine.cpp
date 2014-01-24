#include "stdafx.h"
#include "java_virtual_machine.h"
#include <exception>


/*
 * java_virtual_machine::java_virtual_machine
 */
java_virtual_machine::java_virtual_machine(void) {
    JavaVMInitArgs args;
    JavaVMOption options[2];

    // TODO: THIS IS CRAP: Make sources configurable! But, how?

    args.version = JNI_VERSION_1_6;

    args.nOptions = 1;
    options[0].optionString = "-Djava.class.path="
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.device.publisher\\bin;"
        //"D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib;"
        //"D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\;"
        //"D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\*;"
        //"D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\*.*;"
        //"D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\*.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\commons-codec-1.5.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\gson-2.2.2.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\httpclient-4.1.2.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\httpcore-4.1.3.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\jcl-over-slf4j-1.6.4.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\jena-arq-2.9.4.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\jena-core-2.7.4.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\jena-iri-0.9.4.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\jena-tdb-0.9.4.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\log4j-1.2.16.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\slf4j-api-1.6.4.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\slf4j-log4j12-1.6.4.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\smack.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\smackx-debug.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\smackx.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\xercesImpl-2.10.0.jar;"
        "D:\\dev\\android\\workspace\\eu.vicci.semiwa.libs\\lib\\xml-apis-1.4.01.jar;"
        //"D:\\dev\\android\\workspace\\SimpleDemo\\bin;"
        ;
    args.options = options;

    // http://docs.oracle.com/javase/1.4.2/docs/guide/jni/jni-12.html#JNI_CreateJavaVM
    args.ignoreUnrecognized = JNI_TRUE;

    JNI_CreateJavaVM(&jvm, (void **)&env, &args);
    if (env == NULL) {
        // java vm did not start
        throw std::exception(__FILE__, __LINE__);
    }
}


/*
 * java_virtual_machine::~java_virtual_machine
 */
java_virtual_machine::~java_virtual_machine(void) {
    this->jvm->DestroyJavaVM();
    this->jvm = NULL;
    this->env = NULL;
}


/*
 * java_virtual_machine::find_class
 */
jclass java_virtual_machine::find_class(const char *name, bool exception_on_error) {
    this->env->ExceptionClear();
    jclass c = this->env->FindClass(name);
    if (((this->env->ExceptionOccurred() != NULL) || (c == NULL)) && exception_on_error) {
        this->env->ExceptionDescribe(); 
        throw std::exception();
    } else if (this->env->ExceptionOccurred() != NULL) {
        c = NULL;
    }

    return c;
}


/*
 * java_virtual_machine::find_method
 */
jmethodID java_virtual_machine::find_method(jclass type, const char *name, const char *sign, bool exception_on_error) {
    this->env->ExceptionClear();
    jmethodID m = this->env->GetMethodID(type, name, sign);
    if (((this->env->ExceptionOccurred() != NULL) || (m == NULL)) && exception_on_error) {
        this->env->ExceptionDescribe(); 
        throw std::exception();
    } else if (this->env->ExceptionOccurred() != NULL) {
        m = NULL;
    }
    return m;
}


/*
 * java_virtual_machine::str
 */
jstring java_virtual_machine::str(const char *utf8) {
    for (const char *c = utf8; ((c != NULL) && (*c != 0)); c++) {
        if (static_cast<unsigned char>(*c) >= 128u) {
            throw std::exception();
        }
    }
    return this->env->NewStringUTF(utf8);
}


/*
 * java_virtual_machine::delete_local_ref
 */
void java_virtual_machine::delete_local_ref(jobject o) {
    this->env->ExceptionClear();
    this->env->DeleteLocalRef(o);
    if (this->env->ExceptionOccurred() != NULL) {
        this->env->ExceptionDescribe(); 
        throw std::exception();
    }
}

//
///*
// * java_virtual_machine::delete_global_ref
// */
//void java_virtual_machine::delete_global_ref(jobject o) {
//    this->env->ExceptionClear();
//    this->env->DeleteLocalRef(o);
//    if (this->env->ExceptionOccurred() != NULL) {
//        this->env->ExceptionDescribe(); 
//        throw std::exception();
//    }
//}
//

/*
 * java_virtual_machine::call_void_method
 */
void java_virtual_machine::call_void_method(jobject obj, jmethodID name, ...) {
    this->env->ExceptionClear();

    va_list args;
    va_start(args, name);

    this->env->CallVoidMethodV(obj, name, args);

    va_end(args);

    if (this->env->ExceptionOccurred() != NULL) {
        this->env->ExceptionDescribe(); 
        throw std::exception();
    }
}


/*
 * java_virtual_machine::new_object
 */
jobject java_virtual_machine::new_object(jclass type, jmethodID ctor, ...) {
    this->env->ExceptionClear();

    va_list args;
    va_start(args, ctor);

    jobject o = this->env->NewObjectV(type, ctor, args);

    va_end(args);

    if (this->env->ExceptionOccurred() != NULL) {
        this->env->ExceptionDescribe(); 
        throw std::exception();
    }
    if (o == NULL) {
        throw std::exception();
    }

    return o;
}
