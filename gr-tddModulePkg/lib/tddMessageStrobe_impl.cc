/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "tddMessageStrobe_impl.h"
#include <gnuradio/io_signature.h>
#include <chrono>
#include <thread>

namespace gr {
namespace tddModulePkg {

tddMessageStrobe::sptr
tddMessageStrobe::make(size_t itemsize, int trx_config, float switch_interval, float guard_time, float switch_interval_dl)
{
    return gnuradio::make_block_sptr<tddMessageStrobe_impl>(
        itemsize, trx_config, switch_interval, guard_time, switch_interval_dl);
}


/*
 * The private constructor
 */
tddMessageStrobe_impl::tddMessageStrobe_impl(size_t itemsize,
                                             int trx_config,
                                             float switch_interval,
                                             float guard_time,
                                             float switch_interval_dl)
    : gr::sync_block("msgStrobeTddRx",
                        gr::io_signature::make(1, 1, (itemsize)),
                        gr::io_signature::make(0, 0, 0)),
    d_finished(false),
    d_started(false),
    rx_mode(true),
    d_st_mode_tx(true),
    d_guard_ms(guard_time),
    d_ul_time_ms(switch_interval),
    d_dl_time_ms(switch_interval_dl),
    d_port_src(pmt::mp("sw_st_trigger"))
{
    if(trx_config&0x2)
        d_st_mode_tx = false;
    if (!(trx_config&0x1)){        
        d_same_ul_dl = true;
        d_dl_time_ms = d_ul_time_ms;
    }              

    message_port_register_out(d_port_src);
}

/*
 * Our virtual destructor.
 */
tddMessageStrobe_impl::~tddMessageStrobe_impl() {}

bool tddMessageStrobe_impl::start()
{
    d_finished = false;
    if(d_st_mode_tx){
        //if this is in master config
        d_thread = gr::thread::thread([this] { run(); });
    }
    return block::start();
}

bool tddMessageStrobe_impl::stop()
{
    d_finished = true;
    d_thread.interrupt();
    d_thread.join();
    
    return block::stop();
}
void tddMessageStrobe_impl::run()
{    
    while(!d_finished){
        //downlink time
        std::this_thread::sleep_for(
            std::chrono::milliseconds(static_cast<long>(d_dl_time_ms)));
       
        message_port_pub(d_port_src,pmt::intern("SWITCH"));        
        
        //uplink time (tx)
        std::this_thread::sleep_for(
            std::chrono::milliseconds(static_cast<long>(d_ul_time_ms-d_guard_ms)));
        
        message_port_pub(d_port_src,pmt::intern("STOP"));
        
        std::this_thread::sleep_for(
            std::chrono::milliseconds(static_cast<long>(d_guard_ms)));

        message_port_pub(d_port_src,pmt::intern("SWITCH"));        
    }
    return;
}
int tddMessageStrobe_impl::work(int noutput_items,
                              gr_vector_const_void_star& input_items,
                              gr_vector_void_star& output_items)
{
    if(!d_started&&!d_st_mode_tx){
        //only in follower config
        d_thread = gr::thread::thread([this] { run(); });
        d_started = true;
    }

    //modeled after null_sink 
    return noutput_items;
}

} /* namespace tddModulePkg */
} /* namespace gr */
