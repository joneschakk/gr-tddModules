/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "latTagSink_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace tddModules {

// #pragma message("set the following appropriately and remove this warning")
// using input_type = float;
latTagSink::sptr latTagSink::make()
{
    return gnuradio::make_block_sptr<latTagSink_impl>();
}


/*
 * The private constructor
 */
latTagSink_impl::latTagSink_impl()
    : gr::sync_block("latTagSink",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, sizeof(std::uint8_t)),
                     gr::io_signature::make(0, 0, 0))
{
}

/*
 * Our virtual destructor.
 */
latTagSink_impl::~latTagSink_impl() {}

int latTagSink_impl::work(int noutput_items,
                          gr_vector_const_void_star& input_items,
                          gr_vector_void_star& output_items)
{
    // auto in = static_cast<const input_type*>(input_items[0]);

    std::vector<gr::tag_t> tags;
    get_tags_in_range(tags,0,nitems_read(0),nitems_read(0)+noutput_items);
    for(auto& tag: tags){
        if(pmt::equal(tag.key,pmt::intern("tx_sob"))){
            d_time_now_start = (float)gr::high_res_timer_now()/
                                        (float)gr::high_res_timer_tps();
            d_start_diff = d_time_now_start - pmt::to_float(tag.value);
            d_start_off = tag.offset;
            std::cout<<"SOB time then: "<<pmt::to_float(tag.value)<<" now: "<<d_time_now_start<<"\n";

        }else if(pmt::equal(tag.key,pmt::intern("tx_eob"))){
            d_time_now_end = (float)gr::high_res_timer_now()/
                                        (float)gr::high_res_timer_tps();
            d_end_diff = d_time_now_end - pmt::to_float(tag.value);
            d_end_off = tag.offset;
            std::cout<<"SOB time then: "<<pmt::to_float(tag.value)<<" now: "<<d_time_now_end<<"\n";
            std::cout<<"SOB diff "<<d_start_diff<<", Off: "<<d_start_off
                        <<"\n EOB diff"<<d_end_off<<", Off: "<<d_end_off
                        <<"SOB-EOB diff time@Sink: "<<(d_time_now_end-d_time_now_start)
                        <<", off@Sink: "<<(d_end_off-d_start_off)<<"\n";
        }
        
    }
    return noutput_items;
}

} /* namespace tddModules */
} /* namespace gr */
