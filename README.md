# Implementing TDD
This repo has the folder structure of GNU Radio's [OOT](https://wiki.gnuradio.org/index.php/OutOfTreeModules) module. This is no accident as we are using some custom blocks to enable TDD(Time Division Duplex) in the flowgraphs created with GNU Radio Companion. The repo also contains 2 extra directories - `flowgraphs` which contains the two flowgraphs to be run in master-follower setup and `analysis` which has the logs and further analysis done while testing the TDD functionality.
Refer section [Analysis](#2-analysis) to understand more about the findings from the development and testing.
To enable TDD follow the below instructions.

## 1. How to use this in GNU Radio
### 1.1 OOT modules
To use the modules in your flowgraph run the following commands -
``` bash
git clone https://github.com/joneschakk/gr-tddModules.git
cd gr-tddModules
mkdir build
cmake ..
make -j$(nproc)
sudo make install
sudo ldconfig
```
After this the block should be visible when you search for the blocks in GRC.
The custom blocks are -
1. `tddRandomSrc` - this blocks generates random bytes in the multiples of the user specified packet length. By setting `TX time` and `Samples/sec` you can control the total samples generated during the transmission time, $`Total\,samples = TX\,time*Samples/sec`$.
2. `tddFileSrc` - TBD
3. `tddTaggedStreamMux` - this block corrects and moves the tag offset of `tx_sob` tag from start of the payload to the front of the header samples. This is a tweaked version of the [Tagged Stream Mux](https://wiki.gnuradio.org/index.php/Tagged_Stream_Mux) block from the GNU Radio's block collection.
4. `tddOFDMCyclicPrefix` - this block corrects and moves the `tx_eob` to the end of the last CP added by the block. `tx_eob` is used to signal the end of burst to the USRP device. Read more [here](https://wiki.gnuradio.org/index.php/USRP_Sink). This is a tweaked version of the [OFDM Cyclic Prefix](https://wiki.gnuradio.org/index.php/OFDM_Cyclic_Prefixer) block from the GNU Radio's block collection.
5. `tddMessageStrobe` - this block can be configure in either master or follower mode. In master mode the block will start the timer and sends message to initiate TX as soon as it is run. In follower mode the block needs to be connected to the end of RX flowgraph and, as soon as it receives the first data, it starts the timer and sends messages to toggle between RX/TX. This is how synchronisation is achieved between 2 transceivers. This block is similar to the [Message Strobe](https://wiki.gnuradio.org/index.php/Message_Strobe) block from the GNU Radio's block collection.
### 1.2 Flowgraphs
The above blocks can be used anywhere as you please. The `.grc` files in the flowgraphs directory is just an example of how it can be implemented. Use the appropriately named flowgraphs from the the folder (and edit the USRP IP addresses if required). The flowgraphs have 2 variables `tdd_half_period` which is either the UL/DL time in milliseconds and `guard_time` which is used to compute the $`TX\,time = tdd\_half\_period - guard\_time`$ is kept as safety. The allowed range is $`0<guard\_time<tdd\_half\_period`$ and this variable is used so that the block `tddRandomSrc` will only generate samples $`\le TX\,time*Samples/sec =(tdd\_half\_period - guard\_time)*Samples/sec`$. Change these values as required and once the setup is complete, run the `tdd_follower.grc` first and then once the flowgraph is up and running, start `tdd_master.grc`
## 2. Analysis
For capturing and analysing the performance of the communication link we use the Control Port exposed by GNU Radio. More information about Control Port and Performance Counters and how to use this is explained [here](#22-gr_perf_analyser)
### 2.1 Test results
We tested TDD with the transceivers `tdd_master` and `tdd_follower` and below is the data rate observed for different UL/DL time periods - 
1. one-way, no switch -> 34.4Mbps
2. 1 second -> 34.4Mbps
3. 500ms -> 16Mbps
4. 100ms (guard_time = 10ms) -> 16Mbps
5. 50ms (5ms) -> 16Mbps 
6. 20ms (5ms) -> ~16Mbps
7. 10ms (1ms) -> 12-16Mbps
8. 5ms (1ms) -> 12-16Mbps, but with some **U** at USRP sink
9. 1ms -> ~12Mbps, with header parser errors
### 2.2 gr_perf_analyser
A python script to poll values from the ControlPort in GNU Radio and saves it as a python-dict log. This can be re-used to plot the performance counters in various ways. The behaviour of the script can be tweaked from the `perf_config.yaml` file.

Currently the script supports -
- plotting 2D graphs
    - in a snapshot view i.e., values of all the blocks for a particular timestamp (performance counter value vs block no.)
    - block view i.e., all performance counters of a specified block for the duration of the saved log (performance counter value vs time)
- plotting 3D graphs
    - latency/total time of all the blocks across the duration of the saved log (block no. vs latency vs time)
    - input & output buffer occupancy of all blocks across the duration of the saved log (block no. vs buffer occupancy vs time)
#### Performance Counters
Every GNU Radio block that is written with `gr::block` inherits the performance counters and can be read with the help of an interface like Apache Thrift. Currently GNU Radio supports average, instantaneous and variance values for different performance counters for every block -
- average - avg input % full, avg noutput_items, avg nproduced, avg output % full, avg throughput, avg work time
- instantaneous - input % full, noutput_items, nproduced, output % full, work time, total work time
- variance - var input % full, var noutput_items, var nproduced, var output % full, var work time More information on the GNU Radio [wiki](https://wiki.gnuradio.org/index.php/Performance_Counters)
#### Requirements
##### 1. Apache Thrift
For the script to read values from Control Port, Apache Thrift should be installed before building GNU Radio. Apache Thrift version 0.13.0 is the one that is currently tested. Checkout the v0.13.0 tag from the [git repo](https://github.com/apache/thrift/tree/v0.13.0) and follow the instructions in the installation [page](https://thrift.apache.org/docs/BuildingFromSource). Also make sure to install the [required packages](https://thrift.apache.org/docs/install/) beforehand.
##### 2. Additional packages
Make sure to update matplotlib to atleast v3.10. A simple `pip install matplotlib` should be enough.
##### 3. GNU Radio
Now build GNU Radio from source with `-DENABLE_CTRLPORT_THRIFT=ON` and `-DENABLE_PERFORMANCE_COUNTERS=ON` flags when running the cmake command. For more information refer installation [page](https://wiki.gnuradio.org/index.php?title=LinuxInstall#From_Source).

```shell
cmake -DCMAKE_BUILD_TYPE=Release -DPYTHON_EXECUTABLE=/usr/bin/python3 -DENABLE_CTRLPORT_THRIFT=ON -DENABLE_PERFORMANCE_COUNTERS=ON ../
```

After the install, edit the config file to enable Control Port and Performance Counters at /usr/local/etc/gnuradio/conf.d/gnuradio-runtime.conf This is how the changed values should look like -

```
[PerfCounters]
on = 1
export = 1
clock = thread
#clock = monotonic

[ControlPort]
on = 1
edges_list = 1
```

#### Configuration
- The tool can be configured to run as a logger or as a plotter by changing `type_of_operation: plot`
- The logfile to either save or read data can be given as `logfile_path: /path/to/logfile`
#### Plotter
Plotter needs the hostname, port number to connect to the Thrift server and should be changed once GNU Radio starts the server and the values are available.
#### Logger
- Logger can be configure to display
    - 2D plots for either avg, inst or var value types. If
        - `all_blocks: 1` then it will give a snapshot of all the blocks for every performance counter.
        - the parameter is set as '0', you need set `search_blk: 43` to a block ID and it will display the performance counters for the entire duration of saved log.
    - 3D plots to see instantaneous latency, total work time or buffer values for all the blocks for the entire duration of saved log.
