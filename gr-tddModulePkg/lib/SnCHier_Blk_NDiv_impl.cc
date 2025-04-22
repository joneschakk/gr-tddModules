/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

 #include "SnCHier_Blk_NDiv_impl.h"
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
 
 SnCHier_Blk_NDiv::sptr
 SnCHier_Blk_NDiv::make(int fft_len, int cp_len, bool use_even_carriers)
 {
     return gnuradio::make_block_sptr<SnCHier_Blk_NDiv_impl>(
         fft_len, cp_len, use_even_carriers);
 }
 
 
 /*
  * The private constructor
  */
 SnCHier_Blk_NDiv_impl::SnCHier_Blk_NDiv_impl(int fft_len,
                                              int cp_len,
                                              bool use_even_carriers)
     : gr::hier_block2("SnCHier_Blk_NDiv",
                       gr::io_signature::make(
                           1 /* min inputs */, 1 /* max inputs */, sizeof(gr_complex)),
                       gr::io_signature::make3(
                           1 /* min outputs */, 3 /*max outputs */, sizeof(float),
                           sizeof(float),sizeof(float)))
 {
     std::vector<float> ma_taps(fft_len / 2, 1.0);
     gr::blocks::delay::sptr delay(
         gr::blocks::delay::make(sizeof(gr_complex), fft_len / 2));
     gr::blocks::conjugate_cc::sptr delay_conjugate(gr::blocks::conjugate_cc::make());
     gr::blocks::multiply_cc::sptr delay_corr(gr::blocks::multiply_cc::make());
     gr::filter::fir_filter_ccf::sptr delay_ma(gr::filter::fir_filter_ccf::make(
         1, std::vector<float>(fft_len / 2, use_even_carriers ? 1.0 : -1.0)));
     gr::blocks::complex_to_mag_squared::sptr delay_magsquare(
         gr::blocks::complex_to_mag_squared::make());
     gr::blocks::divide_ff::sptr delay_normalize(gr::blocks::divide_ff::make());
 
     gr::blocks::complex_to_mag_squared::sptr normalizer_magsquare(
         gr::blocks::complex_to_mag_squared::make());
     gr::filter::fir_filter_fff::sptr normalizer_ma(
         gr::filter::fir_filter_fff::make(1, std::vector<float>(fft_len, 0.5)));
     gr::blocks::multiply_ff::sptr normalizer_square(gr::blocks::multiply_ff::make());
 
     gr::blocks::complex_to_arg::sptr peak_to_angle(gr::blocks::complex_to_arg::make());
     gr::blocks::sample_and_hold_ff::sptr sample_and_hold(
         gr::blocks::sample_and_hold_ff::make());
 
     // gr::blocks::plateau_detector_fb::sptr plateau_detector(
     //     gr::blocks::plateau_detector_fb::make(cp_len, threshold));
 
     // store plateau detector for use in callback setting threshold
     // d_plateau_detector = plateau_detector;
 
     connect(self(), 0, delay, 0);
     connect(delay, 0, delay_conjugate, 0);
     connect(delay_conjugate, 0, delay_corr, 1);
     connect(self(), 0, delay_corr, 0);
     connect(delay_corr, 0, delay_ma, 0);
     connect(delay_ma, 0, delay_magsquare, 0);
     // connect(delay_magsquare, 0, delay_normalize, 0);
     connect(delay_magsquare, 0, self(), 1);
     // Energy Path
     connect(self(), 0, normalizer_magsquare, 0);
     connect(normalizer_magsquare, 0, normalizer_ma, 0);
     connect(normalizer_ma, 0, normalizer_square, 0);
     connect(normalizer_ma, 0, normalizer_square, 1);
     // connect(normalizer_square, 0, delay_normalize, 1);
     connect(normalizer_square, 0, self(), 2);
     // Fine frequency estimate (output 0)
     connect(delay_ma, 0, peak_to_angle, 0);
     // connect(peak_to_angle, 0, sample_and_hold, 0);
     connect(peak_to_angle, 0, self(), 0);
     // Peak detect (output 1)
     // connect(delay_normalize, 0, plateau_detector, 0);
     // connect(plateau_detector, 0, sample_and_hold, 1);
     // connect(plateau_detector, 0, self(), 1);
     // connect(delay_normalize, 0, self(), 2);
 }
 
 /*
  * Our virtual destructor.
  */
 SnCHier_Blk_NDiv_impl::~SnCHier_Blk_NDiv_impl() {}
 
 
 } /* namespace tddModulePkg */
 } /* namespace gr */
 