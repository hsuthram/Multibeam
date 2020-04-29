/*
 * jo_beamfrmr_04_api.h
 *
 *  Created on: Mar 4, 2020
 *      Author: jcarmona
 */

#ifndef SRC_JO_BEAMFRMR_04_H
#define SRC_JO_BEAMFRMR_04_H

#define BEAMFORM_RESET_CREG                ((XPAR_JO_BEAMFORM_04_0_BASEADDR) + (0x0 * 4))
#define BEAMFORM_KSR_RE_DATA_CREG          ((XPAR_JO_BEAMFORM_04_0_BASEADDR) + (0x1 * 4))
#define BEAMFORM_KSR_IM_DATA_CREG          ((XPAR_JO_BEAMFORM_04_0_BASEADDR) + (0x2 * 4))
#define BEAMFORM_KSR_WR_CMD_CREG           ((XPAR_JO_BEAMFORM_04_0_BASEADDR) + (0x3 * 4))
#define BEAMFORM_KSR_PTR_RST_CREG          ((XPAR_JO_BEAMFORM_04_0_BASEADDR) + (0x4 * 4))
#define BEAMFORM_KSR_BLK_TO_WR_ON_CREG     ((XPAR_JO_BEAMFORM_04_0_BASEADDR) + (0x5 * 4))
#define BEAMFORM_KSR_OUT_ENA_CREG          ((XPAR_JO_BEAMFORM_04_0_BASEADDR) + (0x6 * 4))

#define BEAMFORM_KSR_OUT_ENABLE            (Xil_Out32(BEAMFORM_KSR_OUT_ENA_CREG, 1))
#define BEAMFORM_KSR_OUT_DISABLE           (Xil_Out32(BEAMFORM_KSR_OUT_ENA_CREG, 0))


extern int jo_beamform_04_cfg ( int bf_mat_ID);

#endif /* SRC_JO_BEAMFRMR_04_H */
