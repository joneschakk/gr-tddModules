"""
Embedded Python Blocks:

Each time this file is saved, GRC will instantiate the first class it finds
to get ports and parameters of your block. The arguments to __init__  will
be the parameters. All of them are required to have default values!
"""

import numpy as np
from gnuradio import gr
import pmt
import time
from threading import Thread
from gnuradio import uhd


class blk(gr.basic_block):  # other base classes are basic_block, decim_block, interp_block
    """Embedded Python Block example - a simple multiply const"""

    def __init__(self, switching_interval=.05, guard_time=.01):  # only default arguments here
        """arguments to this function show up as parameters in GRC"""
        gr.basic_block.__init__(  #basic_block.__init__(
            self,
            name='Strobe SW/ST',   # will show up in GRC
            in_sig=None, #[np.byte],
            out_sig=None #[np.complex64]
        )
        # if an attribute with the same name as a parameter is found,
        # a callback is registered (properties work, too).
        self.switching_interval = switching_interval
        self.guard_time = guard_time
        self.enable = False
        self.msg_port_src = "sw_st_trigger"
        self.message_port_register_out(pmt.intern(self.msg_port_src))
        self.msg_port_usrp_sink = "usrp_trigger"
        self.message_port_register_out(pmt.intern(self.msg_port_usrp_sink))
        self.state = 0 #0-rx/1-tx
        self.t = Thread(target=self.runner, daemon=True)

    # def start(self):
    #     # t = Thread(target=self.run,args=(self,))
    #     self.t.start()

    # def stop(self):
    #     self.t.join()
    
    # def run(self,input_items, output_items):
    #     if input_items[0][0]:
    #         self.enable = 1

    #     while self.enable:
    #         time.sleep((self.switching_interval-self.guard_time))
    #         PMT_msg = pmt.intern("STOP")
    #         self.message_port_pub(pmt.intern(self.msg_port_name), PMT_msg)
    #         time.sleep((self.guard_time))
    #         PMT_msg = pmt.intern("SWITCH")
    #         self.message_port_pub(pmt.intern(self.msg_port_name), PMT_msg)
    def runner(self):
        print("In thread")
        while True:
            time.sleep((self.switching_interval-self.guard_time))
            PMT_msg = pmt.intern("STOP")
            self.message_port_pub(pmt.intern(self.msg_port_src), PMT_msg)
            time.sleep((self.guard_time))
            #send rx to start/stop stream 
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
            
            stream_cmd = pmt.make_dict()
            stream_cmd = pmt.dict_add(stream_cmd,pmt.to_pmt("stream_cmd"),stream_val)
            self.message_port_pub(pmt.intern(self.msg_port_usrp_sink), stream_cmd)

            PMT_msg = pmt.intern("SWITCH")
            self.message_port_pub(pmt.intern(self.msg_port_src), PMT_msg)
    
    def start(self):
        if not self.enable:
            self.enable = 1
        elif self.enable == 1:
            return 0
        print("Start thread")
        
        if self.enable:
            self.t.start()
        
        return 0

    def stop(self):
        self.t.join()

        return 0


    # def work(self, input_items, output_items):
    #     # print(input_items[0])
    #     # if input_items[0][0] and not self.enable:
    #     if not self.enable:
    #         self.enable = 1
    #     elif self.enable == 1:
    #         return 0
    #     print("Start thread")
        
    #     if self.enable:
    #         self.t.start()
    #     # while self.enable:
    #     #     time.sleep((self.switching_interval-self.guard_time))
    #     #     PMT_msg = pmt.intern("STOP")
    #     #     self.message_port_pub(pmt.intern(self.msg_port_name), PMT_msg)
    #     #     time.sleep((self.guard_time))
    #     #     PMT_msg = pmt.intern("SWITCH")
    #     #     self.message_port_pub(pmt.intern(self.msg_port_name), PMT_msg)

    #     # output_items[0][:] = input_items[0] * self.example_param
    #     # out = output_items[0]
    #     # return len(out)
    #     return 0
