/*
 * jo_mux_index_shuffle.h
 *
 *  Created on: Jul 12, 2019
 *      Author: jcarmona
 */

#ifndef SRC_JO_MUX_INDEX_SHUFFLE_API_H_
#define SRC_JO_MUX_INDEX_SHUFFLE_API_H_


#define JO_MUX_INDEX_SHUFFLE_CREG    ( XPAR_MUX_INDEX_SHUFFLER_BASEADDR + (0 * 0x4) )


typedef struct
{
	unsigned int shuffling_mask;
}
jo_mux_index_shuffle;


extern int jo_mux_index_shuffle_cfg     (jo_mux_index_shuffle *cfg_data);
extern int jo_mux_index_shuffle_default_cfg (void);



#endif /* SRC_JO_MUX_INDEX_SHUFFLE_API_H_ */
