Simple RISC-V Simulation with Spike and Rocket-Chip
(c) 2019-2020 W. Rhett Davis

This simple Makefile demonstrates the compilation and simulation of
a simple RISC-V program with both the Spike simulator and the 
Rocket-Chip emulator.  The provided "li.c" program is intended to 
show a simplie example of how to measure the cycles-per-instruction
and cycles-per-second simulation performace in the context of a 
single "load-immediate" instruction.  The example can be easily
extended to more complex programs.

In this example, console output is printed to STDOUT, along with the
contents of the Machine Performance Counters mcycle and minstret,
provided that the setStats() function is properly used to start and
stop these counters, as shown in li.c.  See Section 3.1.15 of the
RISC-V ISA Manual Vol. II: Privileged Architecture (v. 1.9) for more
information on these performance counters.

Quick Start Instructions:
$ source addsetup.sh  (to set up environment for NC State EOS system)
$ source varsetup.sh  (to set up environment for NC State EOS system)
$ make
$ make sim
$ make vsim

Makefile Targets:

default:  Generates $(PROGNAME).riscv, which is the elf-file binary for
          executing the program.  This target also generates 
          $(PROGNAME).riscv.dump, which is a human-readable version of
          the binary file contents.

sim:      Executes $(PROGNAME).riscv with the Spike Instruction-Set 
          Simulator.  In addition to the console output given above,
          the disassembled instruction trace is printed in 
          $(PROGNAME).spike.out along with the output of the Linux
          "time" command.

vsim:     Same as the "sim" target above, but uses the Verilog
          simulation executable compiled from the Rocket-Chip
          Verilator emulator.  The disassembled instruction trace is 
          printed in $(PROGNAME).emulator.out.

vcd:	  Same as the "vsim" target above, but also generates
          a Value Change Dump (VCD) file for the simulation.

vpd:	  Converts the VCD to a Synopsys compressed "VCDPlus" or VPD file.

view:	  Launches Synopsys DVE to view waveforms.

clean:    Deleted the output from the targets listed above.


Remember to change the PROGNAME variable in the Makefile when running
other programs.  Remember also to change the MAXCYCLES variable to
increase the maximum number of cycles for capturing all of the desired
Verilog emulator activity.


          

