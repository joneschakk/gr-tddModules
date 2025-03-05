/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULES_MSGSTROBETDDTX_H
#define INCLUDED_TDDMODULES_MSGSTROBETDDTX_H

#include <gnuradio/block.h>
#include <gnuradio/tddModules/api.h>

namespace gr {
namespace tddModules {

/*!
 * \brief <+description of block+>
 * \ingroup tddModules
 *
 */
class TDDMODULES_API msgStrobeTddTx : virtual public gr::block
{
public:
    typedef std::shared_ptr<msgStrobeTddTx> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of tddModules::msgStrobeTddTx.
     *
     * To avoid accidental use of raw pointers, tddModules::msgStrobeTddTx's
     * constructor is in a private implementation
     * class. tddModules::msgStrobeTddTx::make is the public interface for
     * creating new instances.
     */
    static sptr make(long switch_interval, long guard_time);
};

} // namespace tddModules
} // namespace gr

#endif /* INCLUDED_TDDMODULES_MSGSTROBETDDTX_H */
