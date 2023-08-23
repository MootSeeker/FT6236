/**
 * @file   FT6236.c
 *
 * @brief  	Brief description of the content of FT6236.c
 * @author 	kevin, Juventus Techniker Schule
 * @date   	21.08.2023 - first implementation
 * @version 1.0.0
 * 
 * MIT License
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Copyright (c) 2023 Juventus Techniker Schule
 */
 
// ********************************************************************************
/**
 * @brief  Includes
 *
 * Section for module-specific include files
 * If all include files are inserted in main.h, only the file main.h must be included here
 */
#include "main.h"
 
// ********************************************************************************
/**
 * @brief  Constant variables
 *
 * Section for module-specific constant variables (static) that are only valid within the module
 */
 
// ********************************************************************************
/**
 * @brief  Global variables
 *
 * Section for global variables
 */
st_ft6236 gst_ft6236;
 
// ********************************************************************************
/**
 * @brief  Static variables
 *
 * Section for module-specific variables (static) that are only valid within the module
 */
 
// ********************************************************************************
/**
 * @brief  Static function prototypes
 *
 * Section for module-specific function prototypes (static) for functions that are only valid within the
 * module
 */

// ********************************************************************************
/**
 * @brief  Static functions definitions
 *
 * Definition area for the module-specific functions (static) previously defined within "Static function
 * prototypes"
 */


// ********************************************************************************
/**
 * @brief  Global functions definitions
 *
 * Definition area for the global functions previously defined within "Global function prototypes"
 */

// .................................................................................
/*
 * @brief Init FT6236 touch controller
 * @note  Init Chip, set threshold and read information registers
 *
 * @param 	-
 * @return	enFT6236_ERROS
 */
enFT6236_ERROS ft6236_init( void )
{
	// Local variables
    enFT6236_ERROS stat = FT_ERROR;
	uint8_t id, fw_version, prate, thrs;

	// Reset FT6236 ...............................................................
 	HAL_GPIO_WritePin(FT6236_RST_GPIO_Port, FT6236_RST_Pin, GPIO_PIN_RESET);
 	HAL_Delay(FT6236_RST_TOUT);
 	HAL_GPIO_WritePin(FT6236_RST_GPIO_Port, FT6236_RST_Pin, GPIO_PIN_SET);
 	HAL_Delay(10);

 	// Set initial values .........................................................
 	// Set Threshold value
 	stat = HAL_I2C_Mem_Write(&hi2c1, FT6236_WRITE_ADDR, FT6236_REG_THRESHHOLD, 1, (uint8_t *)FT6236_DEFAULT_THRESHOLD, 1, FT6236_I2C_TOUT);
 	// Set Refresh rate value
 	stat = HAL_I2C_Mem_Write(&hi2c1, FT6236_WRITE_ADDR, FT6236_REG_POINTRATE, 1, (uint8_t *)0xFF, 1, FT6236_I2C_TOUT);
 	HAL_Delay(FT6236_RST_TOUT);

 	// Read Chip ID from Register .................................................
    stat = HAL_I2C_Mem_Read(&hi2c1, FT6236_READ_ADDR, FT6236_REG_VENDID, 1, &id, 1, FT6236_I2C_TOUT);

    // If correct id read information
    if( id == FT6236_VENDID )
    {
		#ifdef DEBUG_TOUCH
    	debug_printf("\tTouch Vend ID: %d", id);

    	// Read Chip ID
    	stat = HAL_I2C_Mem_Read(&hi2c1, FT6236_READ_ADDR, FT6236_REG_CHIPID, 1, &id, 1, FT6236_I2C_TOUT);
    	debug_printf("\tTouch Chip ID: %d", id);

        // Read Firmware version
        stat = HAL_I2C_Mem_Read(&hi2c1, FT6236_READ_ADDR, FT6236_REG_FIRMVERS, 1, &fw_version, 1, FT6236_I2C_TOUT);
        debug_printf("\tTouch Firmware Version: %d", fw_version);

        // Point Rate
        stat = HAL_I2C_Mem_Read(&hi2c1, FT6236_READ_ADDR, FT6236_REG_POINTRATE, 1, &prate, 1, FT6236_I2C_TOUT);
        debug_printf("\tTouch Rate Hz: %d", prate);

        // Threshold
        stat = HAL_I2C_Mem_Read(&hi2c1, FT6236_READ_ADDR, FT6236_REG_THRESHHOLD, 1, &thrs, 1, FT6236_I2C_TOUT);
        debug_printf("\tTouch Thresh: %d", thrs);
        #endif
    }

    // Return status value
    return stat;
}


// .................................................................................
/*
 * @brief Read FT6236 data register
 * @note  Read data register and analyze touch point
 *
 * @param 	-
 * @return	-
 */
void ft6236_read( void )
{
	// Local variables
	uint8_t i = 0;
	uint8_t i2cdat[16];

	// Local struct pointer
	st_ft6236 *pst_ft6236 = &gst_ft6236;

	// Read FT6236 data ...........................................................
	HAL_I2C_Mem_Read(&hi2c1, FT6236_READ_ADDR, FT6236_MEM_READ, 1, i2cdat, sizeof(i2cdat), FT6236_I2C_TOUT);

	#ifdef DEBUG_TOUCH
	for(i = 0; i < 16; i++)
	{
		debug_printf_without_cr_lf("%02x |", i2cdat[i]);
	}
	debug_printf(" ");
	#endif

	// Read gesture ...............................................................
	// IMPORTANT - Gesture is not working with standard firmware on FT6236!
	// Special firmware is required on this chip
//	pst_ft6236->gesture = i2cdat[1];
//	#ifdef DEBUG_TOUCH
//	switch( pst_ft6236->gesture )
//	{
//		case GST_MOVE_DOWN:
//			debug_printf( "GST Down" );
//		break;
//		case GST_MOVE_UP:
//			debug_printf( "GST UP" );
//		break;
//		case GST_MOVE_LEFT:
//			debug_printf( "GST Left" );
//		break;
//		case GST_MOVE_RIGHT:
//			debug_printf( "GST Right" );
//		break;
//		case GST_ZOOM_IN:
//			debug_printf( "GST Zoom In" );
//
//		break;
//		case GST_ZOOM_OUT:
//			debug_printf( "GST Zoom Out" );
//		break;
//
//		default:
//			debug_printf( "No GST" );
//		break;
//	}
//	#endif

	// Get amount of touches ..........................................................
	pst_ft6236->touch_point = i2cdat[2] & 0xf;

	// Get X and Y position of touch point ............................................
	switch (pst_ft6236->touch_point)
	{
		case 2:
			pst_ft6236->x2 = (uint16_t)(i2cdat[9] & 0x0F)<<8 | (uint16_t)i2cdat[10];
			pst_ft6236->y2 = (uint16_t)(i2cdat[11] & 0x0F)<<8 | (uint16_t)i2cdat[12];
		 	#ifdef DEBUG_TOUCH
	    	debug_printf("\tTouch Display at -> X2: %d | Y2: %d", pst_ft6236->x2, pst_ft6236->y2 );
			#endif

		case 1:
			pst_ft6236->x1 = (uint16_t)(i2cdat[3] & 0x0F)<<8 | (uint16_t)i2cdat[4];
	    	pst_ft6236->y1 = (uint16_t)(i2cdat[5] & 0x0F)<<8 | (uint16_t)i2cdat[6];
			#ifdef DEBUG_TOUCH
	    	debug_printf("\tTouch Display at -> X1: %d | Y1: %d", pst_ft6236->x1, pst_ft6236->y1 );
			#endif
	    	break;
		default:

			break;
	}

	// Reset Touch pressed .............................................................
	pst_ft6236->ok = 0;
 }

