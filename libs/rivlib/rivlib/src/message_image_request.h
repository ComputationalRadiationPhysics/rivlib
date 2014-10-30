/*
 * rivlib
 * message_image_request.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#pragma once

#include "the/config.h"
#include "the/types.h"

namespace eu_vicci {
namespace rivlib {


    /**
     * Message sent to request an image frame from an image stream
     */
#ifdef THE_WINDOWS
#pragma pack(push)
#pragma pack(1)
#endif /* THE_WINDOWS */
    typedef union _msg_img_req {
        unsigned char bytes[5];
        struct _req {
            unsigned char id : 8;
            uint32_t time_code;
        }
#ifdef THE_LINUX
        __attribute__((packed))
#endif /* THE_LINUX */
        req;
    }
#ifdef THE_LINUX
    __attribute__((packed))
#endif /* THE_LINUX */
    message_image_request;
#ifdef THE_WINDOWS
#pragma pack(pop)
#endif /* THE_WINDOWS */


} /* end namespace rivlib */
} /* end namespace eu_vicci */
