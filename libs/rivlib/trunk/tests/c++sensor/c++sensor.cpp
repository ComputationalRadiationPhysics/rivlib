// c++sensor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "java_virtual_machine.h"
#include <jni.h>
#include <exception>
#include <time.h>


int _tmain(int argc, _TCHAR* argv[]) {
    try {
        java_virtual_machine jvm;

        jclass conncfg_type = jvm.find_class("eu/vicci/semiwa/device/publisher/SeMiWaPublisherConnectionConfiguration");
        jmethodID conncfg_ctor = jvm.find_method(conncfg_type, "<init>", 
            "(Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

        jobject connCfg = jvm.new_object(conncfg_type, conncfg_ctor,
            jvm.str("192.168.1.100"), // host
            static_cast<jint>(5222), // port
            jvm.str("SGCGUID++"), // uid; TODO: this should be unique within the whole system.
            jvm.str("C++ Test Sensor"), // name
            jvm.str("Sensor"), // device type
            jvm.str("useless debug data"), // value type
            jvm.str("xsd:string"), // unit
            jvm.str("")); // capabilities

        jclass conn_type = jvm.find_class("eu/vicci/semiwa/device/publisher/SeMiWaPublisherConnection");
        jmethodID conn_ctor = jvm.find_method(conn_type, "<init>", "(Leu/vicci/semiwa/device/publisher/SeMiWaPublisherConnectionConfiguration;)V");

        jobject conn = jvm.new_object(conn_type, conn_ctor, connCfg);

        // we could register capabilities here ...
        jmethodID conn_sendSensorRegistered = jvm.find_method(conn_type, "sendSensorRegistered", "()V");
        jmethodID conn_sendSensorUnregistered = jvm.find_method(conn_type, "sendSensorUnregistered", "()V");
        jmethodID conn_sendSensorData = jvm.find_method(conn_type, "sendSensorData", "(Ljava/lang/String;)V");

        jvm.call_void_method(conn, conn_sendSensorRegistered);

        ::Sleep(100);

        char str[1024];
        ::srand((unsigned)time(NULL));

        for (int i = 0; i < 100; i++) {

            sprintf_s(str, 1024, "%d:%d", i, ::rand());

            jstring jstr = jvm.str(str);

            jvm.call_void_method(conn, conn_sendSensorData, jstr);

            jvm.delete_local_ref(jstr);

            ::Sleep(2000);

        }

        jvm.call_void_method(conn, conn_sendSensorUnregistered);

    } catch(...) {
        // it did not work
        return -1;
    }

    return 0;
}

