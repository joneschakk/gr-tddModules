/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULES_TDDFILESOURCE_IMPL_H
#define INCLUDED_TDDMODULES_TDDFILESOURCE_IMPL_H

#include <gnuradio/tddModules/tddFileSource.h>

namespace gr {
namespace tddModules {

class tddFileSource_impl : public tddFileSource {
  private:
    const size_t d_itemsize;
    uint64_t d_start_offset_items;
    uint64_t d_length_items;
    uint64_t d_items_remaining;
    FILE* d_fp;
    FILE* d_new_fp;
    bool d_repeat;
    bool d_updated;
    bool d_file_begin;
    bool d_seekable;
    long d_repeat_cnt;
    pmt::pmt_t d_add_begin_tag;

    gr::thread::mutex fp_mutex;
    pmt::pmt_t _id;

    void do_update();

public:
  tddFileSource_impl(size_t itemsize, const char *filename, bool repeat,
                     uint64_t offset, uint64_t len);
  ~tddFileSource_impl();

  // Where all the action really happens
  int work(int noutput_items, gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

  bool seek(int64_t seek_point, int whence) override;
  void open(const char* filename, bool repeat, uint64_t offset, uint64_t len) override;
  void close() override;

  int work(int noutput_items,
            gr_vector_const_void_star& input_items,
            gr_vector_void_star& output_items) override;

  void set_begin_tag(pmt::pmt_t val) override;
};

} // namespace tddModules
} // namespace gr

#endif /* INCLUDED_TDDMODULES_TDDFILESOURCE_IMPL_H */
