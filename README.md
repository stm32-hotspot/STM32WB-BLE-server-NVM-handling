# STM32WB BLE server NVM handling

## Overview

The STM32WB uses internal NVM storage on CPU2 by default for storing BLE persistent data. However, for some applications, handling of BLE persistent data in CPU2 may not be convenient. In such cases, flash access on STM32WB is performed by CPU2, and the user application on CPU1 does not have full control of flash access in the application. Users also need to be careful that CPU2 NVM data is lost during CPU2 firmware updates. There is an alternative option for storing BLE persistent data - use SRAM2 buffer provided by the user application.

This example demonstrates how to set the BLE server application to store BLE persistent data in an SRAM2 buffer defined by CPU1. EEPROM emulation runs on CPU1 to store BLE persistent data into flash from SRAM2 buffer.

Please refer to [AN5289](https://www.st.com/resource/en/application_note/an5289-building-wireless-applications-with-stm32wb-series-microcontrollers-stmicroelectronics.pdf) for more details about BLE NVM data handling.    

### <b>Keywords</b>

Connectivity, STM32, P-NUCLEO-WB55, BLE


## Board Needed
  * Project was developed and tested with
    * [NUCLEO-WB55RG](https://www.st.com/en/evaluation-tools/nucleo-wb55rg.html)
	
 
## How to use
The P-NUCLEO-WB55 is a complete development platform, based on Arm® Cortex®-M4 and Arm® Cortex®-M0+ cores.
Prior to running this example on CPU1, CPU2 firmware (wireless stack) needs to be loaded. See the process to load the wireless stack [here](https://wiki.st.com/stm32mcu/wiki/Connectivity:STM32WB_BLE_Hardware_Setup). This example can use either stm32wb5x_BLE_Stack_full_fw or stm32wb5x_BLE_Stack_light_fw.

In order to make the program work, you must do the following:

  * Open your toolchain
    * This example was prepared using STM32CubeIDE v1.14.0
  * Rebuild all files and flash the board with the executable file

  * BLE service of contains a single read characteristic, with restricted permisions - authenticated read attribute permission is enabled. Pairing via numeric comparison is needed before client is able to read characteristic value. Pairing creates bond between BLE server and client, which need to be stored in NVM. For more details about BLE security and pairing please check [MOOC - BLE Security with STM32WB](https://www.youtube.com/playlist?list=PLnMKNibPkDnGbiUxaLBqTdM2nNIVAk_MO).

  * ST-LINK (CN15 port of NUCLEO-WB55RG) Virtual COM port is displaying  debug print from the application in any COM port terminal application

As client side can be used Android/IOS mobile phone device with enabled the Bluetooth Low Energy communications and [ST BLE Toolbox](https://wiki.st.com/stm32mcu/wiki/Connectivity:BLE_smartphone_applications#ST_BLE_Toolbox) application.

## Troubleshooting

**Caution** : Issues and the pull-requests are **not supported** to submit problems or suggestions related to the software delivered in this repository. The STM32WB-BLE-Template example is being delivered as-is, and not necessarily supported by ST.

**For any other question** related to the product, the hardware performance or characteristics, the tools, the environment, you can submit it to the **ST Community** on the STM32 MCUs related [page](https://community.st.com/s/topic/0TO0X000000BSqSWAW/stm32-mcus).