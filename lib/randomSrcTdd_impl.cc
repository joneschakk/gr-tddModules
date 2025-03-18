/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "randomSrcTdd_impl.h"
#include <gnuradio/io_signature.h>
// #include <chrono>


namespace gr {
namespace tddModules {

randomSrcTdd::sptr randomSrcTdd::make(float tx_time_interval, long samples_per_sec)
{
    return gnuradio::make_block_sptr<randomSrcTdd_impl>(tx_time_interval,
                                                        samples_per_sec);
}


/*
 * The private constructor
 */
randomSrcTdd_impl::randomSrcTdd_impl(float tx_time_interval, long samples_per_sec)
    : gr::sync_block("randomSrcTdd",
                     gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(1, 1, sizeof(std::uint8_t))),
    d_enable(0),
    d_counter(0)
{
    d_total_samples = tx_time_interval*samples_per_sec;
    message_port_register_in(d_port_name);
    set_msg_handler(d_port_name,
        [this](const pmt::pmt_t& msg) { handler(msg); });
}

/*
 * Our virtual destructor.
 */
randomSrcTdd_impl::~randomSrcTdd_impl() {}

void randomSrcTdd_impl::handler(pmt::pmt_t msg){
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

int randomSrcTdd_impl::work(int noutput_items,
                            gr_vector_const_void_star& input_items,
                            gr_vector_void_star& output_items)
{
    if(d_enable < 2){
        // std::cout<<"Yo AA "<<d_enable<<" "<<d_counter<<"\n";
        return 0;
    }
    // std::cout<<"Yo St "<<d_enable<<" "<<d_counter<<"\n";
    int offset = nitems_written(0);
    std::uint8_t* out = (std::uint8_t*) output_items[0];
    srand(time(0));

    if(d_enable == 3){
        // std::cout<<"Yo EOB "<<d_enable<<" "<<d_counter<<"\n";
        noutput_items = 1920; //make it configurable 
        d_enable = 1;
        d_counter = 0;
    }else if(!d_counter){ //start of a burst
        auto time_now = (float)gr::high_res_timer_now()/
                                (float)gr::high_res_timer_tps();
        add_item_tag(0,(offset),sob_key,pmt::PMT_T);//pmt::from_float(time_now));//pmt::PMT_T
        
        if((d_counter+noutput_items)>=d_total_samples){ //tx time is so low that the num samples<buffer size
            noutput_items = d_total_samples;            //sob and eob are in one burst
            //tag offset is getting messed bcoz the end is not packet_len aligned
            //make it configurable 
            noutput_items = noutput_items + (1920- (noutput_items%1920));

            // std::cout<<"Yo SOB-EOB "<<time_now<<", "<<d_counter<<", "<<noutput_items<<", "<<offset<<"\n";
            d_enable = 1;
            d_counter = 0;
        }
        else{
            d_counter+=noutput_items;
            // std::cout<<"Yo SOB "<<d_enable<<" "<<d_counter<<" "<<offset<<"\n";
        }
        // std::cout<<"Tot samp "<<d_total_samples<<"\n";
    }
    else if((d_counter+noutput_items)>=d_total_samples){
        noutput_items = d_total_samples - d_counter;
        //tag offset is getting messed bcoz the end is not packet_len aligned
        //make it configurable 1920
        noutput_items = noutput_items + (1920- (noutput_items%1920));
        // std::cout<<"Yo FullEOB "<<d_enable<<" "<<d_counter<<", "<<noutput_items<<" "<<offset<<"\n";
        d_enable = 1;
        d_counter = 0;
    }else{
        d_counter+=noutput_items; //in between saob and eob, where burst size>>buffer
        // std::cout<<"Yo normie "<<d_enable<<" "<<d_counter<<"\n";
    }
    
    if(d_enable == 1){ //send eob
        auto time_now = (float)gr::high_res_timer_now()/
                                (float)gr::high_res_timer_tps();
        add_item_tag(0,offset+(noutput_items-1),eob_key,pmt::PMT_T);//pmt::from_float(time_now));
        // std::cout<<"Yo EEOB "<<d_enable<<" "<<d_counter<<", "<<time_now<<"\n";
    }

    //do the copy
    for(auto i=0;i<noutput_items;i++){
        *(out++)=(u_int8_t)rand()%255;
    }


    // if((d_counter)&&(((d_counter+noutput_items)>=d_total_samples)||(d_enable == 3))){
    //     if(d_enable == 3){
    //         noutput_items = 1;
    //     }else{
    //         noutput_items = d_total_samples - d_counter;
    //     }
    //     auto time_now = (float)gr::high_res_timer_now()/
    //                            (float)gr::high_res_timer_tps();
    //     add_item_tag(0,noutput_items,eob_key,pmt::from_float(time_now));
    //     for(auto i=0;i<noutput_items;i++){
    //         *(out++)=(u_int8_t)rand()%255;
    //     }
    //     std::cout<<"Yo EOB "<<d_enable<<" "<<d_counter<<"\n";
    //     d_enable = 1;
    //     d_counter = 0;
    // }else {
    //     if(!d_counter){
    //         auto time_now = (float)gr::high_res_timer_now()/
    //                             (float)gr::high_res_timer_tps();
    //         add_item_tag(0,0,sob_key,pmt::from_float(time_now));//pmt::PMT_T
    //         d_counter+=noutput_items;
    //         std::cout<<"Tot samp "<<d_total_samples<<"\n";
    //         if(noutput_items>=d_total_samples){
    //             noutput_items = d_total_samples;
    //             add_item_tag(0,noutput_items-1,eob_key,pmt::from_float(time_now));//pmt::PMT_T
    //             d_counter = 0;
    //             d_enable = 1;
    //             std::cout<<"Yo SOB-EOB "<<d_enable<<" "<<d_counter<<"\n";
    //         }else{
    //             std::cout<<"Yo SOB "<<d_enable<<" "<<d_counter<<"\n";
    //         }
    //     }else{
    //         d_counter+=noutput_items;
    //         std::cout<<"Yo normie "<<d_enable<<" "<<d_counter<<"\n";
    //     }
    //     for(auto i=0;i<noutput_items;i++){
    //         *(out++)=(u_int8_t)rand()%255;
    //     } 
        
        
    // }

    return noutput_items;
}

} /* namespace tddModules */
} /* namespace gr */
