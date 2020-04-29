


// #include <xparameters.h>
// #include <xil_types.h>
// #include <xgpio.h>
// #include <xstatus.h>
// #include <xuartps.h>
// #include <string.h>
// #include <xil_cache.h>
// #include <xil_printf.h>
// #include <xscugic.h>
// #include <xspips.h>

#include "jo_mux_index_shuffle_api.h"



int jo_mux_index_shuffle_cfg (jo_mux_index_shuffle *cfg_data)
{

	Xil_Out32(JO_MUX_INDEX_SHUFFLE_CREG, cfg_data->shuffling_mask);

	return 0;
}



int jo_mux_index_shuffle_default_cfg (void)
{

	Xil_Out32(JO_MUX_INDEX_SHUFFLE_CREG, 0x76543210);    // this is the default order 7,6,5,4,3,2,1,0
//	Xil_Out32(JO_MUX_INDEX_SHUFFLE_CREG, 0x01234567);    // this is the reverse order 0,1,2,3,4,5,6,7
//	Xil_Out32(JO_MUX_INDEX_SHUFFLE_CREG, 0x77777777);    // this is the reverse order 0,1,2,3,4,5,6,7
	return 0;
}
