/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULES_LATTAGSINK_H
#define INCLUDED_TDDMODULES_LATTAGSINK_H

#include <gnuradio/sync_block.h>
#include <gnuradio/tddModules/api.h>

namespace gr {
namespace tddModules {

/*!
 * \brief <+description of block+>
 * \ingroup tddModules
 *
 */
class TDDMODULES_API latTagSink : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<latTagSink> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of tddModules::latTagSink.
     *
     * To avoid accidental use of raw pointers, tddModules::latTagSink's
     * constructor is in a private implementation
     * class. tddModules::latTagSink::make is the public interface for
     * creating new instances.
     */
    static sptr make(size_t item_size);
};

} // namespace tddModules
} // namespace gr

#endif /* INCLUDED_TDDMODULES_LATTAGSINK_H */
