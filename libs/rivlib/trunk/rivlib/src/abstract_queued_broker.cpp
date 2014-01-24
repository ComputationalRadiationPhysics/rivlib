/*
 * abstract_queued_broker.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "abstract_queued_broker.h"
#include "the/system/threading/auto_lock.h"
#include <vector>
#include <algorithm>
#include "api_impl/core_impl.h"
#include "rivlib/api_ptr_base.h"
#include "rivlib/provider.h"
#include "rivlib/communicator.h"

using namespace eu_vicci::rivlib;
using namespace the::system::threading;


/*
 * abstract_queued_broker::abstract_queued_broker
 */
abstract_queued_broker::abstract_queued_broker(void) : element_node(), runnable(),
    safe_core(nullptr), comms(), provs(), lock_obj(), queue(), worker_thread(nullptr) {
        this->worker_thread = new thread(this);

}


/*
 * abstract_queued_broker::~abstract_queued_broker
 */
abstract_queued_broker::~abstract_queued_broker(void) {
    THE_ASSERT(this->worker_thread != nullptr);
    if (this->worker_thread->is_running()) {
        this->worker_thread->terminate(true);
    }
    the::safe_delete(this->worker_thread);

}


/*
 * abstract_queued_broker::on_thread_terminating
 */
thread::termination_behaviour abstract_queued_broker::on_thread_terminating(void) throw() {
    auto_lock<critical_section> lock(this->lock_obj);
    //this->queue.clear();
    action_entry a;
    a.act = action_type::shutdown;
    this->queue.add(a);
    return thread::termination_behaviour::graceful;
}


/*
 * abstract_queued_broker::run
 */
int abstract_queued_broker::run(void) {
    bool run = true;

    try {
        this->init();
    } catch(the::exception ex) {
        this->log().error("Failed to start broker: %s (%s, %d)",
            ex.get_msg_astr(), ex.get_file(), ex.get_line());
        return -1;
    } catch(...) {
        this->log().error("Failed to start broker: unexpected exception");
        return -1;
    }

    while (run) {
        action_entry a;
        {
            auto_lock<critical_section> lock(this->lock_obj);
            if (!this->queue.empty()) {
                a = this->queue[0];
                this->queue.remove_first();
            }
        }

        switch (a.act) {
        case action_type::none:
            thread::sleep(100);
            break;
        case action_type::activate:
            try {
                this->activate(a.comm, a.prov);
            } catch(the::exception ex) {
                this->log().error("Failed to activate provider \"%s\": %s (%s, %d)",
                    a.prov->get_name_astr(), ex.get_msg_astr(), ex.get_file(), ex.get_line());
            } catch(...) {
                this->log().error("Failed to activate provider \"%s\": unexpected exception",
                    a.prov->get_name_astr());
            }
            break;
        case action_type::deactivate:
            try {
                this->deactivate(a.comm, a.prov);
            } catch(the::exception ex) {
                this->log().error("Failed to deactivate provider \"%s\": %s (%s, %d)",
                    a.prov->get_name_astr(), ex.get_msg_astr(), ex.get_file(), ex.get_line());
            } catch(...) {
                this->log().error("Failed to deactivate provider \"%s\": unexpected exception",
                    a.prov->get_name_astr());
            }
            break;
        case action_type::shutdown:
            run = false;
            break;
        }
    }

    try {
        this->deinit();
    } catch(the::exception ex) {
        this->log().error("Error on stopping broker: %s (%s, %d)",
            ex.get_msg_astr(), ex.get_file(), ex.get_line());
        return -1;
    } catch(...) {
        this->log().error("Error on stopping broker: unexpected exception");
        return -1;
    }

    return 0;
}


/*
 * abstract_queued_broker::on_core_discovered
 */
void abstract_queued_broker::on_core_discovered(void) {
    element_node::on_core_discovered();
    auto_lock<abstract_queued_broker>(*this);
    if (!this->safe_core) {
        this->safe_core = this->get_core();
        dynamic_cast<core_impl*>(this->safe_core.get())->connected() += 
            the::delegate<void, api_ptr_base>(*this, &abstract_queued_broker::on_core_connections_updated);
        dynamic_cast<core_impl*>(this->safe_core.get())->disconnected() += 
            the::delegate<void, api_ptr_base>(*this, &abstract_queued_broker::on_core_connections_updated);
        this->update_info();
    }

    if (!this->worker_thread->is_running()) {
        this->worker_thread->start();
    }
}


/*
 * abstract_queued_broker::on_core_lost
 */
void abstract_queued_broker::on_core_lost(void) {
    element_node::on_core_lost();
    auto_lock<abstract_queued_broker>(*this);
    if (this->safe_core) {
        dynamic_cast<core_impl*>(this->safe_core.get())->connected() -= 
            the::delegate<void, api_ptr_base>(*this, &abstract_queued_broker::on_core_connections_updated);
        dynamic_cast<core_impl*>(this->safe_core.get())->disconnected() -= 
            the::delegate<void, api_ptr_base>(*this, &abstract_queued_broker::on_core_connections_updated);
        this->safe_core.reset();
        this->update_info();
    }

    if (this->worker_thread->is_running()) {
        this->worker_thread->terminate(true);
    }
}


/*
 * abstract_queued_broker::on_core_connections_updated
 */
void abstract_queued_broker::on_core_connections_updated(api_ptr_base peer) {
    this->update_info();
}


/*
 * abstract_queued_broker::queue_activation
 */
void abstract_queued_broker::queue_activation(communicator::ptr comm, provider::ptr prov) {
    auto_lock<critical_section> lock(this->lock_obj);

    action_entry a;
    a.act = action_type::activate;
    a.comm = comm;
    a.prov = prov;

    this->queue.add(a);

}


/*
 * abstract_queued_broker::queue_deactivation
 */
void abstract_queued_broker::queue_deactivation(communicator::ptr comm, provider::ptr prov) {
    auto_lock<critical_section> lock(this->lock_obj);

    // if there is an activation, remove it
    size_t qs = this->queue.size();
    for (size_t i = 0; i < qs; i++) {
        if ((this->queue[i].comm == comm) && (this->queue[i].prov == prov)) {
            this->queue.remove_at(i);
            return;
        }
    }

    action_entry a;
    a.act = action_type::deactivate;
    a.comm = comm;
    a.prov = prov;

    this->queue.add(a);

}


/*
 * abstract_queued_broker::update_info
 */
void abstract_queued_broker::update_info(void) {
    std::vector<api_ptr_base> communicators;
    std::vector<api_ptr_base> providers;

    if (this->safe_core) {
        communicators = dynamic_cast<core_impl*>(this->safe_core.get())->select<communicator>();
        providers = dynamic_cast<core_impl*>(this->safe_core.get())->select<provider>();
    }

    // unregister all pairs no longer active
    for (size_t ci = 0; ci < this->comms.size(); ++ci) {
        bool comm_active = std::find(communicators.begin(), communicators.end(), this->comms[ci]) != communicators.end();

        for (size_t pi = 0; pi < this->provs.size(); ++pi) {
            bool prov_active = comm_active && (std::find(providers.begin(), providers.end(), this->provs[pi]) != providers.end());

            if (!comm_active || !prov_active) {
                this->queue_deactivation(this->comms[ci], this->provs[pi]);
            }
        }
    }

    // register all new pairs
    size_t cs = communicators.size();
    size_t ps = providers.size();
    for (size_t ci = 0; ci < cs; ++ci) {
        communicator::ptr c(dynamic_cast<communicator*>(communicators[ci].get()));
        bool comm_new = this->comms.index_of(c) == the::collections::array<communicator::ptr>::invalid_index;

        for (size_t pi = 0; pi < ps; ++pi) {
            provider::ptr p(dynamic_cast<provider*>(providers[pi].get()));
            bool prov_new = comm_new || this->provs.index_of(p) == the::collections::array<provider::ptr>::invalid_index;

            if (comm_new || prov_new) {
                this->queue_activation(c, p);
            }
        }
    }

    // take over new data
    this->comms.set_size(cs);
    for (size_t ci = 0; ci < cs; ++ci) {
        communicator::ptr c(dynamic_cast<communicator*>(communicators[ci].get()));
        this->comms[ci] = c;
    }
    this->provs.set_size(ps);
    for (size_t pi = 0; pi < ps; ++pi) {
        provider::ptr p(dynamic_cast<provider*>(providers[pi].get()));
        this->provs[pi] = p;
    }

}
