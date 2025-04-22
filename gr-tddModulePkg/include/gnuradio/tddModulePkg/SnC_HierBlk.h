/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULEPKG_SNC_HIERBLK_H
#define INCLUDED_TDDMODULEPKG_SNC_HIERBLK_H

#include <gnuradio/hier_block2.h>
#include <gnuradio/tddModulePkg/api.h>

namespace gr {
namespace tddModulePkg {

/*!
 * \brief <+description of block+>
 * \ingroup tddModulePkg
 *
 */
class TDDMODULEPKG_API SnC_HierBlk : virtual public gr::hier_block2
{
public:
    typedef std::shared_ptr<SnC_HierBlk> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of tddModulePkg::SnC_HierBlk.
     *
     * To avoid accidental use of raw pointers, tddModulePkg::SnC_HierBlk's
     * constructor is in a private implementation
     * class. tddModulePkg::SnC_HierBlk::make is the public interface for
     * creating new instances.
     */
    static sptr
    make(int fft_len, int cp_len, bool use_even_carriers = false, float threshold = 0.9);
};

} // namespace tddModulePkg
} // namespace gr

#endif /* INCLUDED_TDDMODULEPKG_SNC_HIERBLK_H */
