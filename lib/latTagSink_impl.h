/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULES_LATTAGSINK_IMPL_H
#define INCLUDED_TDDMODULES_LATTAGSINK_IMPL_H

#include <gnuradio/tddModules/latTagSink.h>
#include <gnuradio/high_res_timer.h>
namespace gr {
namespace tddModules {

class latTagSink_impl : public latTagSink
{
private:
    float d_time_now_start, d_start_diff, d_time_now_end, d_end_diff,
        d_start_off,d_end_off;

public:
    latTagSink_impl(size_t item_size);
    ~latTagSink_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace tddModules
} // namespace gr

#endif /* INCLUDED_TDDMODULES_LATTAGSINK_IMPL_H */
