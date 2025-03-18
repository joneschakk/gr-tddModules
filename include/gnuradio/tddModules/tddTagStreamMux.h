/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULES_TDDTAGSTREAMMUX_H
#define INCLUDED_TDDMODULES_TDDTAGSTREAMMUX_H

#include <gnuradio/tagged_stream_block.h>
#include <gnuradio/tddModules/api.h>

namespace gr {
namespace tddModules {

/*!
 * \brief <+description of block+>
 * \ingroup tddModules
 *
 */
class TDDMODULES_API tddTagStreamMux : virtual public gr::tagged_stream_block {
public:
  typedef std::shared_ptr<tddTagStreamMux> sptr;

  /*!
   * \brief Return a shared_ptr to a new instance of
   * tddModules::tddTagStreamMux.
   *
   * To avoid accidental use of raw pointers, tddModules::tddTagStreamMux's
   * constructor is in a private implementation
   * class. tddModules::tddTagStreamMux::make is the public interface for
   * creating new instances.
   */
  static sptr make(size_t itemsize, const std::string &lengthtagname,
                   unsigned int tag_preserve_head_pos);
};

} // namespace tddModules
} // namespace gr

#endif /* INCLUDED_TDDMODULES_TDDTAGSTREAMMUX_H */
