/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULES_MSGSTROBETDDRX_IMPL_H
#define INCLUDED_TDDMODULES_MSGSTROBETDDRX_IMPL_H

#include <gnuradio/tddModules/msgStrobeTddRx.h>

namespace gr {
namespace tddModules {

class msgStrobeTddRx_impl : public msgStrobeTddRx
{
private:
    const pmt::pmt_t d_port_src;
    const pmt::pmt_t d_port_usrp;
    gr::thread::thread d_thread;
    long d_period_ms;
    long d_guard_ms;
    pmt::pmt_t msg, stream_val, stream_cmd;
    std::atomic<bool> d_finished;
    bool d_started, rx_mode, d_st_mode;

    void run();

public:
    msgStrobeTddRx_impl(long switch_interval, long guard_time, bool st_mode);
    ~msgStrobeTddRx_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
    bool start();
    bool stop();
};

} // namespace tddModules
} // namespace gr

#endif /* INCLUDED_TDDMODULES_MSGSTROBETDDRX_IMPL_H */
