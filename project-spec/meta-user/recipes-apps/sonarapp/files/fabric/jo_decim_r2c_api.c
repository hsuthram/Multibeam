/*
 * jo_decim_r2c_cfg.c
 *
 *  Created on: Mar 12, 2020
 *      Author: jcarmona
 */


// #include <xparameters.h>
// #include <xil_types.h>
// #include <xgpio.h>
// #include <xstatus.h>
// #include <xuartps.h>
// #include <string.h>
// #include <xil_cache.h>
// #include <xil_printf.h>
#include <sleep.h>

#include "jo_timing_tools.h"
#include "jo_decim_r2c_api.h"



int jo_decim_r2c_mxr_cfg  ( u32 mix_table_ID)
{

	 u32   mxr_coeffs_1_tbl_len = 95;
	 u32   mxr_coeffs_1 [95]    = {   127, 29495, 25520, 58244, 34021, 45668, 14706, 32766, 13709, 44958,
								    34079, 59261, 25932, 29125, 64641, 36045, 41043,  8571, 32023, 19353,
    							    50320, 33030, 53109, 21599, 31453, 5507,  39095, 37182, 2175,  30255,
								    23978, 56199, 33517, 47465, 16494, 32758, 11657, 43172, 34599, 61310,
								    27205, 28094, 62338, 35285, 42329, 10616, 32271, 17557, 48277, 33295,
								    55160, 23385, 30677, 3458,  37822, 38469, 4478,  31015, 22692, 54153,
								    33270, 49262, 18281, 32493, 9607,  41898, 35375, 63615, 28478, 26807,
								    60291, 34525, 44127, 12661, 32518, 15504, 46489, 33559, 57211, 24659,
								    29901, 1153,  36805, 39756, 6525,  31519, 20894, 52109, 33278, 51058,
								    20068, 31973, 7556,  40368, 36151 };

	 u32  *mxr_coeffs_tbl;
	 u32  mxr_coeffs_tbl_len;

	 u32 k;
																									   // Make sure the dig_acq_ctrl module is in IDLE MODE before sending cofiguration
     if (mix_table_ID == 1)
     {
    	 mxr_coeffs_tbl     = mxr_coeffs_1;
    	 mxr_coeffs_tbl_len = mxr_coeffs_1_tbl_len;

         Xil_Out32 (DECIMATOR_CIC_MXR_PHTABLE_DEPTH_CREG,  mxr_coeffs_tbl_len);                                 // configuring the table length
         jo_wait   (INTVL_CFG_1);

  		 Xil_Out32 (DECIMATOR_CIC_MXR_PHTABLE_RST_PTR_2WR_CMD_CREG,  0x01);                            // reset mxr table pointer to WR
  		 Xil_Out32 (DECIMATOR_CIC_MXR_PHTABLE_RST_PTR_2WR_CMD_CREG,  0x00);
         jo_wait   (INTVL_CFG_1);

  		 for (k=0; k < mxr_coeffs_tbl_len; k++)
		 {
				Xil_Out32 (DECIMATOR_CIC_MXR_PHTABLE_DATA_CREG,  mxr_coeffs_tbl[k]);                              // push data point

				Xil_Out32 (DECIMATOR_CIC_MXR_PHTABLE_WR_CREG,    0x01);                                 // register data point
				jo_wait   (INTVL_CFG_1);
				Xil_Out32 (DECIMATOR_CIC_MXR_PHTABLE_WR_CREG,    0x00);
				jo_wait   (INTVL_CFG_1);
		 }

         jo_wait   (INTVL_CFG_1);
		 Xil_Out32 (DECIMATOR_CIC_MXR_PHTABLE_RST_PTR_2RD_CMD_CREG,  0x01);                             // reset mxr table pointer to RD
		 Xil_Out32 (DECIMATOR_CIC_MXR_PHTABLE_RST_PTR_2RD_CMD_CREG,  0x00);

		 return 0;
     }
     else
     {
    	 return -1;                                                                                     // in case the table ID is not found here
     }

}



int jo_decim_r2c_mxr_tbl_ptr_reset ( u32 ptr_rst_type )                            //  DECIDE IF KEEPS
{

	if (ptr_rst_type == DECIMATOR_CIC_MXR_PH_TABLE_RST_PTR_2RD)
	{
		Xil_Out32(DECIMATOR_CIC_MXR_PHTABLE_RST_PTR_2RD_CMD_CREG,  0x01);
		Xil_Out32(DECIMATOR_CIC_MXR_PHTABLE_RST_PTR_2RD_CMD_CREG,  0x00);
	    xil_printf("\n\r mixer table_ptr reset 2RD");
	}
	else if (ptr_rst_type == DECIMATOR_CIC_MXR_PH_TABLE_RST_PTR_2WR)
	{
		Xil_Out32(DECIMATOR_CIC_MXR_PHTABLE_RST_PTR_2WR_CMD_CREG,  0x01);
		Xil_Out32(DECIMATOR_CIC_MXR_PHTABLE_RST_PTR_2WR_CMD_CREG,  0x00);
	    xil_printf("\n\r mixer table_ptr reset 2WR");
	}


	return 0;
}



int jo_decim_r2c_cic2_cfg ( u32 decim_factor )
{

	 Xil_Out32(DECIMATOR_CIC_CIC_CFG_DATA_CREG,         decim_factor);
	 Xil_Out32(DECIMATOR_CIC_CIC_CFG_DATA_PKG_LEN_CREG, 1);                // set to 1 because there is only one parameter that we are configuring via the AXIS_master for hte CIOC config, which is the decim_factor param

	 Xil_Out32(DECIMATOR_CIC_CIC_RESET_CMD_CREG,        0x00);             // resetting the CIC
	 Xil_Out32(DECIMATOR_CIC_CIC_RESET_CMD_CREG,        0x01);
	 Xil_Out32(DECIMATOR_CIC_CIC_RESET_CMD_CREG,        0x00);

 	 while (Xil_In32(DECIMATOR_CIC_CIC_CFG_DATA_PUSH_MREG) == 0)
	 {
	 	jo_wait(100);
	 }

	 Xil_Out32(DECIMATOR_CIC_CIC_CFG_DATA_PUSH_CREG,    0x00);             // pushing the data into the AXIS master cic config stream
 	 Xil_Out32(DECIMATOR_CIC_CIC_CFG_DATA_PUSH_CREG,    0x01);
	 Xil_Out32(DECIMATOR_CIC_CIC_CFG_DATA_PUSH_CREG,    0x00);

	return 0;
}



int jo_decim_r2c_fir_cfg   ( u32 cplx_fir_tbl_id)
{
	 return 0;
}
