#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2025 Jones.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#


import numpy as np
from gnuradio import gr
import pmt

#Atx 1	    	Brx 0
#Atx 2/1		Brx 0
#strobe-STOP
#Arx 2/3		Brx 0
#strobe-SWITCH
#Arx 0        	Brx 1
#Arx 0      	Brx 2/1
#strobe-STOP
#Arx 0  	    Brx 2/3
#strobe-SWITCH
#Arx 1  	    Brx 0

class randomToggleSource_p(gr.sync_block):
    """
    docstring for block randomToggleSource_p
    """
    def __init__(self, tx_time_interval = 1.0, samples_per_sec = 1e6, mode = 1):
        gr.sync_block.__init__(self,
            name="randomToggleSource_p",
            in_sig=None,
            out_sig=[np.byte])
        
        self.sampl_rate = samples_per_sec
        self.tx_time = tx_time_interval
        self.counter = 0
        self.enable = 0 #2-tx/0-rx
        self.tot_sampl = self.tx_time*self.sampl_rate #838860800 #tx_time*sampl_rate
        self.eob_tag_key = "tx_eob"
        self.sob_tag_key = "tx_sob"
        self.time_tag_key = "tx_time"
        if mode:
            self.enable = 2
        self.msg_port_name = "src_pause"
        self.message_port_register_in(pmt.intern(self.msg_port_name))
        self.set_msg_handler(pmt.intern(self.msg_port_name),self.handler)

    def handler(self, msg):
        #pass
        # print("tx-rndsrc msg",self.counter,", ",self.enable,", ", msg)
        message = pmt.to_python(msg)
        if msg == pmt.intern("SWITCH"):
            if self.enable == 0:
                self.enable = 2 #from rx -> tx
            elif self.enable == 1:
                self.enable = 0 #from eob sent mode -> rx

        elif msg == pmt.intern("STOP"):
            if self.enable == 0:
                self.enable = 0
            elif self.enable == 2:
                self.enable = 3 #need to send eob asap
            #for 1 ie eob already sent keep it as is, flip it when switch msg arrives
            #for 0 ie rx mode keep as is & flip when switch msg comes            
        else:
            print("MSG Dont know")

        return 0

    def work(self, input_items, output_items):
        # print("tx-rndsrc-entr",self.counter,", ",self.enable)
        if self.enable < 2:
            # print("tx-rndsrc-entr AA",self.counter,", ",self.enable)
            return 0
        
        out = output_items[0]
        noutput_items = len(out)
        offset = self.nitems_written(0)
        self.counter += noutput_items
        
        if (self.counter) >= self.tot_sampl or self.enable == 3:
            # print("tx-rndsrc-eob",self.counter,", ",self.enable)
            # output_items[0][:] =  np.ones(noutput_items, dtype=np.byte) #(self.tot_sampl-self.counter), dtype=np.byte)
            if offset:                
                if self.enable == 3:
                    noutput_items = 1
                else:
                    noutput_items -= (self.counter - self.tot_sampl)
                
                self.add_item_tag(0, int(offset + noutput_items -1), #(self.tot_sampl-self.counter) - 1,
                    pmt.string_to_symbol(self.eob_tag_key),pmt.PMT_T)
                output_items[0] =  np.random.randint(-128,127,int(noutput_items), dtype=np.byte)
            else:
                noutput_items = 0
            self.enable = 1 #eob sent
            self.counter = 0
        else:
            if self.counter == noutput_items:
                # print("tx-rndsrc-sob",self.counter,", ",self.enable)
                self.add_item_tag(0, offset,
                    pmt.string_to_symbol(self.sob_tag_key),
                    pmt.PMT_T)
            if noutput_items >= self.tot_sampl:
                noutput_items = self.tot_sampl
            output_items[0][:] =  np.random.randint(-128,127,int(noutput_items), dtype=np.byte)

        return int(noutput_items) #len(output_items[0])