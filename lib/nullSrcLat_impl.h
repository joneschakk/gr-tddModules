/* -*- c++ -*- */
/*
 * Copyright 2025 jayc.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULES_NULLSRCLAT_IMPL_H
#define INCLUDED_TDDMODULES_NULLSRCLAT_IMPL_H

#include <gnuradio/tddModules/nullSrcLat.h>

namespace gr {
namespace tddModules {

class nullSrcLat_impl : public nullSrcLat
{
private:
    // Nothing to declare in this block.
    uint64_t d_granularity;
    size_t d_item_size;
public:
    nullSrcLat_impl(size_t item_size, uint64_t granularity);
    ~nullSrcLat_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace tddModules
} // namespace gr

#endif /* INCLUDED_TDDMODULES_NULLSRCLAT_IMPL_H */
