
#include <stdio.h>

#define NINP 16
#define TSTEP 32

/*
long long tanh_fxp(long long x)
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

long long sigmoid_fxp(long long x)
{
  return (tanh_fxp(x>>1)>>1) + 16384;
}

short saturate(long long x)
{
  if (x>32767)
    return 32767;
  if (x<-32768)
    return -32768;
  return x;
}



typedef struct lstmCoef {
  short Wf[NINP][NINP], Uf[NINP][NINP], bf[NINP];
  short Wg[NINP][NINP], Ug[NINP][NINP], bg[NINP];
  short Wi[NINP][NINP], Ui[NINP][NINP], bi[NINP];
  short Wo[NINP][NINP], Uo[NINP][NINP], bo[NINP];
} lstmCoef;

void lstm_layer(lstmCoef *coef, 
                short *c_t1, short *h_t1, short *c_t2, short *h_t2,
                short *x)
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

*/

/*
lstmCoef coef1 = {
#include "coef1.inc"
};
lstmCoef coef2 = {
#include "coef2.inc"
};
short input[TSTEP][NINP] = {
#include "input.inc"
};

*/


#include "expected.inc"

int main( int argc, char* argv[] )
{

  // following are just test reads/writes to make sure flow is working correctly
  //  long long *p=(long long*)0x60000110;	//goes to mem
  //  *p=11;

  // printf("Loaded %d\n",*p);	
//  long long *p2 = (long long*)0x60000150;	//goes to mem
//  *p2 = *p;	// reads *p first then writes *p2. one read + one write.
 // long long *p1=(long long*)0x70000180;	//goes to LSTM
  //*p1=8;

  //printf("Loaded %d %d\n",*p,*p1);
//  printf("Loaded %d\n",*p1);



  // THIS IS FOR INPUT
  long long *ppp = (long long*)0x6000b100;
  long long *ppp1 = (long long*)0x7000c100;
  printf("cpu main input 0x%lx \n",*ppp);
  *ppp1 = ppp;
  
  

	// THIS IS FOR COEF1
  long long *p = (long long*)0x60008000;
  long long *p1 = (long long*)0x70009000;
  printf("cpu main coef1 0x%lx \n",*p);
  *p1 = p;
  

  	// THIS IS FOR COEF2
  long long *pp = (long long*)0x6000b000;
  long long *pp1 = (long long*)0x7000c000;
  printf("cpu main coef2 0x%lx \n",*pp);
  *pp1 = pp;
  
  
  // test read from final c1 array
//  short *test = (short *)0x60000420;
//  printf("cpu main test 0x%d \n",*test);

	
  	
  int j,k;

  short c1[TSTEP+1][NINP],h1[TSTEP+1][NINP];
  short c2[TSTEP+1][NINP],h2[TSTEP+1][NINP];
  long error;

/*
  for (k=0; k<NINP; k++) { c1[0][k]=0; h1[0][k]=0; c2[0][k]=0; h2[0][k]=0; }
  for (j=0; j<TSTEP; j++) 
    lstm_layer(&coef1,c1[j],h1[j],c1[j+1],h1[j+1],input[j]);
  for (j=0; j<TSTEP; j++) 
    lstm_layer(&coef2,c2[j],h2[j],c2[j+1],h2[j+1],h1[j+1]);
*/
    
	


// THis is to read the final array values from memory

	short *read_from_mem = (short *)0x60000000;
  short *read_from_mem1 = (short *)0x60000400;
  short *read_from_mem2 = (short *)0x60000c00;
  short *read_from_mem3 = (short *)0x60001000;
	
  
	for (int i =0; i < TSTEP; i++){
		for(int m =0; m< NINP; m++){
			
			//printf("READING h1[%d][%d] FROM MEM\n",i,m);
			h1[i][m]= *read_from_mem;
			read_from_mem +=1;

      //printf("READING c1[%d][%d] FROM MEM\n",i,m);
			c1[i][m]= *read_from_mem1;
			read_from_mem1 +=1;
			//printf("%d \n", read_from_mem1);

     //printf("READING h2[%d][%d] FROM MEM\n",i,m);
			h2[i][m]= *read_from_mem2;
			read_from_mem2 +=1;
			//printf("%d \n", read_from_mem2);

      //printf("READING c2[%d][%d] FROM MEM\n",i,m);
			c2[i][m]= *read_from_mem3;
			read_from_mem3 +=1;
			//printf("%d \n", read_from_mem3);

		}
		
	}  
  
  
  
	//Following is the code to read the final array values in 4 inidividual for loops instead of in one loop. always leave commented.
  /* 
  for (int i =0; i < TSTEP; i++){
		for(int m =0; m< NINP; m++){
			
			//printf("READING h1[%d][%d] FROM MEM\n",i,m);
			h1[i][m]= *read_from_mem;
			read_from_mem +=1;

		}
		
	}

  for (int i =0; i < TSTEP; i++){
		for(int m =0; m< NINP; m++){

      //printf("READING c1[%d][%d] FROM MEM\n",i,m);
			c1[i][m]= *read_from_mem1;
			read_from_mem1 +=1;
			//printf("%d \n", read_from_mem1);

		}
		
	}

  for (int i =0; i < TSTEP; i++){
		for(int m =0; m< NINP; m++){

     //printf("READING h2[%d][%d] FROM MEM\n",i,m);
			h2[i][m]= *read_from_mem2;
			read_from_mem2 +=1;
			//printf("%d \n", read_from_mem2);

		}
		
	}

  for (int i =0; i < TSTEP; i++){
		for(int m =0; m< NINP; m++){

      //printf("READING c2[%d][%d] FROM MEM\n",i,m);
			c2[i][m]= *read_from_mem3;
			read_from_mem3 +=1;
			//printf("%d \n", read_from_mem3);

		}
		
	}
  
  */
	
  // This is to synchronize the final output values with expected values for error checking., always leave commented. 
  /*
	for (int i =0; i < TSTEP; i++){
		for(int m =0; m< NINP; m++){
			
			printf("c1[%d][%d] = %d \n",i,m,c1[i][m] );
			printf("c1_exp[%d][%d] = %d \n",i,m,c1_exp[i][m] );
		}
		
	}  
  */

  // THIS IS ERROR CHECKING WITH THE SYSTEM C MODULES

  
   error=0;
   for (k=0; k<NINP; k++) {
    error+=c1_exp[TSTEP-1][k]-c1[TSTEP-1][k];
    error+=h1_exp[TSTEP-1][k]-h1[TSTEP-1][k];
    error+=c2_exp[TSTEP-1][k]-c2[TSTEP-1][k];
    error+=h2_exp[TSTEP-1][k]-h2[TSTEP-1][k];
  }
  printf(" final error: %d\n",error);


  

  //THIS IS ERROR CHECKING WITHOUT THE SYSTEMC MODULES
  /*
  error=0;
  for (k=0; k<NINP; k++) {
    error+=c1_exp[TSTEP-1][k]-c1[TSTEP][k];
    error+=h1_exp[TSTEP-1][k]-h1[TSTEP][k];
    error+=c2_exp[TSTEP-1][k]-c2[TSTEP][k];
    error+=h2_exp[TSTEP-1][k]-h2[TSTEP][k];
  }
  printf("error: %d\n",error);
  */

  /*
  long error_c1, error_c2, error_h1, error_h2;
  error_c1 =0;
  error_c2 =0;
  error_h1 =0;
  error_h2 =0;

  for (k=0; k<NINP; k++) {
    error_c1+=c1_exp[TSTEP-1][k]-c1[TSTEP-1][k];
    error_h1+=h1_exp[TSTEP-1][k]-h1[TSTEP-1][k];
    error_c2+=c2_exp[TSTEP-1][k]-c2[TSTEP-1][k];
    error_h2+=h2_exp[TSTEP-1][k]-h2[TSTEP-1][k];

  }
  printf("error in c1: %d\n",error_c1);
  printf("error in h1: %d\n",error_h1);
  printf("error in c2: %d\n",error_c2);
  printf("error in h2: %d\n",error_h2);
  
  */
  
 
  

  /* Uncomment this section for a detailed error check
  printf("LSTM layer 1\n");
  for (j=0; j<TSTEP; j++) {
    printf("j: %d  c: ",j);
    for (k=0; k<NINP; k++)
      printf("%d, ",c1_exp[j][k]-(long)c1[j+1][k]);
    printf("\nh: ");
    for (k=0; k<NINP; k++)
      printf("%d, ",h1_exp[j][k]-(long)h1[j+1][k]);
    printf("\n");
  }
  printf("LSTM layer 2\n");
  for (j=0; j<TSTEP; j++) {
    printf("j: %d  c: ",j);
    for (k=0; k<NINP; k++)
      printf("%d, ",c2_exp[j][k]-(long)c2[j+1][k]);
    printf("\nh: ");
    for (k=0; k<NINP; k++)
      printf("%d, ",h2_exp[j][k]-(long)h2[j+1][k]);
    printf("\n");
  }
  */

  return 0;
}
