SMP2 Simulator for Hardware in the Loop
=======================================

This project is forked from [Burak Deniz's implementation of SMP (*Simulation Model Portability*) Model Development Kit](https://github.com/GITburakdeniz/smp-mdk) for open-source development of SMP2 models, mainly geared for academic research projects related to HIL (Hardware in the Loop) and OBC (On board computer) device and peripheral models.

The scheduler is attached to the computer clock instead of using a discrete event queue so Scheduler and TimeKeeper services behaviour is not fully compliant to the SMP2 standard. Save/restore and hold/resume are not available, but the partial implementation should be enough to run SMP2 models connected to HW interfaces. This is useful for many testing scenarios in which an SMP2 model needs to comunicate through SpW, 1553, CAN, I/O signals or any other protocolol with real hardware.

Differences from original author's code:

- Changed to CMake build system (WIP).
- Modified original code for compatibility with C++17 (removed throws).
- Added some examples from the official handbook (WIP).
- Added a minimal implementation of a simulation infrastructure: Scheduler, Logger, Time Keeper, etc. (WIP).

Instructions
------------

Requirements:

- C++ compiler
- CMake
- Boost

Build library and examples:

~~~bash
mkdir build
cd build
cmake ..
make && make install
~~~

Tested on Ubuntu 18.04.
    
Examples can be run from command line:

~~~bash
./examples/counter/counter
~~~

Related resources and references
--------------------------------

- [SMP 2.0 Handbook - EGOS-SIM-GEN-TN-0099 - Issue 1 Revision 2 - 28 October 2005](https://taste.tuxfamily.org/wiki/images/9/9a/SMP_2.0_Handbook_-_1.2.pdf). Accessed 2021-09-30.