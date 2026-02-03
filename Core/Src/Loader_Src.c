/**
 ******************************************************************************
 * @file    Loader_Src.c
 * @author  MCD Application Team
 * @brief   This file defines the operations of the external loader for
 *          IS42S32400F SDRAM memory of STM32F469I-DISCO.
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include "Loader_Src.h"
#include <string.h>
volatile uint32_t bufferr[16];
/* Private variables ---------------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
KeepInCompilation HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{ 
	return HAL_OK;
}

uint32_t HAL_GetTick(void)
{
	return 1;
}
/** @defgroup STM32F469I_Discovery_SDRAM_Private_Functions Private Functions
 * @{
 */


/**
 * @brief  System initialization.
 * @param  None
 * @retval  1      : Operation succeeded
 * @retval  0      : Operation failed
 */
int main(){return Init();}

int Init()
{

	/*  Init structs to Zero*/
	//memset((void*)0x20001628, 0, 0xC0);
	/*  init system*/
	SystemInit();
	HAL_Init();

	/* Configure the system clock  */
	SystemClock_Config();

	/*Initialaize SDRAM*/
	//return 0;

	return 1;
}

/**
 * @brief   Program memory.
 * @param   Address: page address
 * @param   Size   : size of data
 * @param   buffer : pointer to data buffer
 * @retval  1      : Operation succeeded
 * @retval  0      : Operation failed
 */
KeepInCompilation int Write (uint32_t Address, uint32_t Size, uint8_t* buffer)
{

	/*for(; Size != 0U; Size--)
	{
		bufferr[Size&0xf] =  *(__IO uint32_t *)buffer;
		buffer++;
		Address++;
	}*/

	return 1;

}

/**
 * @brief 	 Full erase of the device
 * @param 	 Parallelism : 0
 * @retval  1           : Operation succeeded
 * @retval  0           : Operation failed
 */
KeepInCompilation int MassErase (uint32_t Parallelism ){

	return 1;
}

/**
 * @brief   Sector erase.
 * @param   EraseStartAddress :  erase start address
 * @param   EraseEndAddress   :  erase end address
 * @retval  None
 */
KeepInCompilation int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress)
{
	return 1;
}


/**
 * Description :
 * Calculates checksum value of the memory zone
 * Inputs    :
 *      StartAddress  : Flash start address
 *      Size          : Size (in WORD)
 *      InitVal       : Initial CRC value
 * outputs   :
 *     R0             : Checksum value
 * Note: Optional for all types of device
 */
uint32_t CheckSum(uint32_t StartAddress, uint32_t Size, uint32_t InitVal)
{
	uint8_t missalignementAddress = StartAddress%4;
	uint8_t missalignementSize = Size ;
	int cnt;
	uint32_t Val;

	StartAddress-=StartAddress%4;
	Size += (Size%4==0)?0:4-(Size%4);

	for(cnt=0; cnt<Size ; cnt+=4)
	{
		Val = *(uint32_t*)StartAddress;
		if(missalignementAddress)
		{
			switch (missalignementAddress)
			{
			case 1:
				InitVal += (uint8_t) (Val>>8 & 0xff);
				InitVal += (uint8_t) (Val>>16 & 0xff);
				InitVal += (uint8_t) (Val>>24 & 0xff);
				missalignementAddress-=1;
				break;
			case 2:
				InitVal += (uint8_t) (Val>>16 & 0xff);
				InitVal += (uint8_t) (Val>>24 & 0xff);
				missalignementAddress-=2;
				break;
			case 3:
				InitVal += (uint8_t) (Val>>24 & 0xff);
				missalignementAddress-=3;
				break;
			}
		}
		else if((Size-missalignementSize)%4 && (Size-cnt) <=4)
		{
			switch (Size-missalignementSize)
			{
			case 1:
				InitVal += (uint8_t) Val;
				InitVal += (uint8_t) (Val>>8 & 0xff);
				InitVal += (uint8_t) (Val>>16 & 0xff);
				missalignementSize-=1;
				break;
			case 2:
				InitVal += (uint8_t) Val;
				InitVal += (uint8_t) (Val>>8 & 0xff);
				missalignementSize-=2;
				break;
			case 3:
				InitVal += (uint8_t) Val;
				missalignementSize-=3;
				break;
			}
		}
		else
		{
			InitVal += (uint8_t) Val;
			InitVal += (uint8_t) (Val>>8 & 0xff);
			InitVal += (uint8_t) (Val>>16 & 0xff);
			InitVal += (uint8_t) (Val>>24 & 0xff);
		}
		StartAddress+=4;
	}

	return (InitVal);
}


/**
 * Description :
 * Verify flash memory with RAM buffer and calculates checksum value of
 * the programmed memory
 * Inputs    :
 *      FlashAddr     : Flash address
 *      RAMBufferAddr : RAM buffer address
 *      Size          : Size (in WORD)
 *      InitVal       : Initial CRC value
 * outputs   :
 *     R0             : Operation failed (address of failure)
 *     R1             : Checksum value
 * Note: Optional for all types of device
 */
KeepInCompilation uint64_t Verify (uint32_t MemoryAddr, uint32_t RAMBufferAddr, uint32_t Size, uint32_t missalignement)
{
	for(int i=1;i;++i) __NOP();
	uint64_t checksum=0x123456;
	/*uint32_t VerifiedData = 0, InitVal = 0;
	 * uint64_t checksum;
	Size*=4;


	checksum = CheckSum((uint32_t)MemoryAddr + (missalignement & 0xf), Size - ((missalignement >> 16) & 0xF), InitVal);
	while (Size>VerifiedData)
	{
		if ( *(uint8_t*)MemoryAddr++ != *((uint8_t*)RAMBufferAddr + VerifiedData))
			return ((checksum<<32) + (MemoryAddr + VerifiedData));

		VerifiedData++;
	}*/

	return (checksum<<32);
}

int SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the System Power Supply
  */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    return 1;
  }

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2) != HAL_OK)
  {
    return 1;
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIS;
  RCC_OscInitStruct.MSISState = RCC_MSI_ON;
  RCC_OscInitStruct.MSISSource = RCC_MSI_RC0;
  RCC_OscInitStruct.MSISDiv = RCC_MSI_DIV8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return 1;
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSIS;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    return 1;
  }
  return 0;
}


KeepInCompilation int Read (uint32_t Address, uint32_t Size, uint8_t* buffer){


		return 1;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
