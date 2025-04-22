/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

 #include "SnCHier_Blk_Nmulcc_impl.h"
 #include <gnuradio/io_signature.h>
 
 #include <gnuradio/blocks/complex_to_arg.h>
 #include <gnuradio/blocks/complex_to_mag_squared.h>
 #include <gnuradio/blocks/conjugate_cc.h>
 #include <gnuradio/blocks/delay.h>
 #include <gnuradio/blocks/divide.h>
 #include <gnuradio/blocks/multiply.h>
 #include <gnuradio/blocks/sample_and_hold.h>
 #include <gnuradio/filter/fir_filter_blk.h>
 
 namespace gr {
 namespace tddModulePkg {
 
 SnCHier_Blk_Nmulcc::sptr SnCHier_Blk_Nmulcc::make()
 {
     return gnuradio::make_block_sptr<SnCHier_Blk_Nmulcc_impl>();
 }
 
 
 /*
  * The private constructor
  */
 SnCHier_Blk_Nmulcc_impl::SnCHier_Blk_Nmulcc_impl()
     : gr::hier_block2("SnCHier_Blk_Nmulcc",
                       gr::io_signature::make(
                           1 /* min inputs */, 1, sizeof(gr_complex)),
                       gr::io_signature::make2(
                           1, 1, sizeof(gr_complex), sizeof(float)))
 {
     // std::vector<float> ma_taps(fft_len / 2, 1.0);
     gr::blocks::delay::sptr delay(
         gr::blocks::delay::make(sizeof(gr_complex), 64 / 2));
     gr::blocks::conjugate_cc::sptr delay_conjugate(gr::blocks::conjugate_cc::make());
     
     gr::blocks::complex_to_mag_squared::sptr normalizer_magsquare(
         gr::blocks::complex_to_mag_squared::make());
     gr::filter::fir_filter_fff::sptr normalizer_ma(
         gr::filter::fir_filter_fff::make(1, std::vector<float>(64, 0.5)));
 
     connect(self(), 0, delay, 0);
     // connect other blocks
     connect(delay, 0, delay_conjugate, 0);
     connect(delay_conjugate, 0, self(), 0);
 
     connect(self(), 0, normalizer_magsquare, 0);
     connect(normalizer_magsquare, 0, normalizer_ma, 0);
     connect(normalizer_ma, 0, self(), 1);
 }
 
 /*
  * Our virtual destructor.
  */
 SnCHier_Blk_Nmulcc_impl::~SnCHier_Blk_Nmulcc_impl() {}
 
 
 } /* namespace tddModulePkg */
 } /* namespace gr */
 