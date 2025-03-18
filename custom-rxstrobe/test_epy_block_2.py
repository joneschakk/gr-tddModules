"""
Embedded Python Blocks:

Each time this file is saved, GRC will instantiate the first class it finds
to get ports and parameters of your block. The arguments to __init__  will
be the parameters. All of them are required to have default values!
"""

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


class blk(gr.sync_block):  # other base classes are basic_block, decim_block, interp_block
    """Embedded Python Block example - a simple multiply const"""

    def __init__(self, sampl_rate=1e6, tx_time = 1):  # only default arguments here
        """arguments to this function show up as parameters in GRC"""
        gr.sync_block.__init__(
            self,
            name='Random Source',   # will show up in GRC
            in_sig=None, #[np.complex64],
            out_sig=[np.byte]
        )
        # if an attribute with the same name as a parameter is found,
        # a callback is registered (properties work, too).
        self.sampl_rate = sampl_rate
        self.tx_time = tx_time
        self.counter = 0
        self.enable = 0 #1-tx/0-rx
        self.tot_sampl = tx_time*sampl_rate #838860800 #tx_time*sampl_rate
        self.eob_tag_key = "tx_eob"
        self.sob_tag_key = "tx_sob"
        self.time_tag_key = "tx_time"

        self.msg_port_name = "file_pause"
        self.message_port_register_in(pmt.intern(self.msg_port_name))
        self.set_msg_handler(pmt.intern(self.msg_port_name),self.handler)

    def handler(self, msg):
        #pass
        print("tx-rndsrc msg",self.counter,", ",self.enable,", ", msg)
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
            
            
            # if self.enable == 2:
            #     self.enable = 0
            # elif self.enable == 0:
            #     self.enable = 1
            

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
            print("tx-rndsrc-eob",self.counter,", ",self.enable)
            # output_items[0][:] =  np.ones(noutput_items, dtype=np.byte) #(self.tot_sampl-self.counter), dtype=np.byte)
            if offset:
                self.add_item_tag(0, int(offset -1), #(self.tot_sampl-self.counter) - 1,
                    pmt.string_to_symbol(self.eob_tag_key),
                    pmt.PMT_T)
                output_items[0][:] =  np.random.randint(-128,127,1, dtype=np.byte)
            self.enable = 1#eob sent
            self.counter = 0
            # noutput_items = 1
        else:
            if self.counter == noutput_items:
                print("tx-rndsrc-sob",self.counter,", ",self.enable)
                self.add_item_tag(0, offset,
                    pmt.string_to_symbol(self.sob_tag_key),
                    pmt.PMT_T)
            if noutput_items >= self.tot_sampl:
                noutput_items = self.tot_sampl
            output_items[0][:] =  np.random.randint(-128,127,noutput_items, dtype=np.byte)
        # else:
        # print("tx-rndsrc-nrm",self.counter,", ",self.enable)
        # self.counter += noutput_items
        

        return noutput_items #len(output_items[0])
