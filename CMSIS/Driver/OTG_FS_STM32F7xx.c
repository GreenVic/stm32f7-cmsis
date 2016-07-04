/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2015 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *
 * $Date:        24. August 2015
 * $Revision:    V1.1
 *
 * Project:      OTG Full/Low-Speed Common Driver for ST STM32F7xx
 * Configured:   via RTE_Device.h configuration file
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 1.1
 *    STM32CubeMX generated code can also be used to configure the driver.
 *  Version 1.0
 *    Initial release
 */

#include <stdint.h>

#include "stm32f7xx_hal.h"
#if       defined(USE_STM32756G_EVAL)
#include "stm32756g_eval_io.h"
#endif

#include "Driver_USBH.h"
#include "Driver_USBD.h"

#include "OTG_FS_STM32F7xx.h"


extern void USBH_FS_IRQ (uint32_t gintsts);
extern void USBD_FS_IRQ (uint32_t gintsts);

uint8_t otg_fs_role = ARM_USB_ROLE_NONE;


// Local Functions *************************************************************

/**
  \fn          void Enable_GPIO_Clock (const GPIO_TypeDef *port)
  \brief       Enable GPIO clock
*/
#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
static void Enable_GPIO_Clock (const GPIO_TypeDef *GPIOx) {
  if      (GPIOx == GPIOA) { __GPIOA_CLK_ENABLE(); }
  else if (GPIOx == GPIOB) { __GPIOB_CLK_ENABLE(); }
  else if (GPIOx == GPIOC) { __GPIOC_CLK_ENABLE(); }
  else if (GPIOx == GPIOD) { __GPIOD_CLK_ENABLE(); }
  else if (GPIOx == GPIOE) { __GPIOE_CLK_ENABLE(); }
#if defined(GPIOF)
  else if (GPIOx == GPIOF) { __GPIOF_CLK_ENABLE(); }
#endif
#if defined(GPIOG)
  else if (GPIOx == GPIOG) { __GPIOG_CLK_ENABLE(); }
#endif
#if defined(GPIOH)
  else if (GPIOx == GPIOH) { __GPIOH_CLK_ENABLE(); }
#endif
#if defined(GPIOI)
  else if (GPIOx == GPIOI) { __GPIOI_CLK_ENABLE(); }
#endif
#if defined(GPIOJ)
  else if (GPIOx == GPIOJ) { __GPIOJ_CLK_ENABLE(); }
#endif
#if defined(GPIOK)
  else if (GPIOx == GPIOK) { __GPIOK_CLK_ENABLE(); }
#endif
}
#endif


// Common IRQ Routine **********************************************************

/**
  \fn          void OTG_FS_IRQHandler (void)
  \brief       USB Interrupt Routine (IRQ).
*/
void OTG_FS_IRQHandler (void) {
  uint32_t gintsts;

  gintsts = USB_OTG_FS->GINTSTS & USB_OTG_FS->GINTMSK;

#if (defined(MX_USB_OTG_FS_HOST) && defined(MX_USB_OTG_FS_DEVICE))
  switch (otg_fs_role) {
#ifdef MX_USB_OTG_FS_HOST
    case ARM_USB_ROLE_HOST:
      USBH_FS_IRQ (gintsts);
      break;
#endif
#ifdef MX_USB_OTG_FS_DEVICE
    case ARM_USB_ROLE_DEVICE:
      USBD_FS_IRQ (gintsts);
      break;
#endif
    default:
      break;
  }
#else
#ifdef MX_USB_OTG_FS_HOST
  USBH_FS_IRQ (gintsts);
#else
  USBD_FS_IRQ (gintsts);
#endif
#endif
}


// Public Functions ************************************************************

/**
  \fn          void OTG_FS_PinsConfigure (uint8_t pins_mask)
  \brief       Configure single or multiple USB Pin(s).
  \param[in]   Mask of pins to be configured (possible masking values:
               ARM_USB_PIN_DP, ARM_USB_PIN_DM, ARM_USB_PIN_VBUS,
               ARM_USB_PIN_OC, ARM_USB_PIN_ID)
*/
void OTG_FS_PinsConfigure (uint8_t pins_mask) {
#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
  GPIO_InitTypeDef GPIO_InitStruct;

  if ((pins_mask & ARM_USB_PIN_DP) != 0U) {
    Enable_GPIO_Clock             (MX_USB_OTG_FS_DP_GPIOx);
    GPIO_InitStruct.Pin         =  MX_USB_OTG_FS_DP_GPIO_Pin;
    GPIO_InitStruct.Mode        =  MX_USB_OTG_FS_DP_GPIO_Mode;
    GPIO_InitStruct.Pull        =  MX_USB_OTG_FS_DP_GPIO_PuPd;
    GPIO_InitStruct.Speed       =  MX_USB_OTG_FS_DP_GPIO_Speed;
    GPIO_InitStruct.Alternate   =  MX_USB_OTG_FS_DP_GPIO_AF;
    HAL_GPIO_Init                 (MX_USB_OTG_FS_DP_GPIOx, &GPIO_InitStruct);
  }
  if ((pins_mask & ARM_USB_PIN_DM) != 0U) {
    Enable_GPIO_Clock             (MX_USB_OTG_FS_DM_GPIOx);
    GPIO_InitStruct.Pin         =  MX_USB_OTG_FS_DM_GPIO_Pin;
    GPIO_InitStruct.Mode        =  MX_USB_OTG_FS_DM_GPIO_Mode;
    GPIO_InitStruct.Pull        =  MX_USB_OTG_FS_DM_GPIO_PuPd;
    GPIO_InitStruct.Speed       =  MX_USB_OTG_FS_DM_GPIO_Speed;
    GPIO_InitStruct.Alternate   =  MX_USB_OTG_FS_DM_GPIO_AF;
    HAL_GPIO_Init                 (MX_USB_OTG_FS_DM_GPIOx, &GPIO_InitStruct);
  }
#ifdef MX_USB_OTG_FS_ID_Pin
  if ((pins_mask & ARM_USB_PIN_ID) != 0U) {
    Enable_GPIO_Clock             (MX_USB_OTG_FS_ID_GPIOx);
    GPIO_InitStruct.Pin         =  MX_USB_OTG_FS_ID_GPIO_Pin;
    GPIO_InitStruct.Mode        =  MX_USB_OTG_FS_ID_GPIO_Mode;
    GPIO_InitStruct.Pull        =  MX_USB_OTG_FS_ID_GPIO_PuPd;
    GPIO_InitStruct.Speed       =  MX_USB_OTG_FS_ID_GPIO_Speed;
    GPIO_InitStruct.Alternate   =  MX_USB_OTG_FS_ID_GPIO_AF;
    HAL_GPIO_Init                 (MX_USB_OTG_FS_ID_GPIOx, &GPIO_InitStruct);
  }
#endif
#ifdef MX_USB_OTG_FS_VBUS_Pin                   // Device VBUS sensing pin (input)
  if ((pins_mask & ARM_USB_PIN_VBUS) != 0U) {
    if (otg_fs_role == ARM_USB_ROLE_DEVICE) {
#if  (defined(USE_STM32756G_EVAL))              // Host VBUS power driving pin is on IO expander it needs to be turned off for Device VBUS detection to work
      BSP_IO_Init();
      BSP_IO_ConfigPin(IO_PIN_7, IO_MODE_OFF);
#endif
      Enable_GPIO_Clock           (MX_USB_OTG_FS_VBUS_GPIOx);
      GPIO_InitStruct.Pin       =  MX_USB_OTG_FS_VBUS_GPIO_Pin;
      GPIO_InitStruct.Mode      =  MX_USB_OTG_FS_VBUS_GPIO_Mode;
      GPIO_InitStruct.Pull      =  MX_USB_OTG_FS_VBUS_GPIO_PuPd;
      GPIO_InitStruct.Speed     =  0U;
      GPIO_InitStruct.Alternate =  0U;
      HAL_GPIO_Init               (MX_USB_OTG_FS_VBUS_GPIOx, &GPIO_InitStruct);
    }
  }
#endif
  if ((pins_mask & ARM_USB_PIN_VBUS) != 0U) {
    if (otg_fs_role == ARM_USB_ROLE_HOST) {
#if  (defined(USE_STM32756G_EVAL))              // Host VBUS power driving pin is on IO expander
      BSP_IO_Init();
      BSP_IO_ConfigPin(IO_PIN_7, IO_MODE_OUTPUT);

      // Initial Host VBUS Power Off
#if  (USB_OTG_FS_VBUS_Power_Pin_Active == 0)    // VBUS active low
      BSP_IO_WritePin (IO_PIN_7, BSP_IO_PIN_SET);
#else                                           // VBUS active high
      BSP_IO_WritePin (IO_PIN_7, BSP_IO_PIN_RESET);
#endif
#elif (defined(MX_USB_OTG_FS_VBUS_Power_Pin))   // Host VBUS power driving pin is GPIO (output)
      Enable_GPIO_Clock           (MX_USB_OTG_FS_VBUS_Power_GPIOx);

      // Initial Host VBUS Power Off
#if  (USB_OTG_FS_VBUS_Power_Pin_Active == 0)    // VBUS active low
      HAL_GPIO_WritePin (MX_USB_OTG_FS_VBUS_Power_GPIOx, MX_USB_OTG_FS_VBUS_Power_GPIO_Pin, GPIO_PIN_SET);
#else                                           // VBUS active high
      HAL_GPIO_WritePin (MX_USB_OTG_FS_VBUS_Power_GPIOx, MX_USB_OTG_FS_VBUS_Power_GPIO_Pin, GPIO_PIN_RESET );
#endif

      GPIO_InitStruct.Pin       =  MX_USB_OTG_FS_VBUS_Power_GPIO_Pin;
      GPIO_InitStruct.Mode      =  MX_USB_OTG_FS_VBUS_Power_GPIO_Mode;
      GPIO_InitStruct.Pull      =  MX_USB_OTG_FS_VBUS_Power_GPIO_PuPd;
      GPIO_InitStruct.Speed     =  0U;
      GPIO_InitStruct.Alternate =  0U;
      HAL_GPIO_Init               (MX_USB_OTG_FS_VBUS_Power_GPIOx, &GPIO_InitStruct);
#endif
    }
  }
  if ((pins_mask & ARM_USB_PIN_OC) != 0U) {
    if (otg_fs_role == ARM_USB_ROLE_HOST) {
#if  (defined(USE_STM32756G_EVAL))              // Host overcurrent sensing pin is on IO expander
      BSP_IO_Init();
      BSP_IO_ConfigPin(IO_PIN_6, IO_MODE_INPUT);
#elif (defined(MX_USB_OTG_FS_Overrcurrent_Pin)) // Host overcurrent sensing pin is GPIO (input)
      Enable_GPIO_Clock           (MX_USB_OTG_FS_Overcurrent_GPIOx);
      GPIO_InitStruct.Pin       =  MX_USB_OTG_FS_Overcurrent_GPIO_Pin;
      GPIO_InitStruct.Mode      =  MX_USB_OTG_FS_Overcurrent_GPIO_Mode;
      GPIO_InitStruct.Pull      =  MX_USB_OTG_FS_Overcurrent_GPIO_PuPd;
      GPIO_InitStruct.Speed     =  0U;
      GPIO_InitStruct.Alternate =  0U;
      HAL_GPIO_Init               (MX_USB_OTG_FS_Overcurrent_GPIOx, &GPIO_InitStruct);
#endif
    }
  }
#endif

#ifdef RTE_DEVICE_FRAMEWORK_CUBE_MX
#if  (defined(USE_STM32756G_EVAL))              // Host VBUS power driving pin is on IO expander
  if ((pins_mask & ARM_USB_PIN_VBUS) != 0U) {
    if (otg_fs_role == ARM_USB_ROLE_HOST) {
      BSP_IO_Init();
      BSP_IO_ConfigPin(IO_PIN_7, IO_MODE_OUTPUT);

      // Initial Host VBUS Power Off
#if  (USB_OTG_FS_VBUS_Power_Pin_Active == 0)    // VBUS active low
      BSP_IO_WritePin (IO_PIN_7, BSP_IO_PIN_SET);
#else                                           // VBUS active high
      BSP_IO_WritePin (IO_PIN_7, BSP_IO_PIN_RESET);
#endif
    }
  }
  if ((pins_mask & ARM_USB_PIN_OC) != 0U) {
    if (otg_fs_role == ARM_USB_ROLE_HOST) {
      BSP_IO_Init();
      BSP_IO_ConfigPin(IO_PIN_6, IO_MODE_INPUT);
    }
  }
#endif
#endif
}

/**
  \fn          void OTG_FS_PinsUnconfigure (uint8_t pins_mask)
  \brief       De-configure to reset settings single or multiple USB Pin(s).
  \param[in]   Mask of pins to be de-configured (possible masking values:
               ARM_USB_PIN_DP, ARM_USB_PIN_DM, ARM_USB_PIN_VBUS,
               ARM_USB_PIN_OC, ARM_USB_PIN_ID)
*/
void OTG_FS_PinsUnconfigure (uint8_t pins_mask) {

#ifdef RTE_DEVICE_FRAMEWORK_CLASSIC
  if ((pins_mask & ARM_USB_PIN_DP) != 0U) {
    HAL_GPIO_DeInit (MX_USB_OTG_FS_DP_GPIOx, MX_USB_OTG_FS_DP_GPIO_Pin);
  }
  if ((pins_mask & ARM_USB_PIN_DM) != 0U) {
    HAL_GPIO_DeInit (MX_USB_OTG_FS_DM_GPIOx, MX_USB_OTG_FS_DM_GPIO_Pin);
  }
#ifdef MX_USB_OTG_FS_ID_Pin
  if ((pins_mask & ARM_USB_PIN_ID) != 0U) {
    HAL_GPIO_DeInit (MX_USB_OTG_FS_ID_GPIOx, MX_USB_OTG_FS_ID_GPIO_Pin);
  }
#endif
#ifdef MX_USB_OTG_FS_VBUS_Pin
  if ((pins_mask & ARM_USB_PIN_VBUS) != 0U) {
    if (otg_fs_role == ARM_USB_ROLE_DEVICE) {
      HAL_GPIO_DeInit (MX_USB_OTG_FS_VBUS_GPIOx, MX_USB_OTG_FS_VBUS_GPIO_Pin);
    }
  }
#endif
  if ((pins_mask & ARM_USB_PIN_VBUS) != 0U) {
    if (otg_fs_role == ARM_USB_ROLE_HOST) {
#if  (defined(USE_STM32756G_EVAL))
      BSP_IO_ConfigPin(IO_PIN_7, IO_MODE_OFF);
#elif (defined(MX_USB_OTG_FS_VBUS_Power_Pin))
      HAL_GPIO_DeInit (MX_USB_OTG_FS_VBUS_Power_GPIOx, MX_USB_OTG_FS_VBUS_Power_GPIO_Pin);
#endif
    }
  }
  if ((pins_mask & ARM_USB_PIN_OC) != 0U) {
    if (otg_fs_role == ARM_USB_ROLE_HOST) {
#if  (defined(USE_STM32756G_EVAL))
      BSP_IO_ConfigPin(IO_PIN_6, IO_MODE_OFF);
#elif (defined(MX_USB_OTG_FS_Overrcurrent_Pin))
      HAL_GPIO_DeInit (MX_USB_OTG_FS_Overcurrent_GPIOx, MX_USB_OTG_FS_Overcurrent_GPIO_Pin);
#endif
    }
  }
#endif

#ifdef RTE_DEVICE_FRAMEWORK_CUBE_MX
#if  (defined(USE_STM32756G_EVAL))              // Host VBUS power driving pin is on IO expander
  if ((pins_mask & ARM_USB_PIN_VBUS) != 0U) {
    if (otg_fs_role == ARM_USB_ROLE_HOST) {
      BSP_IO_ConfigPin(IO_PIN_7, IO_MODE_OFF);
    }
  }
  if ((pins_mask & ARM_USB_PIN_OC) != 0U) {
    if (otg_fs_role == ARM_USB_ROLE_HOST) {
      BSP_IO_ConfigPin(IO_PIN_6, IO_MODE_OFF);
    }
  }
#endif
#endif
}

/**
  \fn          void OTG_FS_PinVbusOnOff (bool state)
  \brief       Drive VBUS Pin On/Off.
  \param[in]   state    State On/Off (true = On, false = Off)
*/
void OTG_FS_PinVbusOnOff (bool state) {

  if (otg_fs_role == ARM_USB_ROLE_HOST) {
#if  (USB_OTG_FS_VBUS_Power_Pin_Active == 0)    // VBUS active low
#if  (defined(USE_STM32756G_EVAL))              // Host VBUS power driving pin is on IO expander
    BSP_IO_WritePin (IO_PIN_7, ((state == true) ? BSP_IO_PIN_RESET : BSP_IO_PIN_SET));
#elif (defined(MX_USB_OTG_FS_VBUS_Power_Pin))   // Host VBUS power driving pin is GPIO (output)
    HAL_GPIO_WritePin (MX_USB_OTG_FS_VBUS_Power_GPIOx, MX_USB_OTG_FS_VBUS_Power_GPIO_Pin, ((state == true) ? GPIO_PIN_RESET : GPIO_PIN_SET));
#endif
#else                                           // VBUS active high
#if  (defined(USE_STM32756G_EVAL))              // Host VBUS power driving pin on IO expander
    BSP_IO_WritePin (IO_PIN_7, ((state == true) ? BSP_IO_PIN_SET   : BSP_IO_PIN_RESET));
#elif (defined(MX_USB_OTG_FS_VBUS_Power_Pin))   // Host VBUS power driving pin is GPIO (output)
    HAL_GPIO_WritePin (MX_USB_OTG_FS_VBUS_Power_GPIOx, MX_USB_OTG_FS_VBUS_Power_GPIO_Pin, ((state == true) ? GPIO_PIN_SET   : GPIO_PIN_RESET));
#endif
#endif
  }
}

/**
  \fn          bool OTG_FS_PinGetOC (void)
  \brief       Get state of Overcurrent Pin.
  \return      overcurrent state (true = Overcurrent active, false = No overcurrent)
*/
bool OTG_FS_PinGetOC (void) {

  if (otg_fs_role == ARM_USB_ROLE_HOST) {
#if  (USB_OTG_FS_Overcurrent_Pin_Active == 0)   // Overcurrent active low
#if  (defined(USE_STM32756G_EVAL))              // Host overcurrent sensing pin is on IO expander
    return (BSP_IO_ReadPin (IO_PIN_6) == BSP_IO_PIN_RESET ? true : false);
#elif (defined(MX_USB_OTG_FS_Overrcurrent_Pin)) // Host overcurrent sensing pin is GPIO (input)
    return ((HAL_GPIO_ReadPin (MX_USB_OTG_FS_Overcurrent_GPIOx, MX_USB_OTG_FS_Overcurrent_GPIO_Pin) == GPIO_PIN_RESET) ? true : false);
#endif
#else                                           // Overcurrent active high
#if  (defined(USE_STM32756G_EVAL))              // Host overcurrent sensing pin is on IO expander
    return (BSP_IO_ReadPin (IO_PIN_6) == BSP_IO_PIN_SET ? true : false);
#elif (defined(MX_USB_OTG_FS_Overrcurrent_Pin)) // Host overcurrent sensing pin is GPIO (input)
    return ((HAL_GPIO_ReadPin (MX_USB_OTG_FS_Overcurrent_GPIOx, MX_USB_OTG_FS_Overcurrent_GPIO_Pin) == GPIO_PIN_SET)   ? true : false);
#endif
#endif
  }
  return false;
}
