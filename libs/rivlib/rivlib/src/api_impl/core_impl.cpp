/*
 * core_impl.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "api_impl/core_impl.h"
//#include "the/assert.h"
//#include <algorithm>
//#include "the/argument_exception.h"
//#include "the/string.h"
//#include "the/text/string_utility.h"

using namespace eu_vicci::rivlib;


/*
 * core_impl::core_impl
 */
core_impl::core_impl(void) : core(), node(), lock_obj(), log_obj(true) {
    // intentionally empty
}


/*
 * core_impl::~core_impl
 */
core_impl::~core_impl(void) {
    this->shutdown();
    this->disconnect_all();
}


/*
 * core_impl::add_broker
 */
void core_impl::add_broker(broker::ptr broker) {
    this->connect(broker);
    // do not handle connection here. Instead us the 'connecting' and 'connected' events.
}


/*
 * core_impl::remove_broker
 */
void core_impl::remove_broker(broker::ptr broker) {
    // do not handle connection here. Instead us the 'disconnecting' and 'disconnected' events.
    this->disconnect(broker);
}


/*
 * core_impl::remove_all_brokers
 */
void core_impl::remove_all_brokers(void) {
    // do not handle connection here. Instead us the 'disconnecting' and 'disconnected' events.
    std::vector<api_ptr_base> peers = this->select<broker>();
    size_t peer_cnt = peers.size();
    for (size_t i = 0; i < peer_cnt; ++i) {
        this->disconnect(peers[i]);
    }
}


/*
 * core_impl::enumerate_brokers
 */
void core_impl::enumerate_brokers(broker_enumerator enumerator, void *ctxt) {
    std::vector<api_ptr_base> peers = this->select<broker>();
    size_t peer_cnt = peers.size();
    for (size_t i = 0; i < peer_cnt; ++i) {
        broker::ptr* p = dynamic_cast<broker::ptr*>(&peers[i]);
        if (p == nullptr) continue;
        if (!enumerator(*p, ctxt)) break;
    }
}


/*
 * core_impl::add_communicator
 */
void core_impl::add_communicator(communicator::ptr communicator) {
    this->connect(communicator);
    // do not handle connection here. Instead us the 'connecting' and 'connected' events.
}


/*
 * core_impl::remove_communicator
 */
void core_impl::remove_communicator(communicator::ptr communicator) {
    // do not handle connection here. Instead us the 'disconnecting' and 'disconnected' events.
    this->disconnect(communicator);
}


/*
 * core_impl::remove_all_communicators
 */
void core_impl::remove_all_communicators(void) {
    // do not handle connection here. Instead us the 'disconnecting' and 'disconnected' events.
    std::vector<api_ptr_base> peers = this->select<communicator>();
    size_t peer_cnt = peers.size();
    for (size_t i = 0; i < peer_cnt; ++i) {
        this->disconnect(peers[i]);
    }
}


/*
 * core_impl::enumerate_communicators
 */
void core_impl::enumerate_communicators(communicator_enumerator enumerator, void *ctxt) {
    std::vector<api_ptr_base> peers = this->select<communicator>();
    size_t peer_cnt = peers.size();
    for (size_t i = 0; i < peer_cnt; ++i) {
        communicator::ptr* p = dynamic_cast<communicator::ptr*>(&peers[i]);
        if (p == nullptr) continue;
        if (!enumerator(*p, ctxt)) break;
    }
}


/*
 * core_impl::add_provider
 */
void core_impl::add_provider(provider::ptr provider) {
    this->connect(provider);
    // do not handle connection here. Instead us the 'connecting' and 'connected' events.
}


/*
 * core_impl::remove_provider
 */
void core_impl::remove_provider(provider::ptr provider) {
    // do not handle connection here. Instead us the 'disconnecting' and 'disconnected' events.
    this->disconnect(provider);
}


/*
 * core_impl::remove_all_providers
 */
void core_impl::remove_all_providers(void) {
    // do not handle connection here. Instead us the 'disconnecting' and 'disconnected' events.
    std::vector<api_ptr_base> peers = this->select<provider>();
    size_t peer_cnt = peers.size();
    for (size_t i = 0; i < peer_cnt; ++i) {
        this->disconnect(peers[i]);
    }
}


/*
 * core_impl::enumerate_providers
 */
void core_impl::enumerate_providers(provider_enumerator enumerator, void *ctxt) {
    std::vector<api_ptr_base> peers = this->select<provider>();
    size_t peer_cnt = peers.size();
    for (size_t i = 0; i < peer_cnt; ++i) {
        provider::ptr* p = dynamic_cast<provider::ptr*>(&peers[i]);
        if (p == nullptr) continue;
        if (!enumerator(*p, ctxt)) break;
    }
}


/*
 * core_impl::shutdown
 */
void core_impl::shutdown(void) {
    this->disconnect_all_recursively();
}


/*
 * core_impl::lock
 */
void core_impl::lock(void) {
    this->lock_obj.lock();
}


/*
 * core_impl::unlock
 */
void core_impl::unlock(void) {
    this->lock_obj.unlock();
}


/*
 * core_impl::log
 */
error_log& core_impl::log(void) {
    return this->log_obj;
}
