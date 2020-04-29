/*
 * configure_AFE58JD18.h
 *
 *  Created on: Nov 8, 2017
 *      Author: jcarmona
 */

#ifndef SRC_JO_AFE58JD18_CHIP_API_H_
#define SRC_JO_AFE58JD18_CHIP_API_H_


#define AFE_CHIP_HW_S_NOT_CONFIGURED 0
#define AFE_CHIP_HW_S_CONFIGURED     1

#define AFE_SPI_SLAVE_NUM            0
#define AFE_SPI_TX_BUF_WIDTH         3
#define AFE_SPI_RX_BUF_WIDTH         3

#define AFE_SPIWRITE_DO_ENABLE       1
#define AFE_SPIWRITE_ALREADY_ENABLED 0
#define AFE_SPIREAD_DO_ENABLE        1
#define AFE_SPIREAD_ALREADY_ENABLED  0

#define AFE_REG_0_SFTW_RST_MSK       0x0001

#define AFE_REG_0                    0
#define AFE_REG_1                    1
#define AFE_REG_2                    2
#define AFE_REG_3                    3
#define AFE_REG_4                    4
#define AFE_REG_5                    5
#define AFE_REG_195                  195                 //
#define AFE_REG_196                  196                 //
#define AFE_REG_197                  197                 //
#define AFE_REG_203                  203                 //
#define AFE_REG_205                  205                 //

#define AFE_SW_RST_LENGTH_DFLT       100
#define AFE_HW_RST_LENGTH_DFLT       10000

#define AFE_PATTMODE_CFGREG2_MASK    0x0380
#define AFE_PATTMODE_NORMAL_OP       0x0000
#define AFE_PATTMODE_SYNC            0x0080
#define AFE_PATTMODE_ALTERNATE       0x0100
#define AFE_PATTMODE_CUSTOM          0x0180
#define AFE_PATTMODE_ALL_ONES        0x0200
#define AFE_PATTMODE_TOGGLED         0x0280
#define AFE_PATTMODE_ALL_ZEROS       0x0300
#define AFE_PATTMODE_RAMP            0x0380
#define AFE_NO_CUSTOM_PATTERN        0x0

#define AFE_GAIN_PGA_CFGREG195_MASK      0x2000
#define AFE_GAIN_LNA_CFGREG196_MASK      0x6000
#define AFE_GAIN_DIGATTEN_CFGREG203_MASK 0x00F0

#define AFE_SPI_DEVICE_ID            (XPAR_XSPIPS_0_DEVICE_ID)
#define AFE_SPI_CLKFREQ              (XPAR_XSPIPS_0_SPI_CLK_FREQ_HZ)

#define AFE_SPI_SELECT_SLAVE_MAIN    XSpiPs_SetSlaveSelect(&spi_AFE, AFE_SPI_SLAVE_NUM)       // Assert the SPI slave select to the main SPI channel of the AFE Eval board
#define AFE_SPI_SETUP                spi_setup            (&spi_AFE, AFE_SPI_DEVICE_ID)




typedef struct
{
	u16 reg0;                       // controls SPI operation; software_reset; no-add_offset
	u16 reg1;                       // LVDS interface enabling and rate setting, global power down. Also it has two bits that must be 1 and must be programmed
	u16 reg2;                       // sets pattern modes; low latencies and dig features bypassing
	u16 reg3;                       // LVDS output number of bits,  Dig Gain and offset enabling; demodulator enabling and resolution, digital offset enabling;
	u16 reg4;                       // MSB/LSB first bit setting; ADC resolution; pattern_channel_association
	u16 reg5;                       // custom pattern

    u16 reg195;                     // PGA settings (gain, HighPass enable, clamp ena); and LPF prog
    u16 reg196;                     // LNA settings (gain, HP filter, global/indep channel setting); and ACT termination
    u16 reg197;                     // power down settings. Important for heat consumption and dissipation

    //  reg201            not-needed/used ( used for LNA gain indep setting)

    u16 reg203;                     // Digital TimeGainControl settlings;  LNA_HPF_program
    u16 reg205;                     // to suppress higher harmonics

    //  reg206 to reg217  not-needed/used (different settings for indep control of channels)
}
 jo_AFE58JD18_registers_state;




extern XSpiPs spi_AFE;

//extern int jo_AFEchip_spi_debug(jo_AFE58JD18_chip *afe_chip);

extern int jo_AFE_chip_init                     (void);
extern int jo_AFE_chip_software_reset           (u32 wait_time);
extern int jo_AFE_chip_hardware_reset           (u32 wait_time);
extern int jo_AFE_chip_regs_struct_set_defaults (void);
extern int jo_AFE_chip_regs_configure_all       (void);

extern int jo_AFE_chip_spi_enable_read_out_mode (void);
extern int jo_AFE_chip_spi_enable_write_mode    (void);

extern int jo_AFE_chip_spi_read_from_register   (u8 reg_num, u16 *val_read,    u8 need_to_enable_read_out_mode);
extern int jo_AFE_chip_spi_write_to_register    (u8 reg_num, u16 val_to_write, u8 need_to_enable_write_mode);

extern int jo_AFE_chip_TI_debug_set_all_to_zero (void);  // only for debug purposes -- remove after system has been deemed stable and functioning



#endif /* SRC_JO_AFE58JD18_CHIP_API_H_ */
