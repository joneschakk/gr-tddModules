/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULEPKG_TDDMESSAGESTROBE_H
#define INCLUDED_TDDMODULEPKG_TDDMESSAGESTROBE_H

#include <gnuradio/sync_block.h>
#include <gnuradio/tddModulePkg/api.h>

namespace gr {
namespace tddModulePkg {

/*!
 * \brief <+description of block+>
 * \ingroup tddModulePkg
 *
 */
class TDDMODULEPKG_API tddMessageStrobe : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<tddMessageStrobe> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of tddModulePkg::tddMessageStrobe.
     *
     * To avoid accidental use of raw pointers, tddModulePkg::tddMessageStrobe's
     * constructor is in a private implementation
     * class. tddModulePkg::tddMessageStrobe::make is the public interface for
     * creating new instances.
     */
    static sptr
    make(size_t itemsize, float switch_interval = 100.0, float guard_time = 10.0, bool st_mode = 0);
};

} // namespace tddModulePkg
} // namespace gr

#endif /* INCLUDED_TDDMODULEPKG_TDDMESSAGESTROBE_H */
