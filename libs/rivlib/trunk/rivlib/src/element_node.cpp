/*
 * rivlib
 * element_node.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "element_node.h"
#include "api_impl/core_impl.h"
#include "the/collections/fast_forward_list.h"

using namespace eu_vicci::rivlib;


/*
 * element_node::~element_node
 */
element_node::~element_node(void) {
    // intentionally empty
}


/*
 * element_node::lock
 */
void element_node::lock(void) {
    if (!this->core.expired()) {
        this->lock_obj = this->core.lock();
        core_impl* ci = dynamic_cast<core_impl*>(this->lock_obj.get());
        THE_ASSERT(ci != nullptr);
        ci->lock();
    }
}


/*
 * element_node::unlock
 */
void element_node::unlock(void) {
    if (this->lock_obj) {
        core_impl* ci = dynamic_cast<core_impl*>(this->lock_obj.get());
        THE_ASSERT(ci != nullptr);
        ci->unlock();
        this->lock_obj.reset();
    }
}


/*
 * element_node::log
 */
error_log& element_node::log(void) {
    if (this->core.expired()) {
        return error_log::null_log;
    } else {
        std::shared_ptr<node_base> lc = this->core.lock();
        core_impl *ci = dynamic_cast<core_impl*>(lc.get());
        THE_ASSERT(ci != nullptr);
        return ci->log();
    }
}


/*
 * element_node::get_core
 */
core::ptr element_node::get_core(void) {
    if (this->core.expired()) {
        this->search_core();
    }

    if (!this->core.expired()) {
        std::shared_ptr<node_base> csp = this->core.lock();
        rivlib::core::ptr cp(dynamic_cast<rivlib::core*>(csp.get()));
        return cp;
    }
    return nullptr;
}


/*
 * element_node::on_connected
 */
void element_node::on_connected(api_ptr_base peer) {
    this->search_core();
    node::on_connected(peer);
}


/*
 * element_node::on_disconnected
 */
void element_node::on_disconnected(api_ptr_base peer) {
    this->search_core();
    node::on_disconnected(peer);
}


/*
 * element_node::on_core_discovered
 */
void element_node::on_core_discovered(void) {
    this->core_discovered_event();
}


/*
 * element_node::on_core_lost
 */
void element_node::on_core_lost(void) {
    this->core_lost_event();
}


/*
 * element_node::element_node
 */
element_node::element_node(void) : node(), core(), lock_obj(),
        search_lock(false) {
    // intentionally empty
}


/*
 * element_node::search_core
 */
void element_node::search_core(void) {
    if (this->search_lock) return;

    try {
        this->search_lock = true;
        the::collections::fast_forward_list<api_ptr_base> searched;
        the::collections::fast_forward_list<api_ptr_base> searching;
        searching.append(this);

        while (!searching.is_empty()) {
            api_ptr_base s = searching.first();
            searching.remove_first();
            node *sn = dynamic_cast<node*>(s.get());

            core_impl *ci = dynamic_cast<core_impl*>(sn);
            if (ci != nullptr) {
                // core found! set and return.
                if (this->core.expired() || (this->core.lock().get() != ci)) {
                    this->core = ci->get_api_ptr();
                    this->on_core_discovered();
                }
                this->search_lock = false;
                return;
            }

            element_node *sen = dynamic_cast<element_node*>(sn);
            THE_ASSERT(sen != nullptr);

            std::vector<api_ptr_base> ps = sen->select<node>();
            size_t ps_cnt = ps.size();
            for (size_t i = 0; i < ps_cnt; ++i) {
                if (searched.contains(ps[i])) continue;
                searching.append(ps[i]);
            }

            searched.append(s);
        }

        // core not found
        if (!this->core.expired()) {
            this->core.reset();
            this->on_core_lost();
        }
        this->search_lock = false;

    } catch(...) {
        this->search_lock = false;
        throw;
    }
}