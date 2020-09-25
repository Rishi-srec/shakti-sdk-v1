/***************************************************************************
* Project           	     : shakti devt board
* Name of the file	     : xadc_driver.h
* Brief Description of file  : Header file for xadc driver.
* Name of Author    	     : Sathya Narayanan N
* Email ID                   : sathya281@gmail.com

  Copyright (C) 2020  IIT Madras. All rights reserved.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
***************************************************************************/
/**
 * @file xadc_driver.h
 * @brief  Header file for xadc driver.c
 * @detail This file contains the definitions for xadc driver. The memory map
 * for the xadc registers are also defined here.
 */

#ifndef XADC_DRIVER
#define XADC_DRIVER

#include "platform.h"
#include "traps.h"

/**@name Register offsets
 *
 * The following constants provide access to each of the registers of the
 * System Monitor/ADC device.
 * @{
 */

/*
 * System Monitor/ADC Local Registers
 */
#define XADC_SRR_OFFSET		0x00  /**< Software Reset Register */
#define XADC_SR_OFFSET		0x04  /**< Status Register */
#define XADC_AOR_OFFSET		0x08  /**< Alarm Output Register */
#define XADC_CONVST_OFFSET	0x0C  /**< ADC Convert Start Register */
#define XADC_ARR_OFFSET		0x10  /**< ADC Reset Register */

/*
 * System Monitor/ADC Interrupt Registers
 */
#define XADC_GIER_OFFSET	0x5C  /**< Global Interrupt Enable Register */
#define XADC_ISR_OFFSET	        0x60  /**< Interrupt Status Register */
#define XADC_IER_OFFSET		0x68  /**< Interrupt Enable register */

/*
 * System Monitor/ADC Internal Channel Registers
 */
#define XADC_TEMP_OFFSET	  (0x200)   /**< On-chip Temperature Reg */
#define XADC_VCCINT_OFFSET	  (0x204)   /**< On-chip VCCINT Data Reg */
#define XADC_VCCAUX_OFFSET	  (0x208)   /**< On-chip VCCAUX Data Reg */
#define XADC_VPVN_OFFSET	  (0x20C)   /**< ADC out of VP/VN	   */

/*
 * System Monitor/ADC External Channel Registers
 */
#define XADC_AUX00_OFFSET	(0x240)   /**< ADC out of VAUXP0/VAUXN0 */
#define XADC_AUX01_OFFSET	(0x244)   /**< ADC out of VAUXP1/VAUXN1 */
#define XADC_AUX02_OFFSET	(0x248)   /**< ADC out of VAUXP2/VAUXN2 */
#define XADC_AUX03_OFFSET	(0x24C)   /**< ADC out of VAUXP3/VAUXN3 */
#define XADC_AUX04_OFFSET	(0x250)   /**< ADC out of VAUXP4/VAUXN4 */
#define XADC_AUX05_OFFSET	(0x254)   /**< ADC out of VAUXP5/VAUXN5 */
#define XADC_AUX06_OFFSET	(0x258)   /**< ADC out of VAUXP6/VAUXN6 */
#define XADC_AUX07_OFFSET	(0x25C)   /**< ADC out of VAUXP7/VAUXN7 */
#define XADC_AUX08_OFFSET	(0x260)   /**< ADC out of VAUXP8/VAUXN8 */
#define XADC_AUX09_OFFSET	(0x264)   /**< ADC out of VAUXP9/VAUXN9 */
#define XADC_AUX10_OFFSET	(0x268)   /**< ADC out of VAUXP10/VAUXN10 */
#define XADC_AUX11_OFFSET	(0x26C)   /**< ADC out of VAUXP11/VAUXN11 */
#define XADC_AUX12_OFFSET	(0x270)   /**< ADC out of VAUXP12/VAUXN12 */
#define XADC_AUX13_OFFSET	(0x274)   /**< ADC out of VAUXP13/VAUXN13 */
#define XADC_AUX14_OFFSET	(0x278)   /**< ADC out of VAUXP14/VAUXN14 */
#define XADC_AUX15_OFFSET	(0x27C)   /**< ADC out of VAUXP15/VAUXN15 */

/*
 * System Monitor/ADC Registers for Maximum/Minimum data captured for the
 * on chip Temperature/VCCINT/VCCAUX data.
 */
#define XADC_MAX_TEMP_OFFSET	(0x280)   /**< Maximum Temperature Reg */
#define XADC_MAX_VCCINT_OFFSET	(0x284)   /**< Maximum VCCINT Register */
#define XADC_MAX_VCCAUX_OFFSET	(0x288)   /**< Maximum VCCAUX Register */
#define XADC_MAX_VBRAM_OFFSET	(0x28C)   /**< Maximum VBRAM Reg / */
#define XADC_MIN_TEMP_OFFSET	(0x290)   /**< Minimum Temperature Reg */
#define XADC_MIN_VCCINT_OFFSET	(0x294)   /**< Minimum VCCINT Register */
#define XADC_MIN_VCCAUX_OFFSET	(0x298)   /**< Minimum VCCAUX Register */
#define XADC_MIN_VBRAM_OFFSET	(0x29C)   /**< Maximum VBRAM Reg / */
#define XADC_MAX_VCCPINT_OFFSET	(0x2A0)   /**< Max VCCPINT Register  */
#define XADC_MAX_VCCPAUX_OFFSET	(0x2A4)   /**< Max VCCPAUX Register  */
#define XADC_MAX_VCCPDRO_OFFSET	(0x2A8)   /**< Max VCCPDRO Register  */
#define XADC_MIN_VCCPINT_OFFSET	(0x2AC)   /**< Min VCCPINT Register  */
#define XADC_MIN_VCCPAUX_OFFSET	(0x2B0)   /**< Min VCCPAUX Register  */
#define XADC_MIN_VCCPDRO_OFFSET	(0x2B4)   /**< Min VCCPDRO Register  */
#define XADC_MAX_VUSR0_OFFSET	(0x480)   /**< Maximum VUSER0 Supply Reg */
#define XADC_MAX_VUSR1_OFFSET	(0x484)   /**< Maximum VUSER1 Supply Reg */
#define XADC_MAX_VUSR2_OFFSET	(0x488)   /**< Maximum VUSER2 Supply Reg */
#define XADC_MAX_VUSR3_OFFSET	(0x48C)   /**< Maximum VUSER3 Supply Reg */
#define XADC_MIN_VUSR0_OFFSET	(0x4A0)   /**< Minimum VUSER0 Supply Reg */
#define XADC_MIN_VUSR1_OFFSET	(0x4A4)   /**< Minimum VUSER1 Supply Reg */
#define XADC_MIN_VUSR2_OFFSET	(0x4A8)   /**< Minimum VUSER2 Supply Reg */
#define XADC_MIN_VUSR3_OFFSET	(0x4AC)   /**< Minimum VUSER3 Supply Reg */


#define XADC_FLAG_REG_OFFSET	(0x2FC) /**< General Status */

/*
 * System Monitor/ADC Configuration Registers
 */
#define XADC_CFR0_OFFSET		(0x300)   /**< Configuration Register 0 */

/*
 * System Monitor/ADC Sequence Registers
 */
#define XADC_SEQ00_OFFSET	(0x320)   /**< Seq Reg 00 Adc Channel Selection */
#define XADC_SEQ01_OFFSET	(0x324)   /**< Seq Reg 01 Adc Channel Selection */
#define XADC_SEQ02_OFFSET	(0x328)   /**< Seq Reg 02 Adc Average Enable */
#define XADC_SEQ03_OFFSET	(0x32C)   /**< Seq Reg 03 Adc Average Enable */
#define XADC_SEQ04_OFFSET	(0x330)   /**< Seq Reg 04 Adc Input Mode Select */
#define XADC_SEQ05_OFFSET	(0x334)   /**< Seq Reg 05 Adc Input Mode Select */
#define XADC_SEQ06_OFFSET	(0x338)   /**< Seq Reg 06 Adc Acquisition Select */
#define XADC_SEQ07_OFFSET	(0x33C)   /**< Seq Reg 07 Adc Acquisition Select */
#define XADC_SEQ08_OFFSET	(0x318)   /**< Seq Reg 08 Adc Channel Selection */
#define XADC_SEQ09_OFFSET	(0x31C)   /**< Seq Reg 09 Adc Average Enable */


/*@}*/

/**
 * @name System Monitor/ADC Software Reset Register (SRR) mask(s)
 * @{
 */
#define XADC_SRR_RESET_VALUE	0x0000000A   /**< Device Reset Mask */

/*@}*/

/**
 * @name System Monitor/ADC Status Register (SR) mask(s)
 * @{
 */
#define XADC_SR_JTAG_BUSY_MASK	   0x00000400 /**< JTAG is busy */
#define XADC_SR_JTAG_MODIFIED_MASK 0x00000200 /**< JTAG Write has occurred */
#define XADC_SR_JTAG_LOCKED_MASK   0x00000100 /**< JTAG is locked */
#define XADC_SR_BUSY_MASK	   0x00000080 /**< ADC is busy in conversion */
#define XADC_SR_EOS_MASK	   0x00000040 /**< End of Sequence */
#define XADC_SR_EOC_MASK	   0x00000020 /**< End of Conversion */
#define XADC_SR_CH_MASK		   0x0000001F /**< Input ADC channel */


/**
 * @name System Monitor/ADC CONVST Register (CONVST) mask(s)
 * @{
 */
#define XADC_CONVST_CONVST_MASK		0x00000001   /**< Conversion Start Mask */
#define XADC_CONVST_TEMPUPDT_MASK	0x00000002   /**< Temperature Update   	Enable Mask */
#define XADC_CONVST_WAITCYCLES_SHIFT	2	/**< Wait Cycles Shift */
#define XADC_CONVST_WAITCYCLES_MASK	0x0003FFFC /**< Wait Cycles Mask */
#define XADC_CONVST_WAITCYCLES_DEFAULT	0x03E8	/**< Wait Cycles   	 default value */
/*@}*/

/**
 * @name System Monitor/ADC Reset Register (ARR) mask(s)
 * @{
 */
#define XADC_ARR_RST_MASK	0x00000001 /**< ADC Reset bit mask */

/*@}*/

/**
 * @name Global Interrupt Enable Register (GIER) mask(s)
 * @{
 */
#define XADC_GIER_GIE_MASK	0x80000000 /**< Global interrupt enable */
/*@}*/

/**
 * @name System Monitor/ADC device Interrupt Status/Enable Registers
 *
 * <b> Interrupt Status Register (IPISR) </b>
 *
 * This register holds the interrupt status flags for the device.
 *
 * <b> Interrupt Enable Register (IPIER) </b>
 *
 * This register is used to enable interrupt sources for the device.
 * Writing a '1' to a bit in this register enables the corresponding Interrupt.
 * Writing a '0' to a bit in this register disables the corresponding Interrupt
 *
 * IPISR/IPIER registers have the same bit definitions and are only defined
 * once.
 * @{
 */
#define XADC_IPIXR_VBRAM_MASK	      0x00000400 /**< ALM3 - VBRAM Output Mask */
#define XADC_IPIXR_TEMP_DEACTIVE_MASK  0x00000200 /**< Alarm 0 DEACTIVE */
#define XADC_IPIXR_OT_DEACTIVE_MASK    0x00000100 /**< Over Temp DEACTIVE */
#define XADC_IPIXR_JTAG_MODIFIED_MASK  0x00000080 /**< JTAG Modified */
#define XADC_IPIXR_JTAG_LOCKED_MASK    0x00000040 /**< JTAG Locked */
#define XADC_IPIXR_EOC_MASK	      0x00000020 /**< End Of Conversion */
#define XADC_IPIXR_EOS_MASK	      0x00000010 /**< End Of Sequence */
#define XADC_IPIXR_VCCAUX_MASK	      0x00000008 /**< Alarm 2 - VCCAUX */
#define XADC_IPIXR_VCCINT_MASK	      0x00000004 /**< Alarm 1 - VCCINT */
#define XADC_IPIXR_TEMP_MASK	      0x00000002 /**< Alarm 0 - Temp ACTIVE */
#define XADC_IPIXR_OT_MASK	      0x00000001 /**< Over Temperature ACTIVE */
#define XADC_IPIXR_VUSR0_MASK	      0x00004000 /**< Alarm 8  VUSER0 */
#define XADC_IPIXR_VUSR1_MASK	      0x00008000 /**< Alarm 9  VUSER1 */
#define XADC_IPIXR_VUSR2_MASK	      0x00010000 /**< Alarm 10 VUSER2 */
#define XADC_IPIXR_VUSR3_MASK	      0x00020000 /**< Alarm 11 VUSER3 */
#define XADC_IPIXR_ALL_MASK	      0x0003C7FF /**< Mask of all interrupts */

/************************** Function Prototypes ******************************/

void xadc_write_ctrl_reg(uint32_t *address, uint32_t data);
uint32_t xadc_read_ctrl_reg(uint32_t *address);
uint32_t xadc_read_data(uint32_t *address);
float xadc_onchip_temp(uint32_t value);
float xadc_onchip_voltage(uint32_t value);

#endif
