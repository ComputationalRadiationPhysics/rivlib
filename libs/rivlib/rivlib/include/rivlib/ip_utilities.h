/*
 * rivlib API
 * ip_utilities.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_IP_HANDSHAKE_ID_H_INCLUDED
#define VICCI_RIVLIB_IP_HANDSHAKE_ID_H_INCLUDED
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


    /**
     * magic id header to be used when establishing connections
     * The size of the struct must be 16 bytes.
     */
    typedef struct _ip_handshake_id_t {

        /** The id string { 'R', 'I', 'V', 0x13, 0x57, 0x9B, 0xDF, 0x00 } */
        unsigned char id_str[8];

        /** test dword 0x12345678 */
        unsigned int tst_dword;

        /** test float 2.71828175f */
        float tst_float;

    } ip_handshake_id;


    /**
     * Possible data channel types (16 bit)
     */
    enum class data_channel_type : uint16_t {

        /** unknown/error */
        unknown = 0,

        /** stream of images aka video */
        image_stream = 1,

    };


    /**
     * Possible subtypes for the image_stream data channel (16 bit)
     */
    enum class data_channel_image_stream_subtype : uint16_t {

        /** unknown/error */
        unknown = 0,

        /** uncompressed rgb images */
        rgb_raw = 1,

        /** zlib-compressed rgb images */
        rgb_zip = 2,

#if(USE_MJPEG == 1)
        /** jpeg-compressed rgb images */
        rgb_mjpeg = 3,
#endif

    };


    /**
     * RIV lib ip_communicator message ids (32 bit)
     */
    enum class message_id : uint32_t {

        /**
         * Call a control connection which data channels are available at the
         * connected provider.
         *
         * No data.
         */
        query_data_channels = 100,

        /**
         * Answer from provider through control connection which data channels
         * are available.
         *
         * 1x uint32  number of channels
         * n times:
         * 1x uint16  name of channel
         * mx char    ASCII name of channel
         * 1x uint16  type of channel (e.g. image_stream)
         * 1x uint16  subtype of channel (e.g. rgb_raw)
         * 1x uint8   quality of service [0..255] higher values indicate higher quality
         */
        data_channels,

        /** Internal message of image blobs */
        image_data_blob,

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */

#else /* __cplusplus */

#error C API not implemented yet

#endif /* __cplusplus */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VICCI_RIVLIB_IP_HANDSHAKE_ID_H_INCLUDED */
