/* -*- c++ -*- */
/*
 * Copyright 2021 Bastian Bloessl <mail@bastibl.net>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

 #include "nullSrcLat_impl.h"
 #include <gnuradio/io_signature.h>
 
 #include "tp.h"
 
 namespace gr {
 namespace tddModules {
 
nullSrcLat::sptr nullSrcLat::make(size_t item_size, uint64_t granularity)
 {
     return gnuradio::make_block_sptr<nullSrcLat_impl>(item_size, granularity);
 }
 
 
 /*
  * The private constructor
  */
 nullSrcLat_impl::nullSrcLat_impl(size_t item_size, uint64_t granularity)
     : d_granularity(granularity),
       d_item_size(item_size),
       gr::sync_block("null_source_latency",
                      gr::io_signature::make(0, 0, 0),
                      gr::io_signature::make(1, 1, item_size))
 {
 }
 
 /*
  * Our virtual destructor.
  */
 nullSrcLat_impl::~nullSrcLat_impl() {}
 
 int nullSrcLat_impl::work(int noutput_items,
                                    gr_vector_const_void_star& input_items,
                                    gr_vector_void_star& output_items)
 {
 
     void* out = static_cast<void*>(output_items[0]);
     memset(out, 0, noutput_items * d_item_size);
 
     uint64_t items = nitems_written(0);
     uint64_t before = items / d_granularity;
     uint64_t after = (items + noutput_items) / d_granularity;
     for(int i = 1; i <= (after - before); i++) {
         tracepoint(null_rand_latency, tx, unique_id(), before + i);
     }
 
     return noutput_items;
 }
 
 } /* namespace sched */
 } /* namespace gr */
 