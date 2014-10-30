/*
 * data_channel_info.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_DATA_CHANNEL_INFO_H_INCLUDED
#define VICCI_RIVLIB_DATA_CHANNEL_INFO_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include <string>
#include "the/types.h"
#include "rivlib/ip_utilities.h"


namespace eu_vicci {
namespace rivlib {


    /**
     * Info about data channels
     */
    typedef struct _data_channel_info_ {

        /** The name of the channel */
        std::string name;

        /** The type of the channel */
        data_channel_type type;

        /** The subtype of the channel */
        union _subtype_t {

            /** generic subtype access */
            uint16_t uint;

            /** subtype for image_stream channels */
            data_channel_image_stream_subtype image_stream;

        } subtype;

        /** quality of service [0..255]; higher numbers indicate higher quality */
        uint8_t quality;

    } data_channel_info;


} /* end namespace rivlib */
} /* end namespace eu_vicci */


#endif /* VICCI_RIVLIB_DATA_CHANNEL_INFO_H_INCLUDED */
