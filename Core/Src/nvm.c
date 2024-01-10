/*
 * nvm.c
 *
 *  Created on: Jan 5, 2024
 *      Author: koudelkl
 */
#include "app_common.h"
#include "main.h"
#include "dbg_trace.h"
#include "flash_interface.h"
#include "nvm.h"

#define BLE_NVM_LENGTH                      507
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint32_t BLE_NVM[BLE_NVM_LENGTH];


#define PWR_FLAG_WUF PWR_FLAG_WUF3

//TODO - improve definition
#define HSEM_PROCESS_1 12U /* Number taken randomly to identify the process locking a semaphore in the driver context */
__IO uint32_t FlashSemaphoreTaken = 0;

void EEPROM_Emul_Init(void)
{
  EE_Status ee_status = EE_OK;

  /* Enable and set FLASH Interrupt priority */
  /* FLASH interrupt is used for the purpose of pages clean up under interrupt */
  HAL_NVIC_SetPriority(FLASH_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(FLASH_IRQn);

  /* Clear OPTVERR bit */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
  while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERR) != RESET) ;


  /* When a direct writing is requested - that is to say no page transfer
     is required to achieve the write - the call of EE_WriteVariableXXbits
     function can return the EE_FLASH_USED value meaning that the flash is currently
     used by CPU2 (semaphore 7 locked). When this is the case, the driver
     also activates the interrupt associated to the release of the semaphore
     (flash not used by CPU2 anymore).
     Then, to benefit from this possibility HSEM_IRQn interrupts are configured.
     HAL_HSEM_FreeCallback is called when the semaphore is released. */
  __HAL_RCC_HSEM_CLK_ENABLE();
  HAL_NVIC_SetPriority(HSEM_IRQn, 15, 0);
  HAL_NVIC_EnableIRQ(HSEM_IRQn);

  /*  Wait for the flash semaphore to be free and take it */
  while(HAL_HSEM_Take(CFG_HW_FLASH_SEMID, HSEM_PROCESS_1) != HAL_OK)
  {
    while( HAL_HSEM_IsSemTaken(CFG_HW_FLASH_SEMID) ) ;
  }

  /* Unlock the Flash Program Erase controller for intialization */
  HAL_FLASH_Unlock();

  /* Set EEPROM emulation firmware to erase all potentially incompletely erased
   pages if the system came from an asynchronous reset. Conditional erase is
   safe to use if all Flash operations where completed before the system reset */
  if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) == RESET)
  {

    /* System reset comes from a power-on reset: Forced Erase */
    /* Initialize EEPROM emulation driver (mandatory) */
    ee_status = EE_Init(EE_FORCED_ERASE);
    if(ee_status != EE_OK) {Error_Handler();}
  }
  else
  {
    /* Clear the Standby flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

    /* Check and Clear the Wakeup flag */
    if (__HAL_PWR_GET_FLAG(PWR_FLAG_WUF) != RESET)
    {
      __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF);
    }


    /* System reset comes from a STANDBY wakeup: Conditional Erase*/
    /* Initialize EEPROM emulation driver (mandatory) */
    ee_status = EE_Init(EE_CONDITIONAL_ERASE);
    if(ee_status != EE_OK) {Error_Handler();}
  }

  /* Lock the Flash Program Erase controller and release flash semaphore */
  HAL_FLASH_Lock();
  HAL_HSEM_Release(CFG_HW_FLASH_SEMID, HSEM_PROCESS_1);
}

void NVM_restore_SRAM_buffer(void)
{
	uint32_t i=0;
	EE_Status ee_status;
	for (i=0; i<BLE_NVM_LENGTH; i++){
		// i+1 - EEPROM starting index is 1
		ee_status=EE_ReadVariable32bits(i+1,(uint32_t*) &(BLE_NVM[i]));
		while((ee_status != EE_OK) && (ee_status != EE_NO_DATA))
		  {
			/*
			 * Repeat reading in case of error
			 * TODO: add timeout mechanism to prevent lock
			 */
			ee_status=EE_ReadVariable32bits(i+1,(uint32_t*) &(BLE_NVM[i]));
		    }
	}

}

void NVM_write_flash(uint32_t StartAddress, uint32_t Size, uint32_t SRAM_buffer_start)
{
	EE_Status ee_status;
	uint32_t i=0;
	uint32_t ee_position=(StartAddress-SRAM_buffer_start)/4;

	/*  Wait for the flash semaphore to be free and take it */
	while(HAL_HSEM_Take(CFG_HW_FLASH_SEMID, HSEM_PROCESS_1) != HAL_OK)
	{
		while( HAL_HSEM_IsSemTaken(CFG_HW_FLASH_SEMID) ) ;
	}
	while(HAL_HSEM_Take(CFG_HW_BLE_NVM_SRAM_SEMID, HSEM_PROCESS_1) != HAL_OK)
		{
			while( HAL_HSEM_IsSemTaken(CFG_HW_BLE_NVM_SRAM_SEMID) ) ;
		}
	HAL_FLASH_Unlock();

	for(i=0; i<Size; i++){
		ee_status = EE_WriteVariable32bits(ee_position+i+1, ((uint32_t*)StartAddress)[i]);
	    /* If flash is used by CPU2 we wait for the interrupt associated to the semaphore 7 releasing
	      to be raised before trying to write again */
		while(ee_status != EE_OK)
	    {
			if ((ee_status & EE_STATUSMASK_CLEANUP) == EE_STATUSMASK_CLEANUP) {ee_status|= EE_CleanUp();}
			if ((ee_status & EE_STATUSMASK_ERROR) == EE_STATUSMASK_ERROR) {Error_Handler();}
			ee_status = EE_WriteVariable32bits(ee_position+i+1, ((uint32_t*)StartAddress)[i]);
	    }
	}
	HAL_FLASH_Lock();
	HAL_HSEM_Release(CFG_HW_BLE_NVM_SRAM_SEMID, HSEM_PROCESS_1);
	HAL_HSEM_Release(CFG_HW_FLASH_SEMID, HSEM_PROCESS_1);

}

uint32_t Get_SRAM_buffer_address(void){
	return (uint32_t)BLE_NVM;
}

void PrintNVM(void)
{
  APP_DBG_MSG("@ NVM content:\r\n");
  APP_DBG_MSG("  StartAddress = 0x%08lX\r\n", (uint32_t)BLE_NVM);
  APP_DBG_MSG("  Size = %ld bytes\r\n", (int32_t)(BLE_NVM_LENGTH * 4));
  for (uint16_t n = 0; n < BLE_NVM_LENGTH; n++)
  {
    APP_DBG_MSG("  %03d: 0x%08lX\r\n", n, BLE_NVM[n]);
  }
  APP_DBG_MSG("\r\n");
}
