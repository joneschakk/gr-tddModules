/* -*- c++ -*- */
/*
 * Copyright 2025 jayc.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULES_NULLSRCLAT_H
#define INCLUDED_TDDMODULES_NULLSRCLAT_H

#include <gnuradio/sync_block.h>
#include <gnuradio/tddModules/api.h>

namespace gr {
namespace tddModules {

/*!
 * \brief <+description of block+>
 * \ingroup tddModules
 *
 */
class TDDMODULES_API nullSrcLat : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<nullSrcLat> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of tddModules::nullSrcLat.
     *
     * To avoid accidental use of raw pointers, tddModules::nullSrcLat's
     * constructor is in a private implementation
     * class. tddModules::nullSrcLat::make is the public interface for
     * creating new instances.
     */
    static sptr make(size_t item_size, uint64_t granularity);
};

} // namespace tddModules
} // namespace gr

#endif /* INCLUDED_TDDMODULES_NULLSRCLAT_H */
