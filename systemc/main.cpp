/*************************************************

SystemC Transaction Level Modeling Tutorial
(c) 2/3/2013 W. Rhett Davis (rhett_davis@ncsu.edu)

**************************************************/

//#include "top.h"
#include <tlm.h>
#include <stdlib.h>
#include "spike.h"
#include "mem.h"
#include "SimpleBusLT.h"
#include "lstmunit.h"

int sc_main (int argc,char  *argv[])
{
  time_t begin_time, end_time;
  time(&begin_time);
  SimpleBusLT<2, 2> bus("bus");
  spike cpu("cpu",argc,argv);
  mem mem0("mem0",8*1024);
  lstmunit lstmunit0("lstmunit0");
  cpu.master(bus.target_socket[0]);
  lstmunit0.master(bus.target_socket[1]);
  bus.initiator_socket[0](mem0.slave);
  bus.initiator_socket[1](lstmunit0.slave);
//  cpu.master(mem0.slave);
  sc_core::sc_start();
  time(&end_time);
  std::cout << "Simulation time: " << sc_core::sc_time_stamp() << std::endl
            << "Wall clock time: " << difftime(end_time,begin_time) 
            << " seconds\n";
  return 0;
}
