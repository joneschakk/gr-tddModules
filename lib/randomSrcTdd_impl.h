/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULES_RANDOMSRCTDD_IMPL_H
#define INCLUDED_TDDMODULES_RANDOMSRCTDD_IMPL_H

#include <gnuradio/tddModules/randomSrcTdd.h>
#include <gnuradio/high_res_timer.h>
namespace gr {
namespace tddModules {

class randomSrcTdd_impl : public randomSrcTdd
{
private:
    long d_total_samples, d_counter;
    int d_enable;
    const pmt::pmt_t eob_key = pmt::intern("tx_eob");//,, time_key, d_port_name;
    const pmt::pmt_t sob_key = pmt::intern("tx_sob");
    const pmt::pmt_t d_port_name = pmt::intern("src_trig");
    pmt::pmt_t time_key;
    void handler(pmt::pmt_t msg);
    u_int8_t send_counter_data;
    int d_mode;
public:
    randomSrcTdd_impl(float tx_time_interval, long samples_per_sec, int mode);
    ~randomSrcTdd_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace tddModules
} // namespace gr

#endif /* INCLUDED_TDDMODULES_RANDOMSRCTDD_IMPL_H */
