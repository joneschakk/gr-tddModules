/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULEPKG_SNCHIER_BLK_NSNH_H
#define INCLUDED_TDDMODULEPKG_SNCHIER_BLK_NSNH_H

#include <gnuradio/hier_block2.h>
#include <gnuradio/tddModulePkg/api.h>

namespace gr {
namespace tddModulePkg {

/*!
 * \brief <+description of block+>
 * \ingroup tddModulePkg
 *
 */
class TDDMODULEPKG_API SnCHier_Blk_NSnH : virtual public gr::hier_block2
{
public:
    typedef std::shared_ptr<SnCHier_Blk_NSnH> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of tddModulePkg::SnCHier_Blk_NSnH.
     *
     * To avoid accidental use of raw pointers, tddModulePkg::SnCHier_Blk_NSnH's
     * constructor is in a private implementation
     * class. tddModulePkg::SnCHier_Blk_NSnH::make is the public interface for
     * creating new instances.
     */
    static sptr make(int fft_len, int cp_len, bool use_even_carriers = false);
};

} // namespace tddModulePkg
} // namespace gr

#endif /* INCLUDED_TDDMODULEPKG_SNCHIER_BLK_NSNH_H */
