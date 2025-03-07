/* -*- c++ -*- */
/*
 * Copyright 2025 jayc.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "nullSinkLat_impl.h"
#include <gnuradio/io_signature.h>
#include <tp.h>

namespace gr {
namespace tddModules {

nullSinkLat::sptr nullSinkLat::make(size_t item_size, uint64_t granularity)
{
    return gnuradio::make_block_sptr<nullSinkLat_impl>(item_size, granularity);
}


/*
 * The private constructor
 */
nullSinkLat_impl::nullSinkLat_impl(size_t item_size, uint64_t granularity)
    : gr::sync_block("nullSinkLat",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, (item_size)),
                     gr::io_signature::make(0, 0, 0)),
                    d_granularity(granularity),
                    d_item_size(item_size)
{
}

/*
 * Our virtual destructor.
 */
nullSinkLat_impl::~nullSinkLat_impl() {}

int nullSinkLat_impl::work(int noutput_items,
                           gr_vector_const_void_star& input_items,
                           gr_vector_void_star& output_items)
{
    uint64_t items = nitems_read(0);
    uint64_t before = items / d_granularity;
    uint64_t after = (items + noutput_items) / d_granularity;
    for(int i = 1; i <= (after - before); i++) {
        tracepoint(null_rand_latency, rx, unique_id(), before + i);
    }

    return noutput_items;
}

} /* namespace tddModules */
} /* namespace gr */
