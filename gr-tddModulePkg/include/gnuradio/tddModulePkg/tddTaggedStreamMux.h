/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULEPKG_TDDTAGGEDSTREAMMUX_H
#define INCLUDED_TDDMODULEPKG_TDDTAGGEDSTREAMMUX_H

#include <gnuradio/tagged_stream_block.h>
#include <gnuradio/tddModulePkg/api.h>

namespace gr {
namespace tddModulePkg {

/*!
 * \brief <+description of block+>
 * \ingroup tddModulePkg
 *
 */
class TDDMODULEPKG_API tddTaggedStreamMux : virtual public gr::tagged_stream_block
{
public:
    typedef std::shared_ptr<tddTaggedStreamMux> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of tddModulePkg::tddTaggedStreamMux.
     *
     * To avoid accidental use of raw pointers, tddModulePkg::tddTaggedStreamMux's
     * constructor is in a private implementation
     * class. tddModulePkg::tddTaggedStreamMux::make is the public interface for
     * creating new instances.
     */
    static sptr make(size_t itemsize,
                     const std::string& lengthtagname,
                     unsigned int tag_preserve_head_pos,
                     unsigned int header_len);
};

} // namespace tddModulePkg
} // namespace gr

#endif /* INCLUDED_TDDMODULEPKG_TDDTAGGEDSTREAMMUX_H */
