/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULES_TDDTAGSTREAMMUX_IMPL_H
#define INCLUDED_TDDMODULES_TDDTAGSTREAMMUX_IMPL_H

#include <gnuradio/tddModules/tddTagStreamMux.h>

namespace gr {
namespace tddModules {

class tddTagStreamMux_impl : public tddTagStreamMux {
private:
  const size_t d_itemsize;
  const unsigned int d_tag_preserve_head_pos, d_header_len_offset;

protected:
  int calculate_output_stream_length(const gr_vector_int &ninput_items);

public:
  tddTagStreamMux_impl(size_t itemsize, const std::string &lengthtagname,
                       unsigned int tag_preserve_head_pos, unsigned int header_len);
  ~tddTagStreamMux_impl();

  // Where all the action really happens
  int work(int noutput_items, gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

};

} // namespace tddModules
} // namespace gr

#endif /* INCLUDED_TDDMODULES_TDDTAGSTREAMMUX_IMPL_H */
