#include "lstmunit.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <tlm.h>
#include <systemc.h>

#define NINP 16
#define TSTEP 32

using namespace std;


lstmunit::lstmunit( sc_core::sc_module_name module_name)
  : sc_module (module_name)
  
{
 	master(*this);
 	slave.register_b_transport(this, &lstmunit::custom_b_transport);
    SC_THREAD(main);

}


unsigned int atoi(unsigned char c0, unsigned char c1)
{ 
  unsigned int i=0;

  if (c0 >= 48 && c0 <= 57)
    i=c0-48;
  else if (c0 >=65 && c0 <= 70)
    i=c0-55;
  else if (c0 >=97 && c0 <= 102)
    i=c0-87;
  else
    cout << "ERROR: Could not convert " << c0 << " to a hexadecimal value\n";

  if (c1 >= 48 && c1 <= 57)
    i+=(c1-48)*16;
  else if (c1 >=65 && c1 <= 70)
    i+=(c1-55)*16;
  else if (c1 >=97 && c1 <= 102)
    i+=(c1-87)*16;
  else
    cout << "ERROR: Could not convert " << c1 << " to a hexadecimal value\n";

  //cout << "returning " << i << endl;
  return i;
} 

long long lstmunit::tanh_fxp(long long x)
{
  static short tanhLUT[256] = {
        0x0000, 0x0200, 0x0400, 0x05ff, 0x07fd, 0x09fb, 0x0bf7, 0x0df2, 
        0x0feb, 0x11e2, 0x13d7, 0x15c9, 0x17b9, 0x19a6, 0x1b90, 0x1d76, 
        0x1f59, 0x2139, 0x2314, 0x24ec, 0x26bf, 0x288e, 0x2a58, 0x2c1e, 
        0x2ddf, 0x2f9a, 0x3151, 0x3302, 0x34ae, 0x3655, 0x37f6, 0x3991, 
        0x3b27, 0x3cb6, 0x3e40, 0x3fc4, 0x4142, 0x42ba, 0x442c, 0x4598, 
        0x46fd, 0x485d, 0x49b6, 0x4b09, 0x4c56, 0x4d9d, 0x4edd, 0x5018, 
        0x514d, 0x527b, 0x53a3, 0x54c6, 0x55e2, 0x56f9, 0x580a, 0x5914, 
        0x5a1a, 0x5b19, 0x5c13, 0x5d07, 0x5df6, 0x5edf, 0x5fc4, 0x60a2, 
        0x617c, 0x6250, 0x6320, 0x63ea, 0x64b0, 0x6571, 0x662d, 0x66e4, 
        0x6797, 0x6846, 0x68f0, 0x6995, 0x6a37, 0x6ad4, 0x6b6e, 0x6c03, 
        0x6c95, 0x6d22, 0x6dac, 0x6e33, 0x6eb5, 0x6f35, 0x6fb0, 0x7029, 
        0x709e, 0x7110, 0x717f, 0x71eb, 0x7254, 0x72ba, 0x731e, 0x737e, 
        0x73dc, 0x7437, 0x7490, 0x74e6, 0x753a, 0x758b, 0x75da, 0x7627, 
        0x7672, 0x76ba, 0x7701, 0x7745, 0x7788, 0x77c8, 0x7807, 0x7844, 
        0x787f, 0x78b8, 0x78f0, 0x7926, 0x795b, 0x798e, 0x79bf, 0x79ef, 
        0x7a1e, 0x7a4b, 0x7a77, 0x7aa2, 0x7acb, 0x7af4, 0x7b1b, 0x7b41, 
        0x7b65, 0x7b89, 0x7bab, 0x7bcd, 0x7bee, 0x7c0d, 0x7c2c, 0x7c49, 
        0x7c66, 0x7c82, 0x7c9d, 0x7cb8, 0x7cd1, 0x7cea, 0x7d02, 0x7d19, 
        0x7d30, 0x7d46, 0x7d5b, 0x7d70, 0x7d84, 0x7d97, 0x7daa, 0x7dbc, 
        0x7dce, 0x7ddf, 0x7df0, 0x7e00, 0x7e0f, 0x7e1f, 0x7e2d, 0x7e3c, 
        0x7e49, 0x7e57, 0x7e64, 0x7e70, 0x7e7d, 0x7e88, 0x7e94, 0x7e9f, 
        0x7eaa, 0x7eb4, 0x7ebf, 0x7ec8, 0x7ed2, 0x7edb, 0x7ee4, 0x7eed, 
        0x7ef5, 0x7efd, 0x7f05, 0x7f0d, 0x7f14, 0x7f1c, 0x7f23, 0x7f29, 
        0x7f30, 0x7f36, 0x7f3d, 0x7f43, 0x7f48, 0x7f4e, 0x7f54, 0x7f59, 
        0x7f5e, 0x7f63, 0x7f68, 0x7f6c, 0x7f71, 0x7f75, 0x7f7a, 0x7f7e, 
        0x7f82, 0x7f86, 0x7f89, 0x7f8d, 0x7f91, 0x7f94, 0x7f97, 0x7f9b, 
        0x7f9e, 0x7fa1, 0x7fa4, 0x7fa6, 0x7fa9, 0x7fac, 0x7fae, 0x7fb1, 
        0x7fb3, 0x7fb6, 0x7fb8, 0x7fba, 0x7fbc, 0x7fbe, 0x7fc0, 0x7fc2, 
        0x7fc4, 0x7fc6, 0x7fc8, 0x7fca, 0x7fcb, 0x7fcd, 0x7fce, 0x7fd0, 
        0x7fd1, 0x7fd3, 0x7fd4, 0x7fd6, 0x7fd7, 0x7fd8, 0x7fd9, 0x7fdb, 
        0x7fdc, 0x7fdd, 0x7fde, 0x7fdf, 0x7fe0, 0x7fe1, 0x7fe2, 0x7fe3, 
        0x7fe4, 0x7fe5, 0x7fe5, 0x7fe6, 0x7fe7, 0x7fe8, 0x7fe9, 0x7fe9, 
        };
    long sign,x0i,x1i;
    long long x0,x1,y0,y1;
    sign=1;
    if (x<0) {
      sign=-1;
      x=-x;
    }
    x0i=x>>9;
    if (x0i>=255)
      return sign*tanhLUT[255];
    x0 = x0i<<9;
    y0 = tanhLUT[x0i];
    if (x0==x)
      return sign*y0;
    x1i=x0i+1;
    x1 = x1i<<9;
    y1 = tanhLUT[x1i];
    return sign*((y0*(x1-x)+y1*(x-x0))>>9);
}

long long lstmunit::sigmoid_fxp(long long x)
{
  return (tanh_fxp(x>>1)>>1) + 16384;
}

short lstmunit::saturate(long long x)
{
  if (x>32767)
    return 32767;
  if (x<-32768)
    return -32768;
  return x;
}

void lstmunit::lstm_layer(lstmunit::lstmCoef *coef, short *c_t1, short *h_t1, short *c_t2, short *h_t2,short *x)
{
  int j,k;
  long long f[NINP],g[NINP],o[NINP],i[NINP],temp;
  for (j=0; j<NINP; j++) {
    f[j]=0;
    for (k=0; k<NINP; k++)
      f[j]+=(((long long)coef->Wf[j][k])*x[k])>>15;
    for (k=0; k<NINP; k++)
      f[j]+=(((long long)coef->Uf[j][k])*h_t1[k])>>15;
    f[j]+=coef->bf[j];
    f[j]=sigmoid_fxp(f[j]);
  }
  for (j=0; j<NINP; j++) {
    g[j]=0;
    for (k=0; k<NINP; k++)
      g[j]+=(((long long)coef->Wg[j][k])*x[k])>>15;
    for (k=0; k<NINP; k++)
      g[j]+=(((long long)coef->Ug[j][k])*h_t1[k])>>15;
    g[j]+=coef->bg[j];
    g[j]=tanh_fxp(g[j]);
  }
  for (j=0; j<NINP; j++) {
    i[j]=0;
    for (k=0; k<NINP; k++)
      i[j]+=(((long long)coef->Wi[j][k])*x[k])>>15;
    for (k=0; k<NINP; k++)
      i[j]+=(((long long)coef->Ui[j][k])*h_t1[k])>>15;
    i[j]+=coef->bi[j];
    i[j]=sigmoid_fxp(i[j]);
  }
  for (j=0; j<NINP; j++) {
    o[j]=0;
    for (k=0; k<NINP; k++)
      o[j]+=(((long long)coef->Wo[j][k])*x[k])>>15;
    for (k=0; k<NINP; k++)
      o[j]+=(((long long)coef->Uo[j][k])*h_t1[k])>>15;
    o[j]+=coef->bo[j];
    o[j]=sigmoid_fxp(o[j]);
  }
  for (j=0; j<NINP; j++) {
    c_t2[j]=saturate((f[j]*c_t1[j]+i[j]*g[j])>>15);
  }
  for (j=0; j<NINP; j++) {
    h_t2[j]=saturate((o[j]*tanh_fxp(c_t2[j]))>>15);
  }
}

void lstmunit::main(){

	tlm::tlm_generic_payload  gp;  

 


}



void lstmunit::custom_b_transport ( tlm::tlm_generic_payload &gp, sc_core::sc_time &delay )
{
  sc_dt::uint64    address   = gp.get_address();
  tlm::tlm_command command   = gp.get_command();
  unsigned long    length    = gp.get_data_length();
  unsigned long    i;
  unsigned char    *dp       = gp.get_data_ptr();
  //sc_core::sc_time mem_delay(1,sc_core::SC_NS);
  sc_core::sc_time lstm_TSTEP_delay(8481,sc_core::SC_NS);
	int j,k;
  //wait(delay+mem_delay);
  cout << sc_core::sc_time_stamp() << " " << sc_object::name();
  for (int k=0; k<NINP; k++) { c1[0][k]=0; h1[0][k]=0; c2[0][k]=0; h2[0][k]=0; }
    switch (command) {
      case tlm::TLM_WRITE_COMMAND:
      {        
			if(address == 0x00009000){
				m_coef_ptr = *reinterpret_cast<sc_dt::uint64*>(dp) & 0x000000000fffffff;				
				read_coef1();
				
				cout << sc_core::sc_time_stamp() << " " << sc_object::name() << " ABOUT TO BEGIN LAYER 1 "<<endl;
   			    for (int j=0; j<TSTEP; j++){ 
   			     	cout << sc_core::sc_time_stamp() << " " << sc_object::name() << " processing layer 1 TSTEP "<< dec<<j <<endl;
			    	lstm_layer(&m_coef1,c1[j],h1[j],c1[j+1],h1[j+1],inp_coef[j]);

			    	if(j ==15 || j ==31) {
              cout<< "PROCESSED 16 LAYERS, WAITING FOR 8481 cycles"<<endl; 
              wait(lstm_TSTEP_delay);
            }
			    }
				cout << sc_core::sc_time_stamp() << " " << sc_object::name() << " DONE WITH LAYER 1 "<<endl;			    			    
			    write_c1_to_mem();
				write_h1_to_mem();
				gp.set_response_status( tlm::TLM_OK_RESPONSE );
				
			}
			
			if(address == 0x0000c000){
				m_coef_ptr = *reinterpret_cast<sc_dt::uint64*>(dp) & 0x000000000fffffff;
				read_coef2();
				cout << sc_core::sc_time_stamp() << " " << sc_object::name() << " ABOUT TO BEGIN LAYER 2 "<<endl;				
				for (int j=0; j<TSTEP; j++){ 
   			    	cout << sc_core::sc_time_stamp() << " " << sc_object::name() << " processing layer 2 TSTEP "<< dec<<j <<endl;
				    lstm_layer(&m_coef2,c2[j],h2[j],c2[j+1],h2[j+1],h1[j+1]);
				   if(j ==15 || j ==31) {
             cout<< "PROCESSED 16 LAYERS, WAITING FOR 8481 cycles"<<endl;
             wait(lstm_TSTEP_delay);
           }
				}
				cout << sc_core::sc_time_stamp() << " " << sc_object::name() << " DONE WITH LAYER 2 "<<endl; 				
				write_c2_to_mem();
				write_h2_to_mem();
				gp.set_response_status( tlm::TLM_OK_RESPONSE );
			}
			
			if(address == 0x0000c100){
				m_coef_ptr = *reinterpret_cast<sc_dt::uint64*>(dp) & 0x000000000fffffff;
				read_inp();
				gp.set_response_status( tlm::TLM_OK_RESPONSE );
			}
        
         gp.set_response_status( tlm::TLM_OK_RESPONSE );
         break;
  		}
  		case tlm::TLM_READ_COMMAND:
  		{
  			cout << "READ COMMAND NOT SUPPORTED BY LSTM "<<endl;
  			 gp.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );
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
  
  void lstmunit::read_coef1(){
  
  	sc_core::sc_time delay=sc_core::SC_ZERO_TIME; // Transaction delay   
  	tlm::tlm_generic_payload  gp;                 // Payload   
  	sc_dt::uint64 addr;                           // Transaction address   
  	gp.set_command(tlm::TLM_READ_COMMAND);   
  	gp.set_address( m_coef_ptr );   
  	gp.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );   
  	gp.set_data_length( sizeof(lstmCoef));   
  	gp.set_data_ptr(reinterpret_cast<unsigned char *>(&m_coef1));   
  	cout << sc_core::sc_time_stamp() << " " << sc_object::name()        
  		 << " READ coef addr:0x" << hex << m_coef_ptr << endl;   
   master->b_transport(gp, delay); 
   sc_core::sc_time read_coef_delay(528,sc_core::SC_NS);
   wait(read_coef_delay);  
   cout << sc_core::sc_time_stamp() << " " << sc_object::name()        
   	    << " READ Complete Wf[0][0]=0x" << hex << m_coef1.Wf[0][0]        
   	    << " bo[15]=0x" << m_coef1.bo[15] << endl;   
   	    
   return;
  	
  
  
  }
   void lstmunit::read_coef2(){
  
  	sc_core::sc_time delay=sc_core::SC_ZERO_TIME; // Transaction delay   
  	tlm::tlm_generic_payload  gp;                 // Payload   
  	sc_dt::uint64 addr;                           // Transaction address   
  	gp.set_command(tlm::TLM_READ_COMMAND);   
  	gp.set_address( m_coef_ptr );   
  	gp.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );   
  	gp.set_data_length( sizeof(lstmCoef));   
  	gp.set_data_ptr(reinterpret_cast<unsigned char *>(&m_coef2));   
  	cout << sc_core::sc_time_stamp() << " " << sc_object::name()        
  		 << " READ coef addr:0x" << hex << m_coef_ptr << endl;   
    master->b_transport(gp, delay);   
    sc_core::sc_time read_coef_delay(528,sc_core::SC_NS);
   wait(read_coef_delay);
   cout << sc_core::sc_time_stamp() << " " << sc_object::name()        
   	    << " READ Complete Wf[0][0]=0x" << hex << m_coef2.Wf[0][0]        
   	    << " bo[15]=0x" << m_coef2.bo[15] << endl;   

   	    
   return;
  	
  
  
  }
  void lstmunit::read_inp(){
  	short temp [32][16];
  	sc_core::sc_time delay=sc_core::SC_ZERO_TIME; // Transaction delay   
  	tlm::tlm_generic_payload  gp;                 // Payload   
  	sc_dt::uint64 addr;                           // Transaction address   
  	gp.set_command(tlm::TLM_READ_COMMAND);   
  	gp.set_address( m_coef_ptr );   
  	gp.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );   
  	gp.set_data_length( sizeof(temp));   
  	gp.set_data_ptr(reinterpret_cast<unsigned char *>(&inp_coef));   
  	cout << sc_core::sc_time_stamp() << " " << sc_object::name()        
  		 << " READ coef addr:0x" << hex << m_coef_ptr << endl;   
   master->b_transport(gp, delay);   
   sc_core::sc_time read_inp_delay(128,sc_core::SC_NS);
   wait(read_inp_delay);
   cout << sc_core::sc_time_stamp() << " " << sc_object::name()        
   	    << " READ Complete input[0][0]=0x" << hex << inp_coef[0][0]        
   	    << " input[31][15]=0x" << inp_coef[31][15] << endl;   
   	    
   return;
  	
  
  
  }
  
  void lstmunit::write_c1_to_mem(){
  
  	sc_core::sc_time delay=sc_core::SC_ZERO_TIME; // Transaction delay   
  	tlm::tlm_generic_payload  gp;                 // Payload   
  	sc_dt::uint64 addr = 0x0400;                           // Transaction address   
  	gp.set_command(tlm::TLM_WRITE_COMMAND);   
//  	gp.set_address( m_coef_ptr );   
  	gp.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );   
  	gp.set_data_length(2);   
//  	gp.set_data_ptr(c1);  
	
	for(int i =0; i<TSTEP; i++ ){
		for(int j =0; j<NINP; j++){
			gp.set_address( addr );
			gp.set_data_ptr(reinterpret_cast<unsigned char*>(&c1[i+1][j]));
      cout << sc_core::sc_time_stamp() << " " << sc_object::name(); 
			cout << " WRITING c1["<<i+1<<"]["<<j<<"] TO MEMORY"  << endl;
			master->b_transport(gp, delay);
			addr+=2;
			
		}
	
	}
  }
  
  void lstmunit::write_h1_to_mem(){
  
  	sc_core::sc_time delay=sc_core::SC_ZERO_TIME; // Transaction delay   
  	tlm::tlm_generic_payload  gp;                 // Payload   
  	sc_dt::uint64 addr = 0x0000;                           // Transaction address   
  	gp.set_command(tlm::TLM_WRITE_COMMAND);   
//  	gp.set_address( m_coef_ptr );   
  	gp.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );   
  	gp.set_data_length(2);   
//  	gp.set_data_ptr(c1);  
	
	for(int i =0; i<TSTEP; i++ ){
		for(int j =0; j<NINP; j++){
			gp.set_address( addr );
			gp.set_data_ptr(reinterpret_cast<unsigned char*>(&h1[i+1][j]));
			cout << sc_core::sc_time_stamp() << " " << sc_object::name(); 
			cout << " WRITING h1["<<i+1<<"]["<<j<<"] TO MEMORY"  << endl;
			master->b_transport(gp, delay);
			addr+=2;
			
		}
	
	}
  }
  
  void lstmunit::write_c2_to_mem(){
  
  	sc_core::sc_time delay=sc_core::SC_ZERO_TIME; // Transaction delay   
  	tlm::tlm_generic_payload  gp;                 // Payload   
  	sc_dt::uint64 addr = 0x1000;                           // Transaction address   
  	gp.set_command(tlm::TLM_WRITE_COMMAND);   
//  	gp.set_address( m_coef_ptr );   
  	gp.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );   
  	gp.set_data_length(2);   
//  	gp.set_data_ptr(c1);  
	
	for(int i =0; i<TSTEP; i++ ){
		for(int j =0; j<NINP; j++){
			gp.set_address( addr );
			gp.set_data_ptr(reinterpret_cast<unsigned char*>(&c2[i+1][j]));
			cout << sc_core::sc_time_stamp() << " " << sc_object::name(); 
			cout << " WRITING c2["<<dec<<i+1<<"]["<<j<<"] TO MEMORY"  << endl;
			master->b_transport(gp, delay);
			addr+=2;
			
		}
	
	}
  }
  
  void lstmunit::write_h2_to_mem(){
  
  	sc_core::sc_time delay=sc_core::SC_ZERO_TIME; // Transaction delay   
  	tlm::tlm_generic_payload  gp;                 // Payload   
  	sc_dt::uint64 addr = 0x0c00;                           // Transaction address   
  	gp.set_command(tlm::TLM_WRITE_COMMAND);   
//  	gp.set_address( m_coef_ptr );   
  	gp.set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );   
  	gp.set_data_length(2);   
//  	gp.set_data_ptr(c1);  
	
	for(int i =0; i<TSTEP; i++ ){
		for(int j =0; j<NINP; j++){
			gp.set_address( addr );
			gp.set_data_ptr(reinterpret_cast<unsigned char*>(&h2[i+1][j]));
			cout << sc_core::sc_time_stamp() << " " << sc_object::name(); 
			cout << " WRITING h2["<<dec<<i+1<<"]["<<j<<"] TO MEMORY"  << endl;
			master->b_transport(gp, delay);
			addr+=2;
			
		}
	
	}
  }
  
tlm::tlm_sync_enum  lstmunit::nb_transport_bw( tlm::tlm_generic_payload &gp,
                           tlm::tlm_phase &phase, sc_core::sc_time &delay)
{
  tlm::tlm_sync_enum status;
  status = tlm::TLM_ACCEPTED;
  return status;
} // end nb_transport_bw


void lstmunit::invalidate_direct_mem_ptr					
  (sc_dt::uint64 start_range, sc_dt::uint64 end_range)
{  
    return;
}




