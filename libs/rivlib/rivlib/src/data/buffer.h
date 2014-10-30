/*
 * rivlib
 * data/buffer.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */
#pragma once
#include <memory>
#include "the/blob.h"
#include "data/buffer_type.h"

namespace eu_vicci {
namespace rivlib {
namespace data {


    /**
     * generic buffer class
     */
    class buffer {
    public:

        /** shared pointer to buffer objects */
        typedef std::shared_ptr<buffer> shared_ptr;

        /**
         * Creates a new buffer object
         *
         * @return A new buffer object
         */
        static shared_ptr create(void);

        /** Dtor */
        ~buffer(void);

        /**
         * Access The data blob
         *
         * @return The data blob
         */
        inline the::blob& data(void) {
            return this->data_blob;
        }


        /**
         * Access The data blob
         *
         * @return The data blob
         */
        inline const the::blob& data(void) const {
            return this->data_blob;
        }

        /**
         * Access The metadata blob
         *
         * @return The metadata blob
         */
        inline the::blob& metadata(void) {
            return this->metadata_blob;
        }

        /**
         * Access The metadata blob
         *
         * @return The metadata blob
         */
        inline const the::blob& metadata(void) const {
            return this->metadata_blob;
        }

        /**
         * Gets the buffer time code
         *
         * @return The buffer time code
         */
        inline unsigned int time_code(void) const {
            return this->time_code_value;
        }

        /**
         * Gets the buffer type id
         *
         * @return The buffer type id
         */
        inline buffer_type type(void) const {
            return this->type_id_value;
        }

        /**
         * Sets the buffer time code
         *
         * @param tc The new time code
         */
        inline void set_time_code(unsigned int tc) {
            this->time_code_value = tc;
        }

        /**
         * Sets the buffer type id
         *
         * @param tid The new type id
         */
        inline void set_type(buffer_type tid) {
            this->type_id_value = tid;
        }

        /**
         * Test for equality. Performs a deep comparison.
         *
         * @param rhs The right hand side operand
         *
         * @return True if 'this' and 'rhs' contain equal data
         */
        inline bool operator==(const buffer& rhs) const {
            return (this->type_id_value == rhs.type_id_value)
                && (this->time_code_value == rhs.time_code_value)
                && (this->data_blob.size() == rhs.data_blob.size())
                && (this->metadata_blob.size() == rhs.metadata_blob.size())
                && (this->data_blob == rhs.data_blob)
                && (this->metadata_blob == rhs.metadata_blob);
        }

        /**
         * Test for inequality. Performs a deep comparison.
         *
         * @param rhs The right hand side operand
         *
         * @return True if 'this' and 'rhs' contain different data
         */
        inline bool operator!=(const buffer& rhs) const {
            return (*this) == rhs;
        }

    private:
    
        /** Ctor */
        buffer(void);

        /** The data blob */
        the::blob data_blob;

        /** The metadata blob */
        the::blob metadata_blob;

        /** The time code value */
        unsigned int time_code_value;

        /** The type id value */
        buffer_type type_id_value;

    };


} /* end namespace data */
} /* end namespace rivlib */
} /* end namespace eu_vicci */
