/*
 * rivlib
 * data/buffer_type.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#pragma once

namespace eu_vicci {
namespace rivlib {
namespace data {

    /**
     * Possible buffer type ids
     */
    enum class buffer_type : unsigned int {
        invalid,
        raw_rgb_bytes,
        raw_bgr_bytes,
        zip_rgb_bytes
    };


} /* end namespace data */
} /* end namespace rivlib */
} /* end namespace eu_vicci */
