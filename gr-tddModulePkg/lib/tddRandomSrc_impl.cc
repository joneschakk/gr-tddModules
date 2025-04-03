/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "tddRandomSrc_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace tddModulePkg {

tddRandomSrc::sptr
tddRandomSrc::make(float tx_time_interval, long samples_per_sec, long pkt_len, bool mode)
{
    return gnuradio::make_block_sptr<tddRandomSrc_impl>(
        tx_time_interval, samples_per_sec, pkt_len, mode);
}


/*
 * The private constructor
 */
tddRandomSrc_impl::tddRandomSrc_impl(float tx_time_interval,
                                     long samples_per_sec,
                                     long pkt_len,
                                     bool mode)
    : gr::sync_block("tddRandomSrc",
                     gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(1, 1, sizeof(std::uint8_t))),
    d_enable(0),
    d_counter(0),
    send_counter_data(0),
    d_pkt_len(pkt_len),
    d_mode(mode)
{
    d_total_samples = tx_time_interval*(long)(samples_per_sec/1000); //as tx_time_interval is in ms
    message_port_register_in(d_port_name);
    set_msg_handler(d_port_name,
        [this](const pmt::pmt_t& msg) { handler(msg); });
    
    /* Only for debug to have a diff start state
     * if(d_mode)
     *      d_enable = 2;
     *  else
     *     d_enable = 0;
    */
}

/*
 * Our virtual destructor.
 */
tddRandomSrc_impl::~tddRandomSrc_impl() {}

void tddRandomSrc_impl::handler(pmt::pmt_t msg){
    if(pmt::equal(msg,pmt::intern("SWITCH"))){
        if(d_enable == 0){
            d_enable = 2;
        }else if(d_enable == 1){
            d_enable = 0;
        }
    }else if(pmt::equal(msg,pmt::intern("STOP"))){
        if(d_enable == 0){
            d_enable = 0;
        }else if(d_enable == 2){
            d_enable = 3;
        }
    }else{
        d_logger->alert("Msg Error");
    }
}

int tddRandomSrc_impl::work(int noutput_items,
                            gr_vector_const_void_star& input_items,
                            gr_vector_void_star& output_items)
{
    if(d_enable < 2){        
        return 0;
    }
    
    int offset = nitems_written(0);
    std::uint8_t* out = (std::uint8_t*) output_items[0];
    srand(time(0));

    if(d_enable == 3){    
        noutput_items = d_pkt_len; //make it configurable 
        d_enable = 1;
        d_counter = 0;
    }else if(!d_counter){ //start of a burst
        //auto time_now = (float)gr::high_res_timer_now() ; for latency measurements                                
        add_item_tag(0,(offset),sob_key,pmt::PMT_T);//pmt::from_float(time_now)); for latency measurements
        
        if((d_counter+noutput_items)>=d_total_samples){ //tx time is so low that the num samples<buffer size
            noutput_items = d_total_samples;            //sob and eob are in one burst
            //tag offset is getting messed bcoz the end is not packet_len aligned
            //make it configurable 
            noutput_items = noutput_items + (d_pkt_len- (noutput_items%d_pkt_len));            
            d_enable = 1;
            d_counter = 0;
        }
        else{
            d_counter+=noutput_items;
        }
    }
    else if((d_counter+noutput_items)>=d_total_samples){
        noutput_items = d_total_samples - d_counter;
        //tag offset is getting messed bcoz the end is not packet_len aligned
        //make it configurable d_pkt_len
        noutput_items = noutput_items + (d_pkt_len- (noutput_items%d_pkt_len));
        d_enable = 1;
        d_counter = 0;
    }else{
        d_counter+=noutput_items; //in between saob and eob, where burst size>>buffer
    }
    
    if(d_enable == 1){ //send eob
        //auto time_now = (float)gr::high_res_timer_now(); for latency measurements
        add_item_tag(0,offset+(noutput_items-1),eob_key,pmt::PMT_T);//pmt::from_float(time_now)); for latency measurements
    }
    
    //do the copy
    for(auto i=0;i<noutput_items;i++){
        /*for debug
        *(out++)=(u_int8_t)(send_counter_data++);     
        if(send_counter_data > 255)
            send_counter_data = 0;
        */
        *(out++)=(u_int8_t)rand()%255; //for random data
    }

    return noutput_items;
}
} /* namespace tddModulePkg */
} /* namespace gr */
