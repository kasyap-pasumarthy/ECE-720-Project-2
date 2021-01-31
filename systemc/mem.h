/*************************************************

SystemC Transaction Level Modeling Tutorial
(c) 11/25/2019 W. Rhett Davis (rhett_davis@ncsu.edu)

**************************************************/


#ifndef __MEM_H__ 
#define __MEM_H__

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

class mem: public sc_core::sc_module
{
  public:  

  mem( sc_core::sc_module_name module_name,
       sc_dt::uint64  memory_size  // memory size (bytes)
      );

  ~mem();
  #define NINP 16
#define TSTEP 32

typedef struct lstmCoef {
  short Wf[NINP][NINP], Uf[NINP][NINP], bf[NINP];
  short Wg[NINP][NINP], Ug[NINP][NINP], bg[NINP];
  short Wi[NINP][NINP], Ui[NINP][NINP], bi[NINP];
  short Wo[NINP][NINP], Uo[NINP][NINP], bo[NINP];
	} lstmCoef;
  tlm_utils::simple_target_socket<mem,64>  slave;
 
  private:

  bool m_initialized[8];
	    
  sc_dt::uint64 m_memory_size,m_last_addr[8];
  unsigned char *data;

  void custom_b_transport
  ( tlm::tlm_generic_payload &gp, sc_core::sc_time &delay );
  
  void main (void);

};


#endif /* __MEM_H__ */
