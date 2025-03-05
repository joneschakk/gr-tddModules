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
import time
from threading import Thread

class msgStrobeTddRx_p(gr.sync_block):
    """
    docstring for block msgStrobeTddRx_p
    """
    def __init__(self, switch_interval = 2, guard_time = 0.3):
        gr.sync_block.__init__(self,
            name="msgStrobeTddRx_p",
            in_sig=[np.byte],
            out_sig=None)
        
        self.switching_interval = switch_interval
        self.guard_time = guard_time
        self.enable = 0
        self.msg_port_src = "sw_st_trigger"
        self.message_port_register_out(pmt.intern(self.msg_port_src))
        self.msg_port_usrp_sink = "usrp_trigger"
        self.message_port_register_out(pmt.intern(self.msg_port_usrp_sink))
        self.state = 0
        self.t = Thread(target=self.runner, daemon=True)

    def runner(self):
        print("In thread")
        while True:
            time.sleep((self.switching_interval-self.guard_time))
            PMT_msg = pmt.intern("STOP")
            self.message_port_pub(pmt.intern(self.msg_port_src), PMT_msg)
            #wait for guard time
            time.sleep((self.guard_time))

            stream_val = pmt.make_dict()
            if self.state:
                # PMT_msg = pmt.intern("")    #start recv as prev was tx                
                stream_val = pmt.dict_add(stream_val,pmt.to_pmt("stream_mode"),pmt.to_pmt("start_cont"))
                stream_val = pmt.dict_add(stream_val,pmt.to_pmt("stream_now"),pmt.PMT_T)
                # PMT_T
            else:
                # PMT_msg = pmt.intern("")    #stop recv as prev was rx
                stream_val = pmt.dict_add(stream_val,pmt.to_pmt("stream_mode"),pmt.to_pmt("stop_cont"))
                stream_val = pmt.dict_add(stream_val,pmt.to_pmt("stream_now"),pmt.PMT_T)
            self.state = (not self.state)   #state change
            #send stream start/stop
            stream_cmd = pmt.make_dict()
            stream_cmd = pmt.dict_add(stream_cmd,pmt.to_pmt("stream_cmd"),stream_val)
            self.message_port_pub(pmt.intern(self.msg_port_usrp_sink), stream_cmd)

            PMT_msg = pmt.intern("SWITCH")
            self.message_port_pub(pmt.intern(self.msg_port_src), PMT_msg)

            return 0
        
    def work(self, input_items, output_items):
        # print(input_items[0])
        if len(input_items[0]) and not self.enable:
            self.enable = 1
        elif self.enable == 1:
            #exit without doing anything
            return 0 #len(output_items[0])
        print("Start thread")
        
        if self.enable:
            self.t.start()

        return 0 #len(input_items[0])
