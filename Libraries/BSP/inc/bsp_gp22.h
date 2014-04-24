/**
 * \file        bsp_gp22.h
 * \brief       Supports all functions to manage the TDC GP22.
 * \date        2014-04-15
 * \version     0.1
 * \author		Kevin Gerber
 *
 * \addtogroup  bsp
 * @{
 *
 * \addtogroup  bsp_gp22
 * \brief		Supports all functions to manage the TDC GP22. The GP22
 * 				is an time to digital converter with a resolution of 90ps.
 * 				It is used in measurement mode 1, which has an operating
 * 				range from 0 to 2.4us.
 * @{
 */

#ifndef BSP_GP22_H_
#define BSP_GP22_H_

/**
 * \brief	A list of all OP codes from the GP22.
 */
typedef enum {
	WR_REG0 = 0x80,
	WR_REG1 = 0x81,
	WR_REG2 = 0x82,
	WR_REG3 = 0x83,
	WR_REG4 = 0x84,
	WR_REG5 = 0x85,
	WR_REG6 = 0x86,
	/*
	RD_REG0 = 0xB0,
	RD_REG1 = 0xB1,
	RD_REG2 = 0xB2,
	RD_REG3 = 0xB3,
	RD_REG4 = 0xB4,
	RD_REG5 = 0xB5,
	RD_REG6 = 0xB6,
	*/
	RD_RES_0 = 0xB0,
	RD_RES_1 = 0xB1,
	RD_RES_2 = 0xB2,
	RD_RES_3 = 0xB3,
	RD_STAT = 0xB4,
	RD_REG_1 = 0xB5,

	RD_IDBIT = 0xB7,
	RD_PW1ST = 0xB8,

	OP_Init = 0x70,
	OP_Power_On_Reset = 0x50,
	OP_Stast_TOF = 0x01,
	OP_Start_Temp = 0x02,
	OP_Start_Cal_Resonator = 0x03,
	OP_Start_Cal_TDC = 0x04,
	OP_Start_TOF_Restart = 0x05,
	OP_Start_Temp_Restart = 0x06

} gp22_opcode_t;

#define BSP_GP22_REG0		0x00242000
#define BSP_GP22_REG1		0x19490000
#define BSP_GP22_REG2		0xA0000000
#define BSP_GP22_REG3		0x00000000
#define BSP_GP22_REG4		0x20000000
#define BSP_GP22_REG5		0x10000000
#define BSP_GP22_REG6		0x00000000


/*
 * ----------------------------------------------------------------------------
 * Prototypes
 * ----------------------------------------------------------------------------
 */


#endif /* BSP_GP22_H_ */

/**
 * @}
 */

/**
 * @}
 */
