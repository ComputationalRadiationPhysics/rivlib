/*
 * rivlib
 * error_log.cpp
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#include "stdafx.h"
#include "error_log.h"
#include "the/string.h"
#include "the/system/performance_counter.h"
#include "the/system/threading/thread.h"
#include "the/text/string_builder.h"
#include "the/text/string_converter.h"
#include "the/text/string_utility.h"
#include <ctime>
#include <cmath>

using namespace eu_vicci::rivlib;


/*
 * error_log::msg::msg
 */
error_log::msg::msg(void) : type(msg_type::error),
        text("<broken message>"), thread_id(""), time_stamp("") {
    // intentionally empty
}


/*
 * error_log::msg::msg
 */
error_log::msg::msg(const msg& src) : type(src.type), text(src.text),
        thread_id(src.thread_id), time_stamp(src.time_stamp) {
    // intentionally empty
}


/*
 * error_log::msg::~msg
 */
error_log::msg::~msg(void) {
    // intentionally empty
}


/*
 * error_log::msg::operator=
 */
error_log::msg& error_log::msg::operator=(const msg& rhs) {
    this->type = rhs.type;
    this->text = rhs.text;
    this->thread_id = rhs.thread_id;
    this->time_stamp = rhs.time_stamp;
    return *this;
}


/*
 * error_log::msg::operator==
 */
bool error_log::msg::operator==(const msg& rhs) const {
    return (this->type == rhs.type)
        && (this->text == rhs.text)
        && (this->thread_id == rhs.thread_id)
        && (this->time_stamp == rhs.time_stamp);
}


/*
 * error_log::msg::fix_text
 */
void error_log::msg::fix_text(void) {
    if (!the::text::string_utility::ends_with(this->text, '\n')) {
        this->text += "\n";
    }
}


/*
 * error_log::msg::set_thread_id
 */
void error_log::msg::set_thread_id(void) {
    the::text::string_converter::convert(this->thread_id,
        the::system::threading::thread::get_current_id().to_astring());
}


/*
 * error_log::msg::set_time_stamp
 */
void error_log::msg::set_time_stamp(void) {
    std::time_t now_time = std::time(NULL);
    struct tm *now_tm;
#if defined(THE_WINDOWS)
    struct tm now_time_tm_storage;
    localtime_s(&now_time_tm_storage, &now_time);
    now_tm = &now_time_tm_storage;
#else /* THE_WINDOWS */
    now_tm = std::localtime(&now_time);
#endif /* THE_WINDOWS */

    double now_tick = 1000.0 * std::fabs(the::system::performance_counter::query_millis());
    now_tick = std::fmod(now_tick, 1.0);
    int now_millis = static_cast<int>(now_tick * 1000) % 1000;

    // ISO 8601: 2004-02-12T15:19:21+00:00
    the::text::astring_builder::format_to(this->time_stamp,
        "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3d",
        1900 + now_tm->tm_year, now_tm->tm_mon + 1, now_tm->tm_mday,
        now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec, now_millis);

}


/*
 * error_log::msg::print
 */
void error_log::msg::print(FILE *stream) {
    char tc = 'x';
    switch (this->type) {
    case msg_type::error: tc = 'E'; break;
    case msg_type::warning: tc = 'W'; break;
    case msg_type::info: tc = 'I'; break;
    }
    fprintf(stream, "%c(%s; %s): %s",
        tc, this->time_stamp.c_str(),
        this->thread_id.c_str(),
        this->text.c_str());
}


/*
 * error_log::null_log
 */
error_log error_log::null_log(false);


/*
 * error_log::error_log
 */
error_log::error_log(bool enable) : enable(enable), lock_obj() {
    // intentionally empty
}


/*
 * error_log::~error_log
 */
error_log::~error_log(void) {
    // intentionally empty
}


/*
 * error_log::message_va
 */
void error_log::message_va(msg_type t, const char* msg, va_list args) {
    if (!this->enable) return;

    error_log::msg m;
    the::text::astring_builder::formatVa_to(m.text, msg, args);
    m.type = t;
    m.fix_text();
    m.set_thread_id();
    m.set_time_stamp();

    this->lock_obj.lock();
    m.print((t == msg_type::error) ? stderr : stdout);
    this->lock_obj.unlock();
}


/*
 * error_log::message_va
 */
void error_log::message_va(msg_type t, const wchar_t* msg, va_list args) {
    if (!this->enable) return;

    the::wstring wm;
    error_log::msg m;
    the::text::wstring_builder::formatVa_to(wm, msg, args);
    the::text::string_converter::convert(m.text, wm);
    m.type = t;
    m.fix_text();
    m.set_thread_id();
    m.set_time_stamp();

    this->lock_obj.lock();
    m.print((t == msg_type::error) ? stderr : stdout);
    this->lock_obj.unlock();
}
