/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULES_MSGSTROBETDDTX_IMPL_H
#define INCLUDED_TDDMODULES_MSGSTROBETDDTX_IMPL_H

#include <gnuradio/tddModules/msgStrobeTddTx.h>

namespace gr {
namespace tddModules {

class msgStrobeTddTx_impl : public msgStrobeTddTx
{
private:
    const pmt::pmt_t d_port_src;
    const pmt::pmt_t d_port_usrp;
    gr::thread::thread d_thread;
    long d_period_ms;
    long d_guard_ms;
    pmt::pmt_t msg, stream_val, stream_cmd;
    std::atomic<bool> d_finished;
    bool rx_mode;

    void run();

public:
    msgStrobeTddTx_impl(long switch_interval, long guard_time);
    ~msgStrobeTddTx_impl();
    bool start();
    bool stop();  
};

} // namespace tddModules
} // namespace gr

#endif /* INCLUDED_TDDMODULES_MSGSTROBETDDTX_IMPL_H */
