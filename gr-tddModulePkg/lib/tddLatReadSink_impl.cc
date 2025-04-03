/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "tddLatReadSink_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace tddModulePkg {

tddLatReadSink::sptr tddLatReadSink::make(size_t item_size)
{
    return gnuradio::make_block_sptr<tddLatReadSink_impl>(item_size);
}


/*
 * The private constructor
 */
tddLatReadSink_impl::tddLatReadSink_impl(size_t item_size)
    : gr::sync_block("tddLatReadSink",
                     gr::io_signature::make(1, 1, item_size),
                     gr::io_signature::make(0, 0, 0))
{
}

/*
 * Our virtual destructor.
 */
tddLatReadSink_impl::~tddLatReadSink_impl() {}

int tddLatReadSink_impl::work(int noutput_items,
                              gr_vector_const_void_star& input_items,
                              gr_vector_void_star& output_items)
{
    std::vector<gr::tag_t> tags;
    get_tags_in_range(tags,0,nitems_read(0),nitems_read(0)+noutput_items);
    for(auto& tag: tags){
        if(pmt::equal(tag.key,pmt::intern("tx_sob"))){
            d_time_now_start = (float)gr::high_res_timer_now() ;///
                                        // (float)gr::high_res_timer_tps();
            d_start_diff = d_time_now_start - pmt::to_float(tag.value);
            d_start_off = tag.offset;
            std::cout<<"SOB time then: "<<pmt::to_float(tag.value)<<" now: "<<d_time_now_start<<
                            " "<<d_start_off<<"\n";

        }else if(pmt::equal(tag.key,pmt::intern("tx_eob"))){
            d_time_now_end = (float)gr::high_res_timer_now() ; ///
                                        // (float)gr::high_res_timer_tps();
            d_end_diff = d_time_now_end - pmt::to_float(tag.value);
            d_end_off = tag.offset;
            // std::cout<<"SOB time then: "<<pmt::to_float(tag.value)<<" now: "<<d_time_now_end<<"\n";
            std::cout<<"EOB time then: "<<pmt::to_float(tag.value)<<" now: "<<d_time_now_end<<"\n"
                    <<"SOB diff-time: "<<d_start_diff<<", Off: "<<d_start_off
                        <<"\nEOB diff-time: "<<d_end_diff<<", Off: "<<d_end_off
                        <<" SOB-EOB diff time@Sink: "<<(d_time_now_end-d_time_now_start)
                        <<", off@Sink: "<<(d_end_off-d_start_off)<<"\n\n";
        }        
    }
    return noutput_items;
}

} /* namespace tddModulePkg */
} /* namespace gr */
