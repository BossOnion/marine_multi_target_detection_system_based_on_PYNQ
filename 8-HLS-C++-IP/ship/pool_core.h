#ifndef __POOL_CORE_H__
#define __POOL_CORE_H__

#include <ap_int.h>
#include <iostream>

void Pool(ap_uint<16> CHin,ap_uint<16> Hin,ap_uint<16> Win,
		ap_uint<8> Kx,ap_uint<8> Ky,ap_uint<2> mode,
		float feature_in[],float feature_out[]
	);
#endif
