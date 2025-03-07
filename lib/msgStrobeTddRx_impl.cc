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

using input_type = std::uint8_t;
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
                     gr::io_signature::make( 0, 0, 0)),
                        //  1 /* min inputs */, 1 /* max inputs */, sizeof(input_type))),
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
    d_finished = false;
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
    stream_val = pmt::make_dict();
    stream_val = pmt::dict_add(stream_val,pmt::intern("stream_now"),
                    pmt::PMT_T);
    //std::cout<<"STB:St wh\n";
    while(!d_finished){
        std::this_thread::sleep_for(
            std::chrono::milliseconds(static_cast<long>(d_period_ms-d_guard_ms)));
        //std::cout<<"STB:Stp sndng\n";
        message_port_pub(d_port_src,pmt::intern("STOP"));
        //std::cout<<"STB:Stp sent\n";
        std::this_thread::sleep_for(
            std::chrono::milliseconds(static_cast<long>(d_guard_ms)));
        //std::cout<<"STB:usrp dic\n";
        if (rx_mode){
            stream_val = pmt::dict_add(stream_val,
                pmt::intern("stream_mode"),pmt::intern("stop_cont"));
        }
        else{
            stream_val = pmt::dict_add(stream_val,
                pmt::intern("stream_mode"),pmt::intern("start_cont"));
        }
        rx_mode = !rx_mode;
        //std::cout<<"STB:Mode sw\n";
        stream_cmd = pmt::make_dict();
        stream_cmd = pmt::dict_add(stream_cmd,pmt::intern("stream_cmd"),
                                    stream_val);
        
        message_port_pub(d_port_usrp,stream_cmd);
        //std::cout<<"STB:sent usrp\n";
        message_port_pub(d_port_src,pmt::intern("SWITCH"));        
        //std::cout<<"STB:sent srctrg\n";
    }
    return;
}
int msgStrobeTddRx_impl::work(int noutput_items,
                              gr_vector_const_void_star& input_items,
                              gr_vector_void_star& output_items)
{
    if(!d_started){
        //std::cout<<"STB:init th\n";
        d_thread = gr::thread::thread([this] { run(); });
        d_started = true;
    }
    // std::cout<<"STB:normie";
    // const uint8_t** in = (const uint8_t**)&input_items[0];
    // uint8_t** out = (uint8_t**)&output_items[0];

    // int n = 0;

    // int ninputs = input_items.size();
    // for (int i = 0; i < ninputs; i++) {
    //     memcpy(out[i], in[i], noutput_items * sizeof(input_type));
    // }
    // n = noutput_items;

    // output_items[0][:]
    //modeled after null_sink 
    return 0;//noutput_items;
}

} /* namespace tddModules */
} /* namespace gr */
