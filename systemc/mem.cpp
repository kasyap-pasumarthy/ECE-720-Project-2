/*************************************************

SystemC Transaction Level Modeling Tutorial
(c) 11/25/2019 W. Rhett Davis (rhett_davis@ncsu.edu)

**************************************************/

#include "mem.h"
#include <string>
#include <iostream>
#include <iomanip>
mem::lstmCoef coef1 = {
#include "coef1.inc"
};
mem::lstmCoef coef2 = {
#include "coef2.inc"
};

short input[TSTEP][NINP] = {
#include "input.inc"
};
using namespace  std;


#define CL  16
#define CCD 4
#define RCD 16
#define RP  16
#define CLK_PERIOD 1
#define DATA_BITS  32

SC_HAS_PROCESS(mem);
mem::mem( sc_core::sc_module_name module_name, sc_dt::uint64 memory_size )
  : sc_module (module_name)
  , m_memory_size (memory_size)
{
  
  slave.register_b_transport(this, &mem::custom_b_transport);
  data=new unsigned char[m_memory_size];
  for ( int i=0 ; i<8 ; i++ )
    m_initialized[i]=false;

}

mem::~mem()
{
  delete data;
}

void                                        
mem::custom_b_transport
 ( tlm::tlm_generic_payload &gp, sc_core::sc_time &delay )
{
  sc_dt::uint64    address   = gp.get_address();
  tlm::tlm_command command   = gp.get_command();
  unsigned long    length    = gp.get_data_length();
  unsigned long    i;
  unsigned char    *dp       = gp.get_data_ptr();
  sc_core::sc_time mem_delay(1,sc_core::SC_NS);

  unsigned long bank, num_reads, bytes_per_read, cycles;
	
  bank=(unsigned long)((address & 0x0000000000007000)>>12);
  
  
  
    switch (command) {
      case tlm::TLM_WRITE_COMMAND:
      {
        // Assume WRITES are buffered and written later.
        // We need only consider the time for 8-byte transfers over the
        // 64-bit bus
        cycles=(length/8 + length%8);
        mem_delay=sc_core::sc_time(cycles*CLK_PERIOD,sc_core::SC_NS);
        wait(delay+mem_delay);
	if (!m_initialized[bank])
	  m_initialized[bank]=true;
        m_last_addr[bank]=address;
        cout << sc_core::sc_time_stamp() << " " << sc_object::name();
        cout << " WRITE len:0x" << hex << length << " addr:0x" << address; 
        if (dp) {
          cout << " data:0x";
          for (i=length;i>0;i--) {
            cout << hex << setfill('0') << setw(2) << (unsigned int)dp[i-1];
            data[address+i-1]=dp[i-1];
	  }
          cout << endl;
	}
        else
          cout << endl;

        gp.set_response_status( tlm::TLM_OK_RESPONSE );
        break;
      }
      case tlm::TLM_READ_COMMAND:
      {
        if(address == 0x00008000){
          cout << sc_core::sc_time_stamp() << " " << sc_object::name();
        	cout<<" Reading coef1 "<<endl;
        	unsigned char *src = reinterpret_cast<unsigned char*>(&coef1);
        	for(i=0; i<length; i++){
				dp[i] = src[i];
			}
        }
        
        
        else if(address == 0x0000b000){
          cout << sc_core::sc_time_stamp() << " " << sc_object::name();
        	cout<<" Reading coef2 "<<endl;
        	unsigned char *src = reinterpret_cast<unsigned char*>(&coef2);
        	for(i=0; i<length; i++){
				dp[i] = src[i];
			}
		}
			
		else if(address == 0x0000b100){
      cout << sc_core::sc_time_stamp() << " " << sc_object::name();
        	cout<<" Reading input "<<endl;
        	unsigned char *src = reinterpret_cast<unsigned char*>(&input);
        	for(i=0; i<length; i++){
				dp[i] = src[i];
			}
        }
        
        
        else {
        bytes_per_read=(2*CCD*DATA_BITS/8);
        num_reads=(length/bytes_per_read + length%bytes_per_read);
	if (!m_initialized[bank]) {
          // Open bank for the first time
	  cycles=CCD*num_reads+CL+RCD;
	  m_initialized[bank]=true;
	}
        else if ((address & 0xFFFFFFFFFFFF8000) == 
                    (m_last_addr[bank] & 0xFFFFFFFFFFFF8000))
	  // Same Row
	  cycles=CCD*num_reads+CL;
	else
	  // New Row
	  cycles=CCD*num_reads+CL+RCD+RP;
        m_last_addr[bank]=address;
        mem_delay=sc_core::sc_time(cycles*CLK_PERIOD,sc_core::SC_NS);
        wait(delay+mem_delay);
        
        cout << sc_core::sc_time_stamp() << " " << sc_object::name();
        cout << " READ len:0x" << hex << length << " addr:0x" << address; 
        if (dp) {
          cout << " data:0x";
          for (i=length;i>0;i--) {
            cout << hex << setfill('0') << setw(2) << (unsigned int)data[address+i-1];
            dp[i-1]=data[address+i-1];
	  }
          cout << endl;
	}
        else
          cout << endl;
        
        }
        
        
        


        gp.set_response_status( tlm::TLM_OK_RESPONSE );
        break;
      }
      default:
      {
        cout << " ERROR Command " << command << " not recognized" << endl;
        gp.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );
      } 
    }
  
  

  return;     
}

void mem::main(){

  cout<<"mem.main() reached"<<endl;



}







