/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TDDMODULEPKG_TDDOFDMCYCLICPREFIX_H
#define INCLUDED_TDDMODULEPKG_TDDOFDMCYCLICPREFIX_H

#include <gnuradio/tagged_stream_block.h>
#include <gnuradio/tddModulePkg/api.h>

namespace gr {
namespace tddModulePkg {

/*!
 * \brief <+description of block+>
 * \ingroup tddModulePkg
 *
 */
class TDDMODULEPKG_API tddOFDMCyclicPrefix : virtual public gr::tagged_stream_block
{
public:
    typedef std::shared_ptr<tddOFDMCyclicPrefix> sptr;

  /*!
   * \brief Return a shared_ptr to a new instance of
   * tddModules::tdd_ofdm_cyclic_prefixer.
   *
   * To avoid accidental use of raw pointers,
   * tddModules::tdd_ofdm_cyclic_prefixer's constructor is in a private
   * implementation class. tddModules::tdd_ofdm_cyclic_prefixer::make is the
   * public interface for creating new instances.
   */
  // static sptr make(size_t input_size,
  //   size_t output_size,
  //   int rolloff_len = 0,
  //   const std::string& len_tag_key = "");

/*!
* \param fft_len IFFT length (i.e. length of the OFDM symbols).
* \param cp_lengths CP lengths. Wraps around after reaching the end.
* \param rolloff_len Length of the rolloff flank in samples.
* \param len_tag_key For framed processing the key of the length tag.
*/
    static sptr make(int fft_len,
                     const std::vector<int>& cp_lengths,
                     int rolloff_len = 0,
                     const std::string& len_tag_key = "");
};

} // namespace tddModulePkg
} // namespace gr

#endif /* INCLUDED_TDDMODULEPKG_TDDOFDMCYCLICPREFIX_H */
