#include "conv_core.h"

void Conv(ap_uint<16> CHin,ap_uint<16> Hin,ap_uint<16> Win,ap_uint<16> CHout,
		ap_uint<8> Kx,ap_uint<8> Ky,ap_uint<8> Sx,ap_uint<8> Sy,ap_uint<1> mode,ap_uint<1> relu_en,
		float feature_in[], float W[],float bias[],float feature_out[]
	)
{
	#pragma HLS INTERFACE s_axilite port=relu_en
	#pragma HLS INTERFACE s_axilite port=mode
	#pragma HLS INTERFACE s_axilite port=Sy
	#pragma HLS INTERFACE s_axilite port=Sx
	#pragma HLS INTERFACE s_axilite port=Ky
	#pragma HLS INTERFACE s_axilite port=Kx
	#pragma HLS INTERFACE s_axilite port=CHout
	#pragma HLS INTERFACE s_axilite port=Win
	#pragma HLS INTERFACE s_axilite port=Hin
	#pragma HLS INTERFACE s_axilite port=CHin
	#pragma HLS INTERFACE m_axi depth=4294967295 port=feature_out offset=slave
	#pragma HLS INTERFACE m_axi depth=4294967295 port=bias offset=slave
	#pragma HLS INTERFACE m_axi depth=4294967295 port=feature_in offset=slave
	#pragma HLS INTERFACE m_axi depth=4294967295 port=W offset=slave
	#pragma HLS INTERFACE s_axilite port=return

	float  sum=0;
	
	ap_uint<8> pad_x,pad_y;
	if(mode==0)
	{
		pad_x=0;pad_y=0;
	}
	else
	{
		pad_x=(Kx-1)/2;
		pad_y=(Ky-1)/2;
	}
	ap_uint<16> Wout=(Win+2*pad_x-Kx)/Sx+1;
	ap_uint<16> Hout=(Hin+2*pad_y-Ky)/Sy+1;

	for(int cout=0;cout<CHout;cout++)
	{
// #pragma HLS LOOP_FLATTEN
#pragma HLS LOOP_TRIPCOUNT min=128 max=128 avg=128
		for(int i=0;i<Hout;i++) 
		{
// #pragma HLS LOOP_FLATTEN
#pragma HLS LOOP_TRIPCOUNT min=64 max=64 avg=64
			for(int j=0;j<Wout;j++) 
			{

#pragma HLS LOOP_TRIPCOUNT min=64 max=64 avg=64
 
				for(int ii=0;ii<Ky;ii++) 
				{
					// #pragma HLS LOOP_FLATTEN
					#pragma HLS LOOP_TRIPCOUNT min=16 max=16 avg=16
					for(int jj=0;jj<Kx;jj++) 
					{

						#pragma HLS LOOP_TRIPCOUNT min=16 max=16 avg=16
							for(int cin=0;cin<CHin;cin++) 
							{
								#pragma HLS PIPELINE II=5
								#pragma HLS LOOP_TRIPCOUNT min=128 max=128 avg=128		

								if((ii==(Ky-1) && jj==(Kx-1) && cin==(CHin-1))==0)
								{

									ap_int<32> h1=i*Sy;
									ap_int<32> h=h1-pad_y+ii;
									ap_int<32> w1=j*Sx;
									ap_int<32> w=w1-pad_x+jj;
									if(h>=0 && w>=0 && h<Hin && w<Win){
										ap_int<32> x1=h*CHin*Win;
										ap_int<32> x2=w*CHin;
										ap_int<32> x=x1+x2+cin;
										ap_int<32> y11=ii*Kx;
										ap_int<32> y12=CHin*CHout;
										ap_int<32> y1=y11*y12;
										ap_int<32> y2=jj*CHin*CHout;
										ap_int<32> y3=cin*CHout;
										ap_int<32> y=y1+y2+y3+cout;
										float tp=feature_in[x]/4*W[y];
										sum+=tp;
									}
									else
										continue;	
								}
								else
								{
									sum=sum/4+bias[cout];
									if(relu_en && sum<0)
										sum=0;
									ap_int<32> z1=i*Wout*CHout;
									ap_int<32> z2=j*CHout;
									ap_int<32> z=z1+z2+cout;
									feature_out[z]=sum;
									sum=0;
								}
							}
					}
				}

			}
		}
	}
}
