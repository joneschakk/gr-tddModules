/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULEPKG_TDDTAGGEDSTREAMMUX_IMPL_H
#define INCLUDED_TDDMODULEPKG_TDDTAGGEDSTREAMMUX_IMPL_H

#include <gnuradio/tddModulePkg/tddTaggedStreamMux.h>

namespace gr {
namespace tddModulePkg {

class tddTaggedStreamMux_impl : public tddTaggedStreamMux
{
private:
    const size_t d_itemsize;
    const unsigned int d_tag_preserve_head_pos, d_header_len_offset;
  
protected:
    int calculate_output_stream_length(const gr_vector_int &ninput_items);

public:
    tddTaggedStreamMux_impl(size_t itemsize,
                            const std::string& lengthtagname,
                            unsigned int tag_preserve_head_pos,
                            unsigned int header_len);
    ~tddTaggedStreamMux_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_int& ninput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace tddModulePkg
} // namespace gr

#endif /* INCLUDED_TDDMODULEPKG_TDDTAGGEDSTREAMMUX_IMPL_H */
