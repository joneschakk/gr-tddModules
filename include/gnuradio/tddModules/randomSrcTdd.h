/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULES_RANDOMSRCTDD_H
#define INCLUDED_TDDMODULES_RANDOMSRCTDD_H

#include <gnuradio/sync_block.h>
#include <gnuradio/tddModules/api.h>

namespace gr {
namespace tddModules {

/*!
 * \brief <+description of block+>
 * \ingroup tddModules
 *
 */
class TDDMODULES_API randomSrcTdd : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<randomSrcTdd> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of tddModules::randomSrcTdd.
     *
     * To avoid accidental use of raw pointers, tddModules::randomSrcTdd's
     * constructor is in a private implementation
     * class. tddModules::randomSrcTdd::make is the public interface for
     * creating new instances.
     */
    static sptr make(float tx_time_interval = 1.0, long samples_per_sec = 1000000, int pkt_len = 1920, int mode = 1);
};

} // namespace tddModules
} // namespace gr

#endif /* INCLUDED_TDDMODULES_RANDOMSRCTDD_H */
