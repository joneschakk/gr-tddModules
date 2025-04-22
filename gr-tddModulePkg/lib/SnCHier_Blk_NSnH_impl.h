/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULEPKG_SNCHIER_BLK_NSNH_IMPL_H
#define INCLUDED_TDDMODULEPKG_SNCHIER_BLK_NSNH_IMPL_H

#include <gnuradio/tddModulePkg/SnCHier_Blk_NSnH.h>
#include <gnuradio/blocks/plateau_detector_fb.h>

namespace gr {
namespace tddModulePkg {

class SnCHier_Blk_NSnH_impl : public SnCHier_Blk_NSnH
{
private:
    // Nothing to declare in this block.
    gr::blocks::plateau_detector_fb::sptr d_plateau_detector;

public:
    SnCHier_Blk_NSnH_impl(int fft_len, int cp_len, bool use_even_carriers);
    ~SnCHier_Blk_NSnH_impl();

    // Where all the action really happens
};

} // namespace tddModulePkg
} // namespace gr

#endif /* INCLUDED_TDDMODULEPKG_SNCHIER_BLK_NSNH_IMPL_H */
