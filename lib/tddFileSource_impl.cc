/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "tddFileSource_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace tddModules {

tddFileSource::sptr tddFileSource::make(size_t itemsize, const char *filename,
                                        bool repeat, uint64_t offset,
                                        uint64_t len) {
  return gnuradio::make_block_sptr<tddFileSource_impl>(itemsize, filename,
                                                       repeat, offset, len);
}

/*
 * The private constructor
 */
tddFileSource_impl::tddFileSource_impl(size_t itemsize, const char *filename,
                                       bool repeat, uint64_t offset,
                                       uint64_t len)
    : gr::sync_block("tddFileSource", gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(1, 1, sizeof(itemsize))) {
                  

                    }

/*
 * Our virtual destructor.
 */
tddFileSource_impl::~tddFileSource_impl() {}

int tddFileSource_impl::work(int noutput_items,
                             gr_vector_const_void_star &input_items,
                             gr_vector_void_star &output_items) {
  auto out = static_cast<output_type *>(output_items[0]);

#pragma message(                                                               \
    "Implement the signal processing in your block and remove this warning")
  // Do <+signal processing+>

  // Tell runtime system how many output items we produced.
  return noutput_items;
}

} /* namespace tddModules */
} /* namespace gr */
