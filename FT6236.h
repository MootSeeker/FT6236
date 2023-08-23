/**
 * @file   FT6236.h
 *
 * @brief  	Brief description of the content of FT6236.h
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
 
#ifndef INC_FT6236_H_
#define INC_FT6236_H_

// ********************************************************************************
/**
* @brief  Includes
*
* Section for module-specific include files
* - Normally not necessary in header files
*/


// ********************************************************************************
/**
* @brief  Macros
*
* Section for module-specific definitions (constants) and macros
*/

/*
 * @brief I2C Chip Address
 */
// 7-Bit mode address
#define FT6236_ADDR 0x34           	// I2C address

// 8-Bit mode address - for use with HAL
#define FT6236_WRITE_ADDR		0x70 // Write address
#define FT6236_READ_ADDR		0x71 // Read address

/*
 * @brief Chip ID's
 */
#define FT6236_VENDID 			0x11  	// FocalTech's panel ID
#define FT6206_CHIPID 			0x06  	// FT6206 ID
#define FT6236_CHIPID 			0x36  	// FT6236 ID
#define FT6236U_CHIPID 			0x64 	// FT6236U ID
#define FT6236_G_FT5201ID 		0xA8	// FocalTech's panel ID

/*
 * @brief Register addresses
 */
#define FT6236_REG_VENDID 		0xA8    // FocalTech's panel ID
#define FT6236_REG_CHIPID 		0xA3    // Chip selecting
#define FT6236_REG_FIRMVERS 	0xA6    // Firmware version

#define FT6236_REG_CALIBRATE 	0x02   	// Calibrate mode
#define FT6236_REG_WORKMODE 	0x00    // Work mode
#define FT6236_REG_FACTORYMODE 	0x40 	// Factory mode
#define FT6236_REG_THRESHHOLD 	0x80  	// Threshold for touch detection
#define FT6236_REG_POINTRATE 	0x88   	// Point rate

#define FT6236_REG_NUMTOUCHES 	0x02 	// Number of touch points
#define FT6236_NUM_X 			0x33 	// Touch X position
#define FT6236_NUM_Y 			0x34 	// Touch Y position
#define FT6236_REG_MODE 		0x00    // Device mode, either WORKING or FACTORY

#define FT6236_MEM_READ			0x00	// Memory start address

/*
 * @brief Default threshold value
 */
#define FT6236_DEFAULT_THRESHOLD 128 // Default threshold for touch detection
 
/*
 * @brief Timeouts
 */
#define FT6236_I2C_TOUT		500	// I2C Read timeout
#define FT6236_RST_TOUT		100 // Timeout to set

// ********************************************************************************
/**
* @brief  Enumerations
*
* Section for module-specific enumerations
*/

// ................................................................................
/*
 * @brief Return value for FT6236 functions
 */
typedef enum
{
	FT_SUCCESS = 0,
	FT_ERROR = 1,
}enFT6236_ERROS;

// ................................................................................
/*
 * @brief Gesture values
 * @note  Not used since function is not available,
 * 		  see: https://github.com/focaltech-systems/drivers-input-touchscreen-FTS_driver/issues/1
 */
typedef enum
{
	GST_MOVE_UP 	= 0x10,	 // Move Up gesture
	GST_MOVE_RIGHT 	= 0x14,	 // Move Right gesture
	GST_MOVE_DOWN 	= 0x18,	 // Move Down gesture
	GST_MOVE_LEFT 	= 0x1C,	 // Move Left gesture
	GST_ZOOM_IN 	= 0x48,  // Zoom In gesture
	GST_ZOOM_OUT 	= 0x49,  // Zoom Out gesture
	GST_NO_GST 		= 0x00,  // No gesture detected
}enTOUCH_GESTURE;
 
// ********************************************************************************
/**
* @brief  Structures
*
* Section for module-specific structures
*/

// ................................................................................
/*
 * @brief Structure of touch screen values
 */
typedef struct
{
	uint16_t x1;			// X value of first touch point
	uint16_t y1;			// Y value of first touch point
	uint16_t x2;			// X value of second touch point
	uint16_t y2;			// Y value of second touch point
	uint8_t  touch_point;	// amount of touch points (max. 2)
	uint8_t  Key_Sta;
	uint8_t  ok;			// Interrupt buffer value
	uint8_t  gesture;		// Gesture - not supported
} st_ft6236;
 
// ********************************************************************************
/**
* @brief  Global variables
*
* Section for module-specific global variables
*/

// ................................................................................
/*
 * @brief Global definition of st_ft6236 struct
 */
extern st_ft6236 gst_ft6236;
 
// ********************************************************************************
/**
* @brief  Global function prototypes
*
* Section for module-specific global function prototypes
*/

// .................................................................................
/*
 * @brief Init FT6236 touch controller
 * @note  Init Chip, set threshold and read information registers
 *
 * @param 	-
 * @return	enFT6236_ERROS
 */
enFT6236_ERROS ft6236_init( void );

// .................................................................................
/*
 * @brief Read FT6236 data register
 * @note  Read data register and analyze touch point
 *
 * @param 	-
 * @return	-
 */
void ft6236_read( void );

 #endif /* END: FT6236.h */
