/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULEPKG_TDDRANDOMSRC_IMPL_H
#define INCLUDED_TDDMODULEPKG_TDDRANDOMSRC_IMPL_H

#include <gnuradio/tddModulePkg/tddRandomSrc.h>

namespace gr {
namespace tddModulePkg {

class tddRandomSrc_impl : public tddRandomSrc
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
    int d_mode, d_pkt_len;

public:
    tddRandomSrc_impl(float tx_time_interval,
                      long samples_per_sec,
                      long pkt_len,
                      bool mode);
    ~tddRandomSrc_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace tddModulePkg
} // namespace gr

#endif /* INCLUDED_TDDMODULEPKG_TDDRANDOMSRC_IMPL_H */
