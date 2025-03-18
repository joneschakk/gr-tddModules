/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "tddTagStreamMux_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace tddModules {

tddTagStreamMux::sptr
tddTagStreamMux::make(size_t itemsize, const std::string &lengthtagname,
                      unsigned int tag_preserve_head_pos) {
  return gnuradio::make_block_sptr<tddTagStreamMux_impl>(
      itemsize, lengthtagname, tag_preserve_head_pos);
}

/*
 * The private constructor
 */
tddTagStreamMux_impl::tddTagStreamMux_impl(size_t itemsize, 
                                            const std::string &lengthtagname,
                                            unsigned int tag_preserve_head_pos)
  : gr::tagged_stream_block("tddTagStreamMux",gr::io_signature::make(1, -1 ,sizeof(itemsize)),
    gr::io_signature::make(1, 1,sizeof(itemsize)),
    lengthtagname),
    d_itemsize(itemsize),
    d_tag_preserve_head_pos(tag_preserve_head_pos)
{
  
  set_tag_propagation_policy(TPP_DONT);

}

/*
 * Our virtual destructor.
 */
tddTagStreamMux_impl::~tddTagStreamMux_impl() {}

int tddTagStreamMux_impl::calculate_output_stream_length(const gr_vector_int &ninput_items) {
  int nout = 0;
  for (unsigned i = 0; i < ninput_items.size(); i++) {
      nout += ninput_items[i];
  }
  return nout;
}

int tddTagStreamMux_impl::work(int noutput_items, gr_vector_int &ninput_items,
                               gr_vector_const_void_star &input_items,
                               gr_vector_void_star &output_items) {
  unsigned char* out = (unsigned char*)output_items[0];
  int n_produced = 0;

  set_relative_rate((uint64_t)ninput_items.size(), 1);

  for (unsigned int i = 0; i < input_items.size(); i++) {
      const unsigned char* in = (const unsigned char*)input_items[i];

      std::vector<tag_t> tags;
      get_tags_in_range(tags, i, nitems_read(i), nitems_read(i) + ninput_items[i]);
      for (unsigned int j = 0; j < tags.size(); j++) {
          uint64_t offset =
              tags[j].offset - nitems_read(i) + nitems_written(0) + n_produced;
          // if (i == d_tag_preserve_head_pos && tags[j].offset == nitems_read(i)) {
          //     offset -= n_produced;
          // }
          if(pmt::equal(tags[j].key,pmt::intern("tx_sob"))){
            offset-=48;
          }
          add_item_tag(0, offset, tags[j].key, tags[j].value);
      }
      memcpy((void*)out, (const void*)in, ninput_items[i] * d_itemsize);
      out += ninput_items[i] * d_itemsize;
      n_produced += ninput_items[i];
  }

  return n_produced;
}

} /* namespace tddModules */
} /* namespace gr */
