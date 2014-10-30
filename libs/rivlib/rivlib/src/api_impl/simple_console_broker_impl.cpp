/*
 * simple_console_broker_impl.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "api_impl/simple_console_broker_impl.h"

using namespace eu_vicci::rivlib;


/*
 * simple_console_broker_impl::simple_console_broker_impl
 */
simple_console_broker_impl::simple_console_broker_impl(void)
        : simple_console_broker(), abstract_queued_broker() {
    // intentionally empty
}


/*
 * simple_console_broker_impl::~simple_console_broker_impl
 */
simple_console_broker_impl::~simple_console_broker_impl(void) {
    // intentionally empty
}


/*
 * simple_console_broker_impl::init
 */
void simple_console_broker_impl::init(void) {
    // intentionally empty
}



/*
 * simple_console_broker_impl::deinit
 */
void simple_console_broker_impl::deinit(void) {
    // intentionally empty
}



/*
 * simple_console_broker_impl::activate
 */
void simple_console_broker_impl::activate(communicator::ptr comm, provider::ptr prov) {
    printf("publishing %s\n", prov->get_name_astr());
    size_t puc = comm->count_public_uris(prov);
    for (size_t i = 0; i < puc; i++) {
        size_t l = comm->public_uri(prov, i, nullptr, 0);
        char *c = new char[l + 1];
        c[l] = 0;
        size_t l2 = comm->public_uri(prov, i, c, l);
        c[l2] = 0;
        printf("\t%s\n", c);
        delete[] c;
    }

}



/*
 * simple_console_broker_impl::deactivate
 */
void simple_console_broker_impl::deactivate(communicator::ptr comm, provider::ptr prov) {
    printf("stopping %s\n", prov->get_name_astr());
}
