/*
 * nvm.h
 *
 *  Created on: Jan 5, 2024
 *      Author: koudelkl
 */

#ifndef INC_NVM_H_
#define INC_NVM_H_

#ifdef __cplusplus
extern "C" {
#endif

void EEPROM_Emul_Init(void);
void NVM_restore_SRAM_buffer(void);
void NVM_write_flash(uint32_t StartAddress, uint32_t Size, uint32_t SRAM_buffer_start);
uint32_t Get_SRAM_buffer_address(void);
void PrintNVM(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* INC_NVM_H_ */
