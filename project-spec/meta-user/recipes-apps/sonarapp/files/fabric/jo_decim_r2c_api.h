/*
 * jo_decim_r2c_cfg.h
 *
 *  Created on: Mar 12, 2020
 *      Author: jcarmona
 */

#ifndef SRC_JO_DECIM_R2C_API_H_
#define SRC_JO_DECIM_R2C_API_H_


#define DECIMATOR_CIC_DECIMATION_FACTOR              10

#define DECIMATOR_CIC_RESET_AXI_CREG                    ((XPAR_DECIM_BASEADDR) + (0x0  * 4))

#define DECIMATOR_CIC_MXR_PHTABLE_DEPTH_CREG            ((XPAR_DECIM_BASEADDR) + (0x1  * 4))       // ctrl_regs_data [ 1][PHASE_BRAM_ADDR_W-1:0];
#define DECIMATOR_CIC_MXR_PHTABLE_DATA_CREG             ((XPAR_DECIM_BASEADDR) + (0x2  * 4))       // ctrl_regs_data [ 3][PHASE_BRAM_DATA_W-1:0];
#define DECIMATOR_CIC_MXR_PHTABLE_WR_CREG               ((XPAR_DECIM_BASEADDR) + (0x3  * 4))       // ctrl_regs_data [ 4][0];
#define DECIMATOR_CIC_MXR_PHTABLE_RST_PTR_CMD_CREG      ((XPAR_DECIM_BASEADDR) + (0x4  * 4))       // ctrl_regs_data [ 5][0];
#define DECIMATOR_CIC_MXR_PHTABLE_RST_PTR_2RD_CMD_CREG  ((XPAR_DECIM_BASEADDR) + (0x4  * 4))       // ctrl_regs_data [ 5][0];
#define DECIMATOR_CIC_MXR_PHTABLE_RST_PTR_2WR_CMD_CREG  ((XPAR_DECIM_BASEADDR) + (0x5  * 4))       // ctrl_regs_data [ 5][0];

#define DECIMATOR_CIC_CIC_RESET_CMD_CREG                ((XPAR_DECIM_BASEADDR) + (0x6  * 4))       // ctrl_regs_data [ 6][CIC_BLK_CFG_W-1:0];
#define DECIMATOR_CIC_CIC_CFG_DATA_CREG                 ((XPAR_DECIM_BASEADDR) + (0x7  * 4))       // ctrl_regs_data [ 6][CIC_BLK_CFG_W-1:0];
#define DECIMATOR_CIC_CIC_CFG_DATA_PUSH_CREG            ((XPAR_DECIM_BASEADDR) + (0x8  * 4))       // ctrl_regs_data [ 7][0];
#define DECIMATOR_CIC_CIC_CFG_DATA_PKG_LEN_CREG         ((XPAR_DECIM_BASEADDR) + (0x9  * 4))

#define DECIMATOR_CIC_FIR_CFG_DATA_CREG                 ((XPAR_DECIM_BASEADDR) + (0x10 * 4))       // ctrl_regs_data [ 8][FIR_BLK_CFG_W-1:0];
#define DECIMATOR_CIC_FIR_CFG_DATA_PUSH_CREG            ((XPAR_DECIM_BASEADDR) + (0x11 * 4))       // ctrl_regs_data [ 9][0];
#define DECIMATOR_CIC_FIR_CFG_DATA_PKG_LEN_CREG         ((XPAR_DECIM_BASEADDR) + (0x12 * 4))

#define DECIMATOR_CIC_FIR_K_DATA_CREG                   ((XPAR_DECIM_BASEADDR) + (0x13 * 4))      // ctrl_regs_data [10][FIR_BLK_K_W-1:0];
#define DECIMATOR_CIC_FIR_K_DATA_PUSH_CREG              ((XPAR_DECIM_BASEADDR) + (0x14 * 4))      // ctrl_regs_data [11][0];
#define DECIMATOR_CIC_FIR_K_DATA_PKG_LEN_CREG           ((XPAR_DECIM_BASEADDR) + (0x15 * 4))

#define DECIMATOR_CIC_CURR_STATUS_MREG                  ((XPAR_DECIM_BASEADDR) + (0x0  * 4))
#define DECIMATOR_CIC_CIC_CFG_DATA_PUSH_MREG            ((XPAR_DECIM_BASEADDR) + (0x1  * 4))
#define DECIMATOR_CIC_FIR_CFG_DATA_PUSH_DONE_MREG       ((XPAR_DECIM_BASEADDR) + (0x2  * 4))
#define DECIMATOR_CIC_FIR_K_DATA_PUSH_DONE_MREG         ((XPAR_DECIM_BASEADDR) + (0x3  * 4))

#define DECIMATOR_CIC_MXR_PHTABLE_MAX_N             100
#define DECIMATOR_CIC_MXR_PH_TABLE_RST_PTR_2RD      1
#define DECIMATOR_CIC_MXR_PH_TABLE_RST_PTR_2WR      0


typedef struct
{
	unsigned int subsamp_factor;
	unsigned int latency;
	unsigned int mixer_vector_N;
}
jo_decim_r2c_cic;                           // DECIDE IF KEEPS


extern int jo_decim_r2c_mxr_cfg           ( u32 mix_table_ID    );
extern int jo_decim_r2c_mxr_tbl_ptr_reset ( u32 ptr_rst_type    );   // DECIDE IF KEEPS
extern int jo_decim_r2c_cic2_cfg          ( u32 decim_factor    );
extern int jo_decim_r2c_fir_cfg           ( u32 cplx_fir_tbl_id );


#endif /* SRC_JO_DECIM_R2C_API_H_ */
