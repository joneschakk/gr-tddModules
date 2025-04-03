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
tddMessageStrobe::make(size_t itemsize, float switch_interval, float guard_time, bool st_mode)
{
    return gnuradio::make_block_sptr<tddMessageStrobe_impl>(
        itemsize, switch_interval, guard_time, st_mode);
}


/*
 * The private constructor
 */
tddMessageStrobe_impl::tddMessageStrobe_impl(size_t itemsize,
                                             float switch_interval,
                                             float guard_time,
                                             bool st_mode)
    : gr::sync_block("msgStrobeTddRx",
                        gr::io_signature::make(1, 1, sizeof(itemsize)),
                        gr::io_signature::make(0, 0, 0)),
    d_finished(false),
    d_started(false),
    rx_mode(true),
    d_st_mode(st_mode),
    d_guard_ms(guard_time),
    d_period_ms(switch_interval),
    d_port_src(pmt::mp("sw_st_trigger"))
#ifdef EN_USRP_STREAM_CMDS
    ,d_port_usrp(pmt::mp("usrp_trigger"))
{
    message_port_register_out(d_port_usrp);
#else
{
#endif
    message_port_register_out(d_port_src);
}

/*
 * Our virtual destructor.
 */
tddMessageStrobe_impl::~tddMessageStrobe_impl() {}

bool tddMessageStrobe_impl::start()
{
    d_finished = false;
    if(d_st_mode){
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
#ifdef EN_USRP_STREAM_CMDS
    stream_val = pmt::make_dict();
    stream_val = pmt::dict_add(stream_val,pmt::intern("stream_now"),
                    pmt::PMT_T);
#endif
    while(!d_finished){
        std::this_thread::sleep_for(
            std::chrono::milliseconds(static_cast<long>(d_period_ms-d_guard_ms)));
        
        message_port_pub(d_port_src,pmt::intern("STOP"));
        
        std::this_thread::sleep_for(
            std::chrono::milliseconds(static_cast<long>(d_guard_ms)));
#ifdef EN_USRP_STREAM_CMDS
        if (rx_mode){
            stream_val = pmt::dict_add(stream_val,
                pmt::intern("stream_mode"),pmt::intern("stop_cont"));
        }
        else{
            stream_val = pmt::dict_add(stream_val,
                pmt::intern("stream_mode"),pmt::intern("start_cont"));
        }
        rx_mode = !rx_mode;
        stream_cmd = pmt::make_dict();
        stream_cmd = pmt::dict_add(stream_cmd,pmt::intern("stream_cmd"),
                                    stream_val);

        message_port_pub(d_port_usrp,stream_cmd);
#endif
        message_port_pub(d_port_src,pmt::intern("SWITCH"));        
    }
    return;
}
int tddMessageStrobe_impl::work(int noutput_items,
                              gr_vector_const_void_star& input_items,
                              gr_vector_void_star& output_items)
{
    if(!d_started&&!d_st_mode){
        //only in follower config
        d_thread = gr::thread::thread([this] { run(); });
        d_started = true;
    }

    //modeled after null_sink 
    return noutput_items;
}

} /* namespace tddModulePkg */
} /* namespace gr */
