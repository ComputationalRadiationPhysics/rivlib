/*
 * rivlib
 * data/slot.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "data/slot.h"

using namespace eu_vicci;
using namespace eu_vicci::rivlib;


/*
 * data::slot::slot
 */
data::slot::slot(void) : buf(), lock_obj(), on_update_event(), on_close_event() {
    // intentionally empty
}


/*
 * data::slot::~slot
 */
data::slot::~slot(void) {
    this->on_close_event(*this);
    // Clearing the event would be nice
    this->buf.reset();
}


/*
 * data::slot::set_buffer
 */
void data::slot::set_buffer(buffer::shared_ptr b) {
    bool update = false;
    {
        auto_lock l(this->lock_obj);
        update = (this->buf != b);
        this->buf = b;
    }

    if (update) {
        this->on_update_event(*this, this->buf);
    }
}
