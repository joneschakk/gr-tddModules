/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

 #ifndef INCLUDED_TDDMODULEPKG_SNC_HIERBLK_IMPL_H
 #define INCLUDED_TDDMODULEPKG_SNC_HIERBLK_IMPL_H
 
 #include <gnuradio/tddModulePkg/SnC_HierBlk.h>
 #include <gnuradio/blocks/plateau_detector_fb.h>
 
 namespace gr {
 namespace tddModulePkg {
 
 class SnC_HierBlk_impl : public SnC_HierBlk
 {
 private:
     // Nothing to declare in this block.
     gr::blocks::plateau_detector_fb::sptr d_plateau_detector;
 
 public:
     SnC_HierBlk_impl(int fft_len, int cp_len, bool use_even_carriers, float threshold);
     ~SnC_HierBlk_impl();
 
     // Where all the action really happens
 };
 
 } // namespace tddModulePkg
 } // namespace gr
 
 #endif /* INCLUDED_TDDMODULEPKG_SNC_HIERBLK_IMPL_H */
 