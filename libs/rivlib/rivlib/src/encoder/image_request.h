/*
 * rivlib
 * encoder/image_request.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#pragma once

#include "data/buffer.h"
#include <memory>

namespace eu_vicci {
namespace rivlib {
namespace encoder {


    /**
     * request for a new image frame from a video stream
     */
    class image_request {
    public:

        /** type of smart pointer */
        typedef std::shared_ptr<image_request> ptr;

        /**
         * Callback type used to asynchronously request encoder output
         *
         * @param data The encoded data
         * @param ctxt The user-defined context
         */
        typedef void (* output_callback)(const data::buffer::shared_ptr data, void *ctxt);

        /**
         * ctor
         *
         * @param func The callback function for the output
         * @param ctxt The context pointer for the output callback function
         * @param last_time_id The time id of the last frame
         */
        image_request(output_callback func = nullptr, void *ctxt = nullptr, unsigned int last_time_id = 0);

        /** dtor */
        ~image_request(void);

        /**
         * Calls the output callback
         *
         * @param data Points to the encoded data
         */
        void call(const data::buffer::shared_ptr data);

        /**
         * Clears the stored data
         */
        void reset(void);

        /**
         * Sets the data
         *
         * @param func The callback function for the output
         * @param ctxt The context pointer for the output callback function
         * @param last_time_id The time id of the last frame
         */
        inline void set(output_callback func, void *ctxt, unsigned int last_time_id) {
            this->func = func;
            this->ctxt = ctxt;
            this->last_time_id = last_time_id;
        }

        /**
         * Answer the time if of the last frame
         *
         * @return The time id of the last frame
         */
        inline unsigned int last_time(void) const {
            return this->last_time_id;
        }

        /**
         * Checks if this request uses the specified callback target
         *
         * @param func The callback function for the output
         * @param ctxt The context pointer for the output callback function
         */
        inline bool equals(output_callback func, void *ctxt) const {
            return (this->func == func)
                && (this->ctxt == ctxt);
        }

    private:

        /** The callback function for the output */
        output_callback func;

        /** The context pointer for the output callback function */
        void *ctxt;

        /** The time id of the last frame */
        unsigned int last_time_id;

    };


} /* end namespace encoder */
} /* end namespace rivlib */
} /* end namespace eu_vicci */
