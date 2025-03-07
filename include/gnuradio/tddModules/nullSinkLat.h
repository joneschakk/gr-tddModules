/* -*- c++ -*- */
/*
 * Copyright 2025 jayc.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULES_NULLSINKLAT_H
#define INCLUDED_TDDMODULES_NULLSINKLAT_H

#include <gnuradio/sync_block.h>
#include <gnuradio/tddModules/api.h>

namespace gr {
namespace tddModules {

/*!
 * \brief <+description of block+>
 * \ingroup tddModules
 *
 */
class TDDMODULES_API nullSinkLat : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<nullSinkLat> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of tddModules::nullSinkLat.
     *
     * To avoid accidental use of raw pointers, tddModules::nullSinkLat's
     * constructor is in a private implementation
     * class. tddModules::nullSinkLat::make is the public interface for
     * creating new instances.
     */
    static sptr make(size_t item_size, uint64_t granularity);
};

} // namespace tddModules
} // namespace gr

#endif /* INCLUDED_TDDMODULES_NULLSINKLAT_H */
