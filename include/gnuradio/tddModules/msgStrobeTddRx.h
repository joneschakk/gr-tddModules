/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULES_MSGSTROBETDDRX_H
#define INCLUDED_TDDMODULES_MSGSTROBETDDRX_H

#include <gnuradio/sync_block.h>
#include <gnuradio/tddModules/api.h>

namespace gr {
namespace tddModules {

/*!
 * \brief <+description of block+>
 * \ingroup tddModules
 *
 */
class TDDMODULES_API msgStrobeTddRx : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<msgStrobeTddRx> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of tddModules::msgStrobeTddRx.
     *
     * To avoid accidental use of raw pointers, tddModules::msgStrobeTddRx's
     * constructor is in a private implementation
     * class. tddModules::msgStrobeTddRx::make is the public interface for
     * creating new instances.
     */
    static sptr make(long switch_interval, long guard_time, bool st_mode);
};

} // namespace tddModules
} // namespace gr

#endif /* INCLUDED_TDDMODULES_MSGSTROBETDDRX_H */
