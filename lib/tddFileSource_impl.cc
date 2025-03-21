/* -*- c++ -*- */
/*
 * Copyright 2025 Jones.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "tddFileSource_impl.h"
#include <gnuradio/io_signature.h>


#include <gnuradio/thread/thread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdio>
#include <sstream>
#include <stdexcept>

#ifdef _MSC_VER
#define GR_FSEEK _fseeki64
#define GR_FTELL _ftelli64
#define GR_FSTAT _fstati64
#define GR_FILENO _fileno
#define GR_STAT _stati64
#define S_ISREG(m) (((m)&S_IFMT) == S_IFREG)
#else
#define GR_FSEEK fseeko
#define GR_FTELL ftello
#define GR_FSTAT fstat
#define GR_FILENO fileno
#define GR_STAT stat
#endif


namespace gr {
namespace tddModules {

tddFileSource::sptr tddFileSource::make(size_t itemsize, const char *filename,
                                        bool repeat, uint64_t offset,
                                        uint64_t len, float tx_time, long sampl_rate) {
  return gnuradio::make_block_sptr<tddFileSource_impl>(itemsize, filename,
                                                       repeat, offset, len, tx_time, sampl_rate);
}

/*
 * The private constructor
 */
tddFileSource_impl::tddFileSource_impl(size_t itemsize, const char *filename,
                                       bool repeat, uint64_t start_offset_items,
                                       uint64_t length_items, float tx_time, long sampl_rate)
    : gr::sync_block("tddFileSource", gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(1, 1, (itemsize))),
      d_itemsize(itemsize),
      d_start_offset_items(start_offset_items),
      d_length_items(length_items),
      d_fp(0),
      d_new_fp(0),
      d_repeat(repeat),
      d_updated(false),
      d_file_begin(true),
      d_repeat_cnt(0),
      d_add_begin_tag(pmt::PMT_NIL),
      d_mode(0) //maybe add a param later
{
  open(filename, repeat, start_offset_items, length_items);
  do_update();

  std::stringstream str;
  str << name() << unique_id();
  _id = pmt::string_to_symbol(str.str());

  d_items_in_burst = (u_int64_t)tx_time*sampl_rate;
  message_port_register_in(d_port_name);
  set_msg_handler(d_port_name,
    [this](const pmt::pmt_t& msg) { handler(msg); });
}

/*
 * Our virtual destructor.
 */
tddFileSource_impl::~tddFileSource_impl() {
  if (d_fp)
        fclose((FILE*)d_fp);
  if (d_new_fp)
      fclose((FILE*)d_new_fp);
}

void tddFileSource_impl::handler(pmt::pmt_t msg){
    gr::thread::scoped_lock lock(fp_mutex); // hold for the rest of this function
    if(pmt::equal(msg,pmt::intern("SWITCH"))){
      if(d_mode == 0){
          d_mode = 2;
      }else if(d_mode == 1){
          d_mode = 0;
      }
  }else if(pmt::equal(msg,pmt::intern("STOP"))){
      if(d_mode == 0){
          d_mode = 0;
      }else if(d_mode == 2){
          d_mode = 3;
      }
  }else{
      d_logger->alert("Msg Error");
  }
}

// bool tddFileSource_impl::seek(int64_t seek_point, int whence)
// {
//     if (d_seekable) {
//         seek_point += d_start_offset_items;
// 
//         switch (whence) {
//         case SEEK_SET:
//             break;
//         case SEEK_CUR:
//             seek_point += (d_length_items - d_items_remaining);
//             break;
//         case SEEK_END:
//             seek_point = d_length_items - seek_point;
//             break;
//         default:
//             d_logger->warn("bad seek mode {:d}", whence);
//             return 0;
//         }
// 
//         if ((seek_point < (int64_t)d_start_offset_items) ||
//             (seek_point > (int64_t)(d_start_offset_items + d_length_items - 1))) {
//             d_logger->warn("bad seek point {:d}", seek_point);
//             return 0;
//         }
//         d_items_remaining = d_length_items - (seek_point - d_start_offset_items);
//         return GR_FSEEK((FILE*)d_fp, seek_point * d_itemsize, SEEK_SET) == 0;
//     } else {
//         d_logger->warn("file not seekable");
//         return 0;
//     }
// }


void tddFileSource_impl::open(const char* filename,
                            bool repeat,
                            uint64_t start_offset_items,
                            uint64_t length_items)
{
    // obtain exclusive access for duration of this function
    gr::thread::scoped_lock lock(fp_mutex);

    if (d_new_fp) {
        fclose(d_new_fp);
        d_new_fp = 0;
    }

    if ((d_new_fp = fopen(filename, "rb")) == NULL) {
        d_logger->error("[fopen] {:s}: {:s}", filename, strerror(errno));
        throw std::runtime_error("can't open file");
    }

    struct GR_STAT st;

    if (GR_FSTAT(GR_FILENO(d_new_fp), &st)) {
        d_logger->error("[fstat] {:s}: {:s}", filename, strerror(errno));
        throw std::runtime_error("can't fstat file");
    }
    if (S_ISREG(st.st_mode)) {
        d_seekable = true;
    } else {
        d_seekable = false;
    }

    uint64_t file_size;

    if (d_seekable) {
        // Check to ensure the file will be consumed according to item size
        if (GR_FSEEK(d_new_fp, 0, SEEK_END) == -1) {
            throw std::runtime_error("can't fseek()");
        }
        file_size = GR_FTELL(d_new_fp);

        // Make sure there will be at least one item available
        if ((file_size / d_itemsize) < (start_offset_items + 1)) {
            if (start_offset_items) {
                d_logger->warn("file is too small for start offset: {:d} < {:d}",
                               file_size - 1,
                               start_offset_items * d_itemsize);
            } else {
                d_logger->warn("file is too small ({:d})", file_size);
            }
            fclose(d_new_fp);
            throw std::runtime_error("file is too small");
        }
    } else {
        file_size = INT64_MAX;
    }

    uint64_t items_available = (file_size / d_itemsize - start_offset_items);

    // If length is not specified, use the remainder of the file. Check alignment at end.
    if (length_items == 0) {
        length_items = items_available;
        if (d_seekable && (file_size % d_itemsize)) {
            d_logger->warn("file size is not a multiple of item size ({:d} ≠ N·{:d})",
                           file_size,
                           d_itemsize);
        }
    }

    // Check specified length. Warn and use available items instead of throwing an
    // exception.
    if (length_items > items_available) {
        d_logger->warn(
            "file too short, will read fewer than requested items: {:d} > {:d}",
            length_items,
            items_available);
        length_items = items_available;
    }

    // Rewind to start offset
    if (d_seekable) {
        auto start_offset = start_offset_items * d_itemsize;
#ifdef _POSIX_C_SOURCE
#if _POSIX_C_SOURCE >= 200112L
        // If supported, tell the OS that we'll be accessing the file sequentially
        // and that it would be a good idea to start prefetching it
        auto fd = fileno(d_new_fp);
        static const std::map<int, const std::string> fadv_errstrings = {
            { EBADF, "bad file descriptor" },
            { EINVAL, "invalid advise" },
            { ESPIPE, "tried to act as if a pipe or similar was a file" }
        };
        if (file_size && file_size != INT64_MAX) {
            if (auto ret = posix_fadvise(
                    fd, start_offset, file_size - start_offset, POSIX_FADV_SEQUENTIAL)) {
                d_logger->warn("failed to advise to read sequentially, {:s}",
                               fadv_errstrings.at(ret));
            }
            if (auto ret = posix_fadvise(
                    fd, start_offset, file_size - start_offset, POSIX_FADV_WILLNEED)) {
                d_logger->warn("failed to advise we'll need file contents soon, {:s}",
                               fadv_errstrings.at(ret));
            }
        }
#endif
#endif
        if (GR_FSEEK(d_new_fp, start_offset, SEEK_SET) == -1) {
            throw std::runtime_error("can't fseek()");
        }
    }

    d_updated = true;
    d_repeat = repeat;
    d_start_offset_items = start_offset_items;
    d_length_items = length_items;
    d_items_remaining = length_items;
    d_counter = 0;
}

// void tddFileSource_impl::close()
// {
//     // obtain exclusive access for duration of this function
//     gr::thread::scoped_lock lock(fp_mutex);
//     if (d_new_fp != NULL) {
//         fclose(d_new_fp);
//         d_new_fp = NULL;
//     }
//     d_updated = true;
// }

void tddFileSource_impl::do_update()
{
    if (d_updated) {
        gr::thread::scoped_lock lock(fp_mutex); // hold while in scope

        if (d_fp)
            fclose(d_fp);

        d_fp = d_new_fp; // install new file pointer
        d_new_fp = 0;
        d_updated = false;
        d_file_begin = true;
    }
}

void tddFileSource_impl::set_begin_tag(pmt::pmt_t val) { d_add_begin_tag = val; }

int tddFileSource_impl::work(int noutput_items,
                            gr_vector_const_void_star &input_items,
                            gr_vector_void_star &output_items) {
    char* o = (char*)output_items[0];
    // uint64_t size = std::min(noutput_items,d_items_in_burst); //if the items in the burst is smaller

    do_update(); // update d_fp is reqd
    if (d_fp == NULL)
        throw std::runtime_error("work with file not open");

    gr::thread::scoped_lock lock(fp_mutex); // hold for the rest of this function

    // No items remaining - all done
    if (d_items_remaining == 0) {
        return WORK_DONE;
    }else if (d_mode<2){
        return 0; //idle since its RX_mode
    }

    auto offset = nitems_written(0);
    if(d_mode == 3){ //should never come here as this is violate tx_time limit with 
                        //extra items in the buffer to be processed
        // std::cout<<"Yo EOB "<<d_mode<<" "<<d_counter<<"\n";
        noutput_items = 1920; //make it configurable 
        d_mode = 1;
        d_counter = 0;
    }else if(!d_counter){ //start of a burst
        auto time_now = (float)gr::high_res_timer_now()/
                                (float)gr::high_res_timer_tps();
        add_item_tag(0,(offset),sob_key,pmt::PMT_T);//pmt::from_float(time_now));//pmt::PMT_T
        
        if((noutput_items>=d_items_in_burst)||(noutput_items>=d_items_remaining)){ //tx time is so low that the num samples<buffer size
            noutput_items = std::min(d_items_in_burst,d_items_remaining);            //sob and eob are in one burst
            //tag offset is getting messed bcoz the end is not packet_len aligned
            //make it configurable 
            noutput_items = noutput_items + (1920- (noutput_items%1920));

            // std::cout<<"Yo SOB-EOB "<<time_now<<", "<<d_counter<<", "<<noutput_items<<", "<<offset<<"\n";
            d_mode = 1;
            d_counter = 0;            
        }
        else{
            d_counter+=noutput_items;
            // std::cout<<"Yo SOB "<<d_mode<<" "<<d_counter<<" "<<offset<<"\n";
        }
        // std::cout<<"Tot samp "<<d_total_samples<<"\n";
    }
    else if((d_counter+noutput_items)>=d_items_in_burst){
        noutput_items = d_items_in_burst - d_counter;
        //tag offset is getting messed bcoz the end is not packet_len aligned
        //make it configurable 1920
        noutput_items = noutput_items + (1920- (noutput_items%1920));
        // std::cout<<"Yo FullEOB "<<d_mode<<" "<<d_counter<<", "<<noutput_items<<" "<<offset<<"\n";
        d_mode = 1;
        d_counter = 0;
    }else{
        d_counter+=noutput_items; //in between sob and eob, where burst size>>buffer
        // std::cout<<"Yo normie "<<d_mode<<" "<<d_counter<<"\n";
    }
    d_items_remaining -= noutput_items; //decrement the items left in the file to be read
    size_t nitems_read = fread(o, d_itemsize, noutput_items, (FILE*)d_fp);
    if (noutput_items != nitems_read) {
        // Size of non-seekable files is unknown. EOF is normal.
        if (!d_seekable && feof((FILE*)d_fp)) {
            // size -= nitems_read;
            d_items_remaining = 0;
            noutput_items = nitems_read;
            d_mode = 1;
        }
        else
            throw std::runtime_error("fread error");
    }

    if(d_mode == 1){ //send eob
            auto time_now = (float)gr::high_res_timer_now()/
                                (float)gr::high_res_timer_tps();
            add_item_tag(0,offset+(noutput_items-1),eob_key,pmt::PMT_T);//pmt::from_float(time_now));
            // std::cout<<"Yo EEOB "<<d_mode<<" "<<d_counter<<", "<<time_now<<"\n";
    }
    
    //from the filesource.cc
    // while (size) {
    //     // Add stream tag whenever the file starts again
    //     // if (d_file_begin && d_add_begin_tag != pmt::PMT_NIL) {
    //     //     add_item_tag(0,
    //     //                   nitems_written(0) + noutput_items - size,
    //     //                   d_add_begin_tag,
    //     //                   pmt::from_long(d_repeat_cnt),
    //     //                   _id);
    //     //     d_file_begin = false;
    //     // }
    //     uint64_t nitems_to_read = std::min(size, d_items_remaining);
    //     size_t nitems_read = fread(o, d_itemsize, nitems_to_read, (FILE*)d_fp);
    //     if (nitems_to_read != nitems_read) {
    //         // Size of non-seekable files is unknown. EOF is normal.
    //         if (!d_seekable && feof((FILE*)d_fp)) {
    //             size -= nitems_read;
    //             d_items_remaining = 0;
    //             d_mode = 1;
    //             break;
    //         }
    //         throw std::runtime_error("fread error");
    //     }
    //     size -= nitems_read;
    //     d_items_remaining -= nitems_read;
    //     o += nitems_read * d_itemsize;
    //     // Ran out of items ("EOF")
    //     // if (d_items_remaining == 0) {
    //     //     // Repeat: rewind and request tag
    //     //     // if (d_repeat && d_seekable) {
    //     //     //     if (GR_FSEEK(d_fp, d_start_offset_items * d_itemsize, SEEK_SET) == -1) {
    //     //     //         throw std::runtime_error("can't fseek()");
    //     //     //     }
    //     //     //     d_items_remaining = d_length_items;
    //     //     //     if (d_add_begin_tag != pmt::PMT_NIL) {
    //     //     //         d_file_begin = true;
    //     //     //         d_repeat_cnt++;
    //     //     //     }
    //     //     // }
    //     //     // // No repeat: return
    //     //     // else {
    //     //     //     break;
    //     //     // }
    //     // }
    // }

    return (noutput_items);
}

} /* namespace tddModules */
} /* namespace gr */
