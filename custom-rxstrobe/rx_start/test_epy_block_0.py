"""
Embedded Python Blocks:

Each time this file is saved, GRC will instantiate the first class it finds
to get ports and parameters of your block. The arguments to __init__  will
be the parameters. All of them are required to have default values!
"""

import numpy as np
from gnuradio import gr
import time
import pmt

class blk(gr.sync_block):  # other base classes are basic_block, decim_block, interp_block
    """Embedded Python Block example - a simple multiply const"""

    def __init__(self, example_param=1.0):  # only default arguments here
        """arguments to this function show up as parameters in GRC"""
        gr.sync_block.__init__(
            self,
            name='TagLatSink',   # will show up in GRC
            in_sig=[np.complex64],
            out_sig=None #[np.complex64]
        )
        # if an attribute with the same name as a parameter is found,
        # a callback is registered (properties work, too).
        self.example_param = example_param
        self.start
        self.read_cntr = 0

    def work(self, input_items, output_items):
        """example: multiply with constant"""
        # output_items[0][:] = input_items[0] * self.example_param
        # tag = 0
        # out = output_items[0]
        # noutput_items = len(out)
        
        tag = self.get_tags_in_range(0,0,len(input_items[0]))
        # print(type(tag),tag)
        for x in tag:
            if x.key == pmt.intern("tx_sob"):
                self.start = time.time()
                start_diff = self.start - pmt.to_double(x.value)
                # print("Start time ",self.start," Diff ", start_diff, " Off",(self.read_cntr+x.offset))
            elif x.key == pmt.intern("tx_eob"):
                stop = time.time()
                stop_diff = stop - pmt.to_double(x.value)
                diff = stop - self.start
                # print("Stop time ",stop," Diff ",stop_diff," blk lat ",diff," Off",(self.read_cntr+x.offset))
        self.read_cntr+=len(input_items[0])
        return len(input_items[0])
