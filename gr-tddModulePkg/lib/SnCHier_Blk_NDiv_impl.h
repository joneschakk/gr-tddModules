/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULEPKG_SNCHIER_BLK_NDIV_IMPL_H
#define INCLUDED_TDDMODULEPKG_SNCHIER_BLK_NDIV_IMPL_H

#include <gnuradio/tddModulePkg/SnCHier_Blk_NDiv.h>

namespace gr {
namespace tddModulePkg {

class SnCHier_Blk_NDiv_impl : public SnCHier_Blk_NDiv
{
private:
    // Nothing to declare in this block.

public:
    SnCHier_Blk_NDiv_impl(int fft_len, int cp_len, bool use_even_carriers);
    ~SnCHier_Blk_NDiv_impl();

    // Where all the action really happens
};

} // namespace tddModulePkg
} // namespace gr

#endif /* INCLUDED_TDDMODULEPKG_SNCHIER_BLK_NDIV_IMPL_H */
