/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULES_TDDFILESOURCE_H
#define INCLUDED_TDDMODULES_TDDFILESOURCE_H

#include <gnuradio/sync_block.h>
#include <gnuradio/tddModules/api.h>

namespace gr {
namespace tddModules {

/*!
 * \brief <+description of block+>
 * \ingroup tddModules
 *
 */
class TDDMODULES_API tddFileSource : virtual public gr::sync_block {
public:
  typedef std::shared_ptr<tddFileSource> sptr;

  /*!
   * \brief Return a shared_ptr to a new instance of tddModules::tddFileSource.
   *
   * To avoid accidental use of raw pointers, tddModules::tddFileSource's
   * constructor is in a private implementation
   * class. tddModules::tddFileSource::make is the public interface for
   * creating new instances.
   */
  static sptr make(size_t itemsize, const char *filename, bool repeat = false,
                   uint64_t offset = 0, uint64_t len = 0);
};

} // namespace tddModules
} // namespace gr

#endif /* INCLUDED_TDDMODULES_TDDFILESOURCE_H */
