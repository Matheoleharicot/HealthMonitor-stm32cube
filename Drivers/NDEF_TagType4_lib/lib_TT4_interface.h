/**
  ******************************************************************************
  * @file    lib_TT4_interface.h
  * @author  MMY Application Team
  * @brief   This file help to manage TagType4 NDEF library.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        http://www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIB_TT4_INTERFACE_H
#define __LIB_TT4_INTERFACE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF_URI.h"

/** @addtogroup NFC_libraries
  * @{
  */
  
/** @addtogroup TAGTYPE4_NDEF_INTERFACE
  * @{
  */

/**
 * @defgroup TAGTYPE4_NDEF_INTERFACE_Exported_Types             TAGTYPE4_NDEF_INTERFACE Exported Types
 * @{
 */

/**
  * @brief  CCfile structure
  */
typedef struct
{
  uint16_t NumberCCByte;
  uint8_t Version;
  uint16_t MaxReadByte;
  uint16_t MaxWriteByte;
  uint8_t TField;
  uint8_t LField;
  uint16_t FileID;
  uint16_t NDEFFileMaxSize;
  uint8_t ReadAccess;
  uint8_t WriteAccess;
}sCCFileInfo;

/**
  * @brief  Tag type supported
  */
typedef enum 
{
  UNKNOWN_TAG = 0,
  INTERFACE_TAG,
  URI_TAG,
  SMS_TAG,
  GEO_TAG,
  EMAIL_TAG,
  TEXT_TAG,
  BT_TAG,
  WIFI_TAG
} Tag_TypeDef;

/**
  * @}
  */
    
/**
 * @defgroup TAGTYPE4_NDEF_INTERFACE_Exported_FunctionPrototypes             TAGTYPE4_NDEF_INTERFACE Exported FunctionPrototypes
 * @{
 */

uint16_t TT4_Init( void );
uint16_t TT4_ReadURI(sURI_Info *pURI);
uint16_t TT4_WriteURI(sURI_Info *pURI);
/**
  * @}
  */ 
  
/**
  * @}
  */  
  
/**
  * @}
  */  

#ifdef __cplusplus
}
#endif

#endif /* __LIB_TT4_INTERFACE_H */


/******************* (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
