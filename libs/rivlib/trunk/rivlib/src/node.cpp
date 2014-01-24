/*
 * rivlib
 * node.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "node.h"
#include <algorithm>
#include "the/argument_null_exception.h"

using namespace eu_vicci::rivlib;


/*
 * node::~node
 */
node::~node(void) {
    // cannot call "this->disconnect_all();" because this would lock,
    //  which is no longer existant here (pure virtual method thus)
    THE_ASSERT(this->peer_nodes.size() == 0);
}


/*
 * node::get_api_ptr
 */
std::shared_ptr<node_base> node::get_api_ptr(void) {
    if (this->self_api_ptr.expired()) {
        std::shared_ptr<node_base> nn(this);
        this->self_api_ptr = nn;
        return nn;
    }
    return this->self_api_ptr.lock();
}


/*
 * node::node
 */
node::node(void) : node_base(), self_api_ptr(), peer_nodes(), connecting_event(),
        connected_event(), disconnecting_event(), disconnected_event() {
    // intentionally empty
}


/*
 * node::connect
 */
void node::connect(api_ptr_base peer) {
    the::system::threading::auto_lock<node>(*this);
    // locking peer is not required, because I know that all 'nodes' use the same lock object
    if (!peer) throw the::argument_null_exception("peer", __FILE__, __LINE__);
    if (this->is_connected(peer)) return;

    node *peer_node = dynamic_cast<node*>(peer.get());
    THE_ASSERT(peer_node != nullptr);

    bool abort = false;
    this->on_connecting(peer, abort);
    if (abort) throw the::exception("aborted", __FILE__, __LINE__);
    peer_node->on_connecting(this, abort);
    if (abort) throw the::exception("aborted", __FILE__, __LINE__);

    this->peer_nodes.push_back(peer);
    peer_node->peer_nodes.push_back(this);

    this->on_connected(peer);
    peer_node->on_connected(this);

}


/*
 * node::disconnect
 */
void node::disconnect(api_ptr_base peer) {
    the::system::threading::auto_lock<node>(*this);
    // locking peer is not required, because I know that all 'nodes' use the same lock object
    if (!peer) throw the::argument_null_exception("peer", __FILE__, __LINE__);
    std::vector<api_ptr_base>::iterator this_it
        = std::find(this->peer_nodes.begin(), this->peer_nodes.end(), peer);
    if (this_it == this->peer_nodes.end()) return;

    node *peer_node = dynamic_cast<node*>(peer.get());
    THE_ASSERT(peer_node != nullptr);

    std::vector<api_ptr_base>::iterator node_it
        = std::find(peer_node->peer_nodes.begin(), peer_node->peer_nodes.end(), this);
    THE_ASSERT(node_it != peer_node->peer_nodes.end());

    bool abort = false;
    this->on_disconnecting(peer, abort);
    if (abort) throw the::exception("aborted", __FILE__, __LINE__);
    peer_node->on_disconnecting(this, abort);
    if (abort) throw the::exception("aborted", __FILE__, __LINE__);

    try {
        this->peer_nodes.erase(this_it);
    } catch(...) {
        THE_ASSERT(std::find(this->peer_nodes.begin(), this->peer_nodes.end(), peer)
            == this->peer_nodes.end());
    }
    try {
        peer_node->peer_nodes.erase(node_it);
    } catch(...) {
        THE_ASSERT(std::find(peer_node->peer_nodes.begin(), peer_node->peer_nodes.end(), this)
            == peer_node->peer_nodes.end());
    }

    this->on_disconnected(peer);
    peer_node->on_disconnected(this);

}


/*
 * node::disconnect_all
 */
void node::disconnect_all(void) {
    the::system::threading::auto_lock<node>(*this);

    std::vector<api_ptr_base> ary(this->peer_nodes);
    size_t cnt = ary.size();
    for (size_t i = 0; i < cnt; ++i) {
        node* n = dynamic_cast<node*>(ary[i].get());
        THE_ASSERT(n != nullptr);
        n->disconnect(this);
    }

    THE_ASSERT(this->peer_nodes.size() == 0);
}


/*
 * node::disconnect_all_recursively
 */
void node::disconnect_all_recursively(void) {
    the::system::threading::auto_lock<node>(*this);

    std::vector<api_ptr_base> ary(this->peer_nodes);
    size_t cnt = ary.size();
    this->disconnect_all();

    THE_ASSERT(this->peer_nodes.size() == 0);

    for (size_t i = 0; i < cnt; ++i) {
        node* n = dynamic_cast<node*>(ary[i].get());
        THE_ASSERT(n != nullptr);
        n->disconnect_all_recursively();
    }

}


/*
 * node::is_connected
 */
bool node::is_connected(api_ptr_base peer) {
    the::system::threading::auto_lock<node>(*this);
    return std::find(this->peer_nodes.begin(),
        this->peer_nodes.end(), peer)
        != this->peer_nodes.end();
}


/*
 * node::on_connecting
 */
void node::on_connecting(api_ptr_base peer, bool& abort) {
    this->connecting_event(peer, abort);
}


/*
 * node::on_connected
 */
void node::on_connected(api_ptr_base peer) {
    this->connected_event(peer);
}


/*
 * node::on_disconnecting
 */
void node::on_disconnecting(api_ptr_base peer, bool& abort) {
    this->disconnecting_event(peer, abort);
}


/*
 * node::on_disconnected
 */
void node::on_disconnected(api_ptr_base peer) {
    this->disconnected_event(peer);
}
