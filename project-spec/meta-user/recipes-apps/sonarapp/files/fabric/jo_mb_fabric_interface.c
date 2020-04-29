/*
 * jo_mb_fabric_config.c
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
#include <math.h>

#include "jo_timing_tools.h"
#include "jo_AFE_interface_api.h"
#include "jo_AFE58JD18_chip_api.h"
#include "jo_decim_r2c_api.h"
#include "jo_dma_rx_api.h"
#include "jo_dig_acq_ctrl_api.h"
#include "jo_tx_exc_arb_pwm_api.h"
#include "jo_undersampling_ctrl_api.h"
#include "jo_beamform_04_api.h"
#include "jo_mux_index_shuffle_api.h"
#include "jo_mb_fabric_interface.h"


static jo_fabif_session_cfg mb_session;


int jo_mb_session_cfg_setup ( int  cfg_bndl_id, jo_fabif_session_cfg_report *sess_rep )
{


    switch (cfg_bndl_id)
    {
        case 1:
				mb_session.samp_period_clks  = 17;                               // gives a sampling freq of
				mb_session.xducer_elems_n    = 128;                              //

				mb_session.beams_n           = 128;                              //
				mb_session.bf_matrix_id      = 1;                                //

				mb_session.dec1_k            = 4;                                //
				mb_session.dec2_k            = 4;                                //
				mb_session.dec2_k_rng.lower  = 4;                                //
				mb_session.dec2_k_rng.upper  = 15;                               //
				mb_session.decim_usamps_max  = 600;                              // TODO Verify that this number makes sense, it is the maximum number of rows after beamforming;

				mb_session.mxr_tbl_id        = 1;                                //
				mb_session.cplx_fir_tbl_id   = 1;                                //
				mb_session.pwm_chrp_tbl_id   = 1;                                //

                break;

        default: // code to be executed if n doesn't match any cases

				mb_session.samp_period_clks  = 17;                               // gives a sampling freq of
				mb_session.xducer_elems_n    = 128;                              //

				mb_session.beams_n           = 128;                              //
				mb_session.bf_matrix_id      = 1;                                //

				mb_session.dec1_k            = 4;                                //
				mb_session.dec2_k            = 4;                                //
				mb_session.dec2_k_rng.lower  = 4;                                //
				mb_session.dec2_k_rng.upper  = 15;                               //

				mb_session.mxr_tbl_id        = 1;                                //
				mb_session.cplx_fir_tbl_id   = 1;                                //
				mb_session.pwm_chrp_tbl_id   = 1;                                //

    }


    //==========  section with parameters that are constant for all configurations

	mb_session.clk_freq_MHz    = 100;                                            // This parameter is constant for all setups. It is used to calculate the acqusition ranges
	mb_session.clk_period_ns   = 10;
	mb_session.samps_per_usamp = 8;                                              // number of multiplexers;  this is the total number of sampling periods per the usamp period


	//------ this dma data is to be reported to CORE 2 for its setup

	mb_session.dma_cfg.elems_total         = DMA_RX_BD_N;                        // configured in DMA setup (core 2)
	mb_session.dma_cfg.bytes_per_word      = DMA_RX_BUFF_WORD_BYTES;
	mb_session.dma_cfg.words_per_elem_max  = DMA_RX_BUFF_ELEM_WORDS;
	mb_session.dma_cfg.words_per_usamp     = mb_session.beams_n;                 // one value per beam in a usamp
    mb_session.dma_cfg.elem_usamp_capacity = floor( ((double)(mb_session.dma_cfg.words_per_elem_max)) /
    		                                        ((double)(mb_session.dma_cfg.words_per_usamp))  	  );

    //------ setup of parameters for the undersampling control

    mb_session.usamp_ctrl.isamps_per_samp  = 1;
    mb_session.usamp_ctrl.samps_per_usamp  = mb_session.samps_per_usamp;
    mb_session.usamp_ctrl.mux_sw_dly_mask  = 16;
    mb_session.usamp_ctrl.last_mux_index   = mb_session.samps_per_usamp-1;


    //==========  section with calls to configure the AFE module via SPI

    jo_AFE_chip_init                      ();
	jo_AFE_chip_regs_configure_all        ();
	jo_AFE_chip_TI_debug_set_all_to_zero  ();                                      // temporary for debug purposes -- remove when we are fine with the AFE registers


    //==========  section with calls to configure the fabric modules

	jo_decim_r2c_mxr_cfg  (   mb_session.mxr_tbl_id      );                        // configure mixer
	jo_decim_r2c_fir_cfg  (   mb_session.cplx_fir_tbl_id );                        // configure fir with CHIRP coefficients
	jo_decim_r2c_cic2_cfg (   mb_session.dec2_k          );                        // configure rate of second decimator
	jo_tx_pwm_arb_cfg     (   mb_session.pwm_chrp_tbl_id );                        // configure CHIRP for Tx PWM
	jo_beamform_04_cfg    (   mb_session.bf_matrix_id    );                        // configure beamforming matrix
	jo_dacq_session_cfg   (   mb_session.samp_period_clks);                        // configure and enable sampling signal, enable ping output
	jo_undersamp_ctrl_cfg ( &(mb_session.usamp_ctrl)     );

	jo_mux_index_shuffle_default_cfg ();                                           // temporary, while we test different ideas twith the mux


    //==========  section with place-holding (dummy) configuration for the dacq module with ping data

	jo_dacq_ping_cfg  dacq_cfg_dummy;

	dacq_cfg_dummy.period_cnts = 100000;
    dacq_cfg_dummy.start_cnts  = 10;
    dacq_cfg_dummy.stop_cnts   = 200;
	dacq_cfg_dummy.iterations  = 1;

    jo_dacq_cfg_ping (&dacq_cfg_dummy);                                          // download ping cfg to module


    sess_rep->samp_period_in_ns  = mb_session.samp_period_clks * mb_session.clk_period_ns;
    sess_rep->usamp_period_in_ns = sess_rep->samp_period_in_ns * mb_session.samps_per_usamp;
    sess_rep->beams_n            = mb_session.beams_n;

	return 0;
}





int jo_mb_fabif_ping_acq_cfg ( jo_fabif_ping_cfg_data *ping )                     // calculates configuration parameters based on input range and SOS, returns actual acq ranges
{
	u32     usamp_period;

	double  acqw_start_t;
	double  acqw_stop_t;
	u32     decim2_K_pre;

	u32     usamps_start_bdec;
	u32     usamps_stop_bdec;
	u32     usamps_net_bdec;
	u32     usamps_net_pre;

	u32     dma_elems_req;
	u32     dma_dec_usamps_elem;
	u32     samps_net_dma_elem;

	u32     P;

	jo_dacq_ping_cfg  dacq_cfg;




	//================= preliminary estimation of ranges

    usamp_period      = mb_session.samp_period_clks * mb_session.samps_per_usamp * mb_session.clk_period_ns;

    acqw_start_t      = ping->acq_start_d / (ping->SoS/2);
    acqw_stop_t       = ping->acq_stop_d  / (ping->SoS/2);

    usamps_start_bdec = floor(acqw_start_t / usamp_period);                  // window of samples_to_start without considering decimation (before decimation or bdec)
    usamps_stop_bdec  = ceil (acqw_stop_t  / usamp_period);                  // window of samples_to_stop without considering decimation (before decimation or bdec)

    usamps_net_bdec   = usamps_stop_bdec - usamps_start_bdec + 1;



    //================= estimation of required decimation

    decim2_K_pre = ceil( ( usamps_net_bdec / ((double)mb_session.decim_usamps_max) )  /  mb_session.dec1_k );         // estimating the factor for the decimation stage 2 so that the total number of usamps does not exceed the max to get

    if      (decim2_K_pre < mb_session.dec2_k_rng.lower )  mb_session.dec2_k = mb_session.dec2_k_rng.lower;
    else if (decim2_K_pre > mb_session.dec2_k_rng.upper )  return (-1);                                                // we cannot allow a decimation larger than the upper ... decim_K  = mb_session.dec2_rate_upper;
    else                                                   mb_session.dec2_k = decim2_K_pre;

    ping->decim_K_total =  mb_session.dec1_k * mb_session.dec2_k;



    //================= adjusting number of samples to dma elements buffer size

    P  =  mb_session.samps_per_usamp * (ping->decim_K_total);                                                                 // factor to make sure the intervals calculated are multiple of both the decimation factor and the multiplexing factor

    usamps_net_pre      = ceil( usamps_net_bdec / (double)P ) * P;                                                            // making sure the range samps_pet_pre is a multiple of samps_per_usamp

    dma_elems_req       = ceil( (usamps_net_pre / (double)(ping->decim_K_total)) / mb_session.dma_cfg.elem_usamp_capacity );  // making sure the decimated usamps are a multiple of the number of usamps that fit in a dma element

    dma_dec_usamps_elem = ceil( (usamps_net_pre / (ping->decim_K_total)) / dma_elems_req );

    samps_net_dma_elem  = dma_dec_usamps_elem * P;




    //================= preparing the structure to program the acquisition

    dacq_cfg.start_cnts    = usamps_start_bdec * mb_session.samps_per_usamp;
    dacq_cfg.stop_cnts     = dacq_cfg.start_cnts + (samps_net_dma_elem * dma_elems_req);

    dacq_cfg.period_cnts   = ceil( ping->period_in_secs * mb_session.clk_period_ns * 1E9 );                            // TODO  check this value 1e9
    dacq_cfg.iterations    = ping->iterations;

    ping->acq_start_cnt    = dacq_cfg.start_cnts;                                                                      // return adjusted values to control head
    ping->acq_dec_net_cnts = (samps_net_dma_elem * dma_elems_req);                                                     // so that it can display ranges correctly


    //================= programming the fabric with the ping cfg data

    jo_dacq_cfg_ping (&dacq_cfg);                                                                                      // program the acquisition in the fabric modules

    Xil_Out32(AFE_ITF_AXI_STREAM_PKG_SIZE_CREG, samps_net_dma_elem); //SET_AFE_PKG_SIZE ( samps_net_dma_elem );                                                                         // set the data package size in the AFE interface module
                                                                                                                       // Xil_Out32(AFE_ITF_AXI_STREAM_PKG_SIZE_CREG, samps_net_dma_elem );
	return 0;
}



int jo_mb_fabif_ping_run (void)                                                                                      // launches a ping session
{
	DIG_CTRL_STOP_PINGS;           															       					 // Make sure an ACQ_RUN_STOP is issued to the dig_acq_ctrl_module before configuring to ensure that it is in idle mode, ready to receive configuration
		jo_wait(100);
	DIG_CTRL_RUN_PINGS;

	return 0;
}
