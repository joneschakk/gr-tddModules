/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULEPKG_TDDLATREADSINK_IMPL_H
#define INCLUDED_TDDMODULEPKG_TDDLATREADSINK_IMPL_H

#include <gnuradio/tddModulePkg/tddLatReadSink.h>
#include <gnuradio/high_res_timer.h>
namespace gr {
namespace tddModulePkg {

class tddLatReadSink_impl : public tddLatReadSink
{
private:
    float d_time_now_start, d_start_diff, d_time_now_end, d_end_diff,
            d_start_off,d_end_off;

public:
    tddLatReadSink_impl(size_t item_size);
    ~tddLatReadSink_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace tddModulePkg
} // namespace gr

#endif /* INCLUDED_TDDMODULEPKG_TDDLATREADSINK_IMPL_H */
