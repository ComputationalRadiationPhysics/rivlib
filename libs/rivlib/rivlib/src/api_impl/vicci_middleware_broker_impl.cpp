/*
 * vicci_middleware_broker_impl.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "api_impl/vicci_middleware_broker_impl.h"

#if defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0)

#include "the/string.h"
#include "the/text/string_builder.h"
#include "the/text/string_converter.h"
#include "vislib/DirectoryIterator.h"
#include "vislib/File.h"
#include "vislib/Path.h"

using namespace eu_vicci::rivlib;


/*
 * vicci_middleware_broker_impl::vicci_middleware_broker_impl
 */
vicci_middleware_broker_impl::vicci_middleware_broker_impl(void)
        : vicci_middleware_broker(), abstract_queued_broker(), cfg(),
        vm(nullptr), host(DEFAULT_VICCI_HOST), port(DEFAULT_VICCI_HOST_PORT),
        conncfg_type(nullptr), conncfg_ctor(nullptr), conn_type(nullptr),
        conn_ctor(nullptr), conn_sendSensorRegistered(nullptr),
        conn_sendSensorUnregistered(nullptr), conn_sendSensorData(nullptr),
        connections() {
    // intentionally empty
}


/*
 * vicci_middleware_broker_impl::~vicci_middleware_broker_impl
 */
vicci_middleware_broker_impl::~vicci_middleware_broker_impl(void) {
    if (this->vm != nullptr) {
        this->deinit();
    }
}


/*
 * vicci_middleware_broker_impl::clear_java_class_paths
 */
void vicci_middleware_broker_impl::clear_java_class_paths(void) {
    this->cfg.clear_class_paths();
}


/*
 * vicci_middleware_broker_impl::add_java_class_path
 */
void vicci_middleware_broker_impl::add_java_class_path(const char *path) {
    if (vislib::sys::File::Exists(path)) {
        this->cfg.add_class_path(path);
    } else {
        vislib::StringA dir = vislib::sys::Path::GetDirectoryName(path);
        vislib::sys::DirectoryIteratorA diter(path, true, false);
        while (diter.HasNext()) {
            this->cfg.add_class_path(vislib::sys::Path::Resolve(diter.Next().Path, dir).PeekBuffer());
        }
    }
}


/*
 * vicci_middleware_broker_impl::add_java_class_path
 */
void vicci_middleware_broker_impl::add_java_class_path(const wchar_t *path) {
    if (vislib::sys::File::Exists(path)) {
        std::string p;
        the::text::string_converter::convert(p, path);
        this->cfg.add_class_path(p);
    } else {
        vislib::StringW dir = vislib::sys::Path::GetDirectoryName(path);
        vislib::sys::DirectoryIteratorW diter(path, true, false);
        while (diter.HasNext()) {
            std::string p;
            the::text::string_converter::convert(p, vislib::sys::Path::Resolve(diter.Next().Path, dir).PeekBuffer());
            this->cfg.add_class_path(p);
        }
    }
}


/*
 * vicci_middleware_broker_impl::set_vicci_semiwa_host
 */
void vicci_middleware_broker_impl::set_vicci_semiwa_host(const char *host, unsigned short port) {
    this->host = host;
    this->port = port;
}


/*
 * vicci_middleware_broker_impl::init
 */
void vicci_middleware_broker_impl::init(void) {
    if (this->vm == nullptr) {
        this->vm = new java_vm(this->cfg);

        try {
            this->conncfg_type = this->vm->find_class("eu/vicci/semiwa/device/publisher/SeMiWaPublisherConnectionConfiguration");
            this->conncfg_ctor = this->vm->find_method(conncfg_type, "<init>", 
                "(Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

            this->conn_type = this->vm->find_class("eu/vicci/semiwa/device/publisher/SeMiWaPublisherConnection");
            this->conn_ctor = this->vm->find_method(conn_type, "<init>", "(Leu/vicci/semiwa/device/publisher/SeMiWaPublisherConnectionConfiguration;)V");

            this->conn_sendSensorRegistered = this->vm->find_method(conn_type, "sendSensorRegistered", "()V");
            this->conn_sendSensorUnregistered = this->vm->find_method(conn_type, "sendSensorUnregistered", "()V");
            this->conn_sendSensorData = this->vm->find_method(conn_type, "sendSensorData", "(Ljava/lang/String;)V");

        } catch(...) {
            this->conncfg_type = nullptr;
            this->conncfg_ctor = nullptr;
            this->conn_type = nullptr;
            this->conn_ctor = nullptr;
            this->conn_sendSensorRegistered = nullptr;
            this->conn_sendSensorUnregistered = nullptr;
            this->conn_sendSensorData = nullptr;
            try {
                the::safe_delete(this->vm);
            } catch(...) {
            }

            throw;
        }
    }
}


/*
 * vicci_middleware_broker_impl::deinit
 */
void vicci_middleware_broker_impl::deinit(void) {
    if (this->vm != nullptr) {

        connection_map_type::iterator i = this->connections.begin();
        connection_map_type::iterator e = this->connections.end();
        for (;i != e; ++i) {
            try {
                this->vm->call_void_method(i->second, this->conn_sendSensorUnregistered);
                this->vm->delete_local_ref(i->second);
            } catch(...) {
            }
        }
        this->connections.clear();

        this->vm->delete_local_ref(this->conncfg_type);
        this->conncfg_type = nullptr;
        this->conncfg_ctor = nullptr;
        this->vm->delete_local_ref(this->conn_type);
        this->conn_type = nullptr;
        this->conn_ctor = nullptr;
        this->conn_sendSensorRegistered = nullptr;
        this->conn_sendSensorUnregistered = nullptr;
        this->conn_sendSensorData = nullptr;
        the::safe_delete(this->vm);
    }
}


/*
 * vicci_middleware_broker_impl::activate
 */
void vicci_middleware_broker_impl::activate(communicator::ptr comm, provider::ptr prov) {
    connection_pair_type s(comm, prov);
    if (this->connections.find(s) == this->connections.end()) {

        the::text::astring_builder value;

        size_t puc = comm->count_public_uris(prov);
        for (size_t i = 0; i < puc; i++) {
            size_t l = comm->public_uri(prov, i, nullptr, 0);
            char *c = new char[l + 1];
            c[l] = 0;
            size_t l2 = comm->public_uri(prov, i, c, l);
            c[l2] = 0;
            if (i != 0) value.append(";");
            value.append(c, l2);
            delete[] c;
        }

        jobject connCfg = this->vm->new_object(conncfg_type, conncfg_ctor,
            this->vm->str(this->host), // host
            static_cast<jint>(this->port), // port
            // uid; this must be unique within the whole system.
            // using url_encode to avoid spaces and special characters
            this->vm->str(the::text::string_utility::url_encode(prov->get_name_wstr())),
            this->vm->str("RIV Provider"), // name
            this->vm->str("Sensor"), // device type
            this->vm->str("URIs"), // value type
            this->vm->str("xsd:string"), // unit
            this->vm->str("")); // capabilities

        jobject conn = this->vm->new_object(conn_type, conn_ctor, connCfg);
        // we could register capabilities here ...

        this->vm->delete_local_ref(connCfg);

        this->vm->call_void_method(conn, conn_sendSensorRegistered);
        this->connections[s] = conn;

        jstring jvalstr = this->vm->str(value.to_string());
        this->vm->call_void_method(conn, conn_sendSensorData, jvalstr);
        this->vm->delete_local_ref(jvalstr);
    }

}


/*
 * vicci_middleware_broker_impl::deactivate
 */
void vicci_middleware_broker_impl::deactivate(communicator::ptr comm, provider::ptr prov) {
    connection_pair_type s(comm, prov);
    connection_map_type::iterator i = this->connections.find(s);
    if (i != this->connections.end()) {

        this->vm->call_void_method(i->second, this->conn_sendSensorUnregistered);
        this->vm->delete_local_ref(i->second);

        this->connections.erase(i);
    }
}

#endif /* defined(VICCI_RIVLIB_WITH_JNI) && (VICCI_RIVLIB_WITH_JNI != 0) */
