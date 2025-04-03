/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULEPKG_TDDLATREADSINK_H
#define INCLUDED_TDDMODULEPKG_TDDLATREADSINK_H

#include <gnuradio/sync_block.h>
#include <gnuradio/tddModulePkg/api.h>

namespace gr {
namespace tddModulePkg {

/*!
 * \brief <+description of block+>
 * \ingroup tddModulePkg
 *
 */
class TDDMODULEPKG_API tddLatReadSink : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<tddLatReadSink> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of tddModulePkg::tddLatReadSink.
     *
     * To avoid accidental use of raw pointers, tddModulePkg::tddLatReadSink's
     * constructor is in a private implementation
     * class. tddModulePkg::tddLatReadSink::make is the public interface for
     * creating new instances.
     */
    static sptr make(size_t item_size);
};

} // namespace tddModulePkg
} // namespace gr

#endif /* INCLUDED_TDDMODULEPKG_TDDLATREADSINK_H */
