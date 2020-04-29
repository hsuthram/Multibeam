/*
 * jo_global_regs.h
 *
 *  Created on: Sep 16, 2019
 *      Author: jcarmona
 */

#ifndef SRC_JO_GLOBAL_REGS_H_
#define SRC_JO_GLOBAL_REGS_H_


#define GLOBAL_REGS_PATH_SELECT_CREG       ((XPAR_GLOBAL_REGISTERS_BASEADDR) + (0x0 * 4))



#define GLOBA_REGS_SELECT_DSP_PATH               0
#define GLOBA_REGS_SELECT_RAW_DATA_PATH          1


//  Structures


 typedef struct
 {
//  int  sreset;
    int  path_select;
 }
 jo_global_regs_cfg;


 typedef struct
 {
    int  curr_status;
 }
 jo_global_regs_monit;



  // Global Variables prototypes



  // Function Prototypes

extern int jo_global_regs_select_path_raw_data ( void);
extern int jo_global_regs_select_path_dsp      ( void);


#endif /* SRC_JO_GLOBAL_REGS_H_ */
