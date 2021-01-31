
#ifndef __LSTMUNIT_H__ 
#define __LSTMUNIT_H__

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

class lstmunit : public sc_core::sc_module	,virtual public tlm::tlm_bw_transport_if<>	
{
  public:  
	SC_HAS_PROCESS(lstmunit);
  lstmunit( sc_core::sc_module_name module_name);
  //virtual ~lstmunit();    

#define NINP 16
#define TSTEP 32

  tlm::tlm_initiator_socket<64> master;
  tlm_utils::simple_target_socket<lstmunit,64>  slave;
  
typedef struct lstmCoef {
  short Wf[NINP][NINP], Uf[NINP][NINP], bf[NINP];
  short Wg[NINP][NINP], Ug[NINP][NINP], bg[NINP];
  short Wi[NINP][NINP], Ui[NINP][NINP], bi[NINP];
  short Wo[NINP][NINP], Uo[NINP][NINP], bo[NINP];
	} lstmCoef;
	
	lstmCoef m_coef1;  
	lstmCoef m_coef2;
	short inp_coef[TSTEP][NINP];
	sc_dt::uint64 m_coef_ptr; 
	void read_coef1 ( );
	void read_coef2 ( );
	void read_inp();
	void write_c1_to_mem();
	void write_c2_to_mem();
	void write_h1_to_mem();
	void write_h2_to_mem();
	
	
	 short c1[TSTEP+1][NINP],h1[TSTEP+1][NINP];
  short c2[TSTEP+1][NINP],h2[TSTEP+1][NINP];
  
  long long tanh_fxp(long long);
  long long sigmoid_fxp(long long);
  short saturate(long long);
//  void lstm_layer(lstmCoef *coef, short *c_t1, short *h_t1, short *c_t2, short *h_t2, short *x);
   void lstm_layer(lstmCoef*, short*, short* , short* , short* , short* );
   void main (void);
    void custom_b_transport ( tlm::tlm_generic_payload &gp, sc_core::sc_time &delay );
   
  private:
	    
//  sc_dt::uint64 m_memory_size;
//  unsigned char *data;

//  void custom_b_transport
//  ( tlm::tlm_generic_payload &gp, sc_core::sc_time &delay );
  
  
  
  // Not Implemented for this example but required by the initiator socket
  void invalidate_direct_mem_ptr
    (sc_dt::uint64 start_range, sc_dt::uint64 end_range);
  tlm::tlm_sync_enum nb_transport_bw (tlm::tlm_generic_payload  &gp,
    tlm::tlm_phase &phase, sc_core::sc_time &delay);

};


#endif /* __LSTM_UNIT_H__ *///test
