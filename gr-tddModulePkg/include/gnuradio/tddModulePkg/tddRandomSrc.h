/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULEPKG_TDDRANDOMSRC_H
#define INCLUDED_TDDMODULEPKG_TDDRANDOMSRC_H

#include <gnuradio/sync_block.h>
#include <gnuradio/tddModulePkg/api.h>

namespace gr {
namespace tddModulePkg {

/*!
 * \brief <+description of block+>
 * \ingroup tddModulePkg
 *
 */
class TDDMODULEPKG_API tddRandomSrc : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<tddRandomSrc> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of tddModulePkg::tddRandomSrc.
     *
     * To avoid accidental use of raw pointers, tddModulePkg::tddRandomSrc's
     * constructor is in a private implementation
     * class. tddModulePkg::tddRandomSrc::make is the public interface for
     * creating new instances.
     */
    static sptr make(float tx_time_interval = 100.0,
                     long samples_per_sec = 1000000,
                     long pkt_len = 1920,
                     bool mode = 1);
};

} // namespace tddModulePkg
} // namespace gr

#endif /* INCLUDED_TDDMODULEPKG_TDDRANDOMSRC_H */
