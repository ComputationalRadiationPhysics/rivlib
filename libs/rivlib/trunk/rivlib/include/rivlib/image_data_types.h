/*
 * rivlib API
 * image_data_types.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_IMAGE_DATA_TYPES_H_INCLUDED
#define VICCI_RIVLIB_IMAGE_DATA_TYPES_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#include "rivlib/common.h"


#ifdef __cplusplus

namespace eu_vicci {
namespace rivlib {


    /** possible image types */
    enum class image_colour_type {
        rgb,
        bgr,
    };

    /** possible data types */
    enum class image_data_type {
        byte,
    };

    /** possible image orientations */
    enum class image_orientation {
        bottom_up, // y=0 is bottom-most scanline
        top_down, // y=0 is top-most scanline
    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */

#else /* __cplusplus */

#error C API not implemented yet

#endif /* __cplusplus */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VICCI_RIVLIB_IMAGE_DATA_TYPES_H_INCLUDED */
