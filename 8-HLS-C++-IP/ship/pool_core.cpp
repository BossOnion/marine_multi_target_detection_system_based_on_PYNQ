#include "pool_core.h"

void Pool(ap_uint<16> CHin,ap_uint<16> Hin,ap_uint<16> Win,
		ap_uint<8> Kx,ap_uint<8> Ky,ap_uint<2> mode,
		float feature_in[],float feature_out[]
	)
{
	#pragma HLS INTERFACE m_axi depth=4294967295 port=feature_out offset=slave
	#pragma HLS INTERFACE m_axi depth=4294967295 port=feature_in offset=slave
	#pragma HLS INTERFACE s_axilite port=Win
	#pragma HLS INTERFACE s_axilite port=Kx
	#pragma HLS INTERFACE s_axilite port=Hin
	#pragma HLS INTERFACE s_axilite port=mode
	#pragma HLS INTERFACE s_axilite port=Ky
	#pragma HLS INTERFACE s_axilite port=CHin
	#pragma HLS INTERFACE s_axilite port=return

	ap_uint<16> Hout,Wout;
	Wout=Win/Kx;
	Hout=Hin/Ky;

	float sum0 = -99999999999999999;
	float sum11 = 0;
	float sum12 = 0;
	float sum13 = 0;
	float sum14 = 0;

		for(int c=0;c<CHin;c++)
			{

//		 #pragma HLS LOOP_FLATTEN
		//  #pragma HLS LOOP_TRIPCOUNT min=8 max=8 avg=8
		#pragma HLS LOOP_TRIPCOUNT min=32 max=32 avg=32

				for(int i=0;i<Hout;i++)
				{
//		#pragma HLS LOOP_FLATTEN
		#pragma HLS LOOP_TRIPCOUNT min=32 max=32 avg=32

					for(int j=0;j<Wout;j++)
					{
//#pragma HLS LOOP_FLATTEN
		#pragma HLS LOOP_TRIPCOUNT min=32 max=32 avg=32

						for(int ii=0;ii<Ky;ii++)
						{
		#pragma HLS LOOP_TRIPCOUNT min=2 max=2 avg=2

							for(int jj=0;jj<Kx;jj++)
							{
		#pragma HLS PIPELINE
		#pragma HLS LOOP_TRIPCOUNT min=2 max=2 avg=2
								ap_int<16> h1=i*Ky;
								ap_int<16> w1=j*Kx;
								ap_int<16> h=h1+ii;
								ap_int<16> w=w1+jj;
								ap_int<16> x=h*CHin*Win;
								ap_int<16> y=w*CHin;
								ap_int<16> f=x+y+c;

								if(ii==0 && jj==0){
									if(sum0<feature_in[f])
										sum11=feature_in[f];}
								else if(ii==0 && jj==1){
									if(sum11<feature_in[f])
										sum12=feature_in[f];
									else
										sum12=sum11;}
								else if(ii==1 && jj==0){
									if(sum12<feature_in[f])
										sum13=feature_in[f];
									else
										sum13=sum12;}
								else if(ii==1 && jj==1){
									if(sum13<feature_in[f])
										sum14=feature_in[f];
									else
										sum14=sum13;
										ap_int<16> z1=i*Wout*CHin;
										ap_int<16> z2=j*CHin;
										ap_int<16> z=z1+z2+c;
										feature_out[z]=sum14;}
									
							}
						}
					}
				}
			}


}
