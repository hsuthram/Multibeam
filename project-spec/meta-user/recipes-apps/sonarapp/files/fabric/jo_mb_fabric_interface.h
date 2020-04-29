/*
 * jo_mb_fabric_interface.h
 *
 *  Created on: Mar 12, 2020
 *      Author: jcarmona
 */

#ifndef SRC_JO_MB_FABRIC_INTERFACE_H_
#define SRC_JO_MB_FABRIC_INTERFACE_H_





	typedef struct
	{
		u32              samp_period_in_ns;
		u32              usamp_period_in_ns;
		u32              beams_n;                              // depends on the bf_matrix to use
	 }
	  jo_fabif_session_cfg_report;



	typedef struct
	{

		//========== input parameters for the session configuration

		u8               mxr_tbl_id;
		u8               cplx_fir_tbl_id;
		u8               pwm_chrp_tbl_id;
		u8               tvg_tbl_id;
		u8               bf_matrix_id;

		u32              samp_period_clks;


		//========== output parameters after session configuration; corresponding to settings in fabric

		u32              clk_freq_MHz;                         // fixed to 100MHz; used to calculate clk counts for the acq ranges
		u32              clk_period_ns;                        // 10 ns
		u32              samps_per_usamp;                      // multiplexing factor
		u32              xducer_elems_n;                       // number of elements in transducer
		u32              beams_n;                              // depends on the bf_matrix to use

		u32              dec1_k;                               // decimation factor for first CIC  fixed
		u32              dec2_k;                               // decimation factor for second CIC (depends on acq range)

		struct                                                 // decimation range:  this was deliberately stated as an anonymous structure.
		{
			u8           upper;
			u8           lower;
		}                  dec2_k_rng;

		u32              decim_usamps_max;                     // maximum number of usamps, determined by the number of rows to plot in the screen (decimated depth)

		jo_dma_cfg       dma_cfg;

		jo_usamp_ctrl    usamp_ctrl;


	 }
	  jo_fabif_session_cfg;



	typedef struct
	{

		//============= input parameters

		double     SoS;                        // speed of sound
		double     acq_start_d;                // depth to start acq
		double     acq_stop_d;                 // depth to stop acq

		double     period_in_secs;             // time interval between sequential pings
		u32        iterations;

		//============= output parameters


	 	u32        decim_K_total;              // total decimation factor used
	 	u32        acq_start_cnt;              // sampling period at which acquisition starts (not decimated)
	 	u32        acq_dec_net_cnts;           // decimated sample counts acquired for each ping

	 	struct                                 // implicit structure to be passed to Core 2
	 	{
	 	    u32      elems_req;
	 	    u32      dec_usamps_per_elem;
	 	}          dma_usage;

	}
	 jo_fabif_ping_cfg_data;



	extern int jo_mb_session_cfg_setup  ( int  cfg_bndl_id );


#endif /* SRC_JO_MB_FABRIC_INTERFACE_H_ */
