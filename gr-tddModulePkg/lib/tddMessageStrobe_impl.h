/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULEPKG_TDDMESSAGESTROBE_IMPL_H
#define INCLUDED_TDDMODULEPKG_TDDMESSAGESTROBE_IMPL_H

#include <gnuradio/tddModulePkg/tddMessageStrobe.h>

namespace gr {
namespace tddModulePkg {
//#ifdef EN_USRP_STREAM_CMDS

class tddMessageStrobe_impl : public tddMessageStrobe
{
private:
    const pmt::pmt_t d_port_src;
    const pmt::pmt_t d_port_usrp;
    gr::thread::thread d_thread;
    float d_period_ms;
    float d_guard_ms;
    pmt::pmt_t msg, stream_val, stream_cmd;
    std::atomic<bool> d_finished;
    bool d_started, rx_mode, d_st_mode;

    void run();

public:
    tddMessageStrobe_impl(size_t itemsize, float switch_interval, float guard_time, bool st_mode);
    ~tddMessageStrobe_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
    bool start();
    bool stop();
};

} // namespace tddModulePkg
} // namespace gr

#endif /* INCLUDED_TDDMODULEPKG_TDDMESSAGESTROBE_IMPL_H */
