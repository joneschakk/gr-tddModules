/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULES_TDDFILESOURCE_IMPL_H
#define INCLUDED_TDDMODULES_TDDFILESOURCE_IMPL_H

#include <gnuradio/tddModules/tddFileSource.h>

namespace gr {
namespace tddModules {

class tddFileSource_impl : public tddFileSource {
private:
  // Nothing to declare in this block.

public:
  tddFileSource_impl(size_t itemsize, const char *filename, bool repeat,
                     uint64_t offset, uint64_t len);
  ~tddFileSource_impl();

  // Where all the action really happens
  int work(int noutput_items, gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
};

} // namespace tddModules
} // namespace gr

#endif /* INCLUDED_TDDMODULES_TDDFILESOURCE_IMPL_H */
