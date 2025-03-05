/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "msgStrobeTddRx_impl.h"
#include <gnuradio/io_signature.h>
#include <chrono>
#include <thread>

namespace gr {
namespace tddModules {

using input_type = gr_complex;
msgStrobeTddRx::sptr msgStrobeTddRx::make(long switch_interval, long guard_time)
{
    return gnuradio::make_block_sptr<msgStrobeTddRx_impl>(switch_interval, guard_time);
}


/*
 * The private constructor
 */
msgStrobeTddRx_impl::msgStrobeTddRx_impl(long switch_interval = 1000, long guard_time = 100)
    : gr::sync_block("msgStrobeTddRx",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(0, 0, 0)),
     d_finished(false),
     d_started(false),
     rx_mode(true),
     d_guard_ms(guard_time),
     d_period_ms(switch_interval),
     d_port_src(pmt::mp("sw_st_trigger")),
     d_port_usrp(pmt::mp("usrp_trigger"))
{
    message_port_register_out(d_port_src);
    message_port_register_out(d_port_usrp);

}

/*
 * Our virtual destructor.
 */
msgStrobeTddRx_impl::~msgStrobeTddRx_impl() {}

bool msgStrobeTddRx_impl::start()
{
    return block::start();
}

bool msgStrobeTddRx_impl::stop()
{
    d_finished = true;
    d_thread.interrupt();
    d_thread.join();
    
    return block::stop();
}
void msgStrobeTddRx_impl::run()
{    
    stream_val = pmt::dict_add(stream_val,pmt::intern("stream_now"),
                    pmt::PMT_T);
    while(!d_finished){
        std::this_thread::sleep_for(
            std::chrono::milliseconds(static_cast<long>(d_period_ms-d_guard_ms)));
        message_port_pub(d_port_src,pmt::intern("STOP"));
        std::this_thread::sleep_for(
            std::chrono::milliseconds(static_cast<long>(d_guard_ms)));
        if (rx_mode){
            stream_val = pmt::dict_add(stream_val,
                pmt::intern("stream_mode"),pmt::intern("stop_cont"));
        }
        else{
            stream_val = pmt::dict_add(stream_val,
                pmt::intern("stream_mode"),pmt::intern("start_cont"));
        }
        rx_mode = ~rx_mode;
        stream_cmd = pmt::dict_add(stream_cmd,pmt::intern("stream_cmd"),
                                    stream_val);
        message_port_pub(d_port_usrp,stream_cmd);

        message_port_pub(d_port_src,pmt::intern("SWITCH"));        
    }
    return;
}
int msgStrobeTddRx_impl::work(int noutput_items,
                              gr_vector_const_void_star& input_items,
                              gr_vector_void_star& output_items)
{
    if(!d_started){
        d_thread = gr::thread::thread([this] { run(); });
        d_started = true;
    }

    //modeled after null_sink 
    return noutput_items;
}

} /* namespace tddModules */
} /* namespace gr */
