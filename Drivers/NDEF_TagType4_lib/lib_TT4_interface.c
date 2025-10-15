/**
  ******************************************************************************
  * @file    lib_TT4_interface.c
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

/* Includes ------------------------------------------------------------------*/
#include "lib_NDEF_private.h"
#include "lib_TT4_interface.h"

/** @addtogroup NFC_libraries
 *  @{
 */
 
/** @addtogroup TAGTYPE4_NDEF_INTERFACE
 *  @{
 */

/** @defgroup TAGTYPE4_NDEF_INTERFACE_Private_Variables TAGTYPE4 NDEF Variables 
  * @{
  */
  
/**
 * @brief  This buffer contains the data send/received by TAG
 */
uint8_t NDEF_Buffer [NFC_TT4_NDEF_MAX_SIZE];

/**
 * @brief  This structure contains the data of the CC file
 */
sCCFileInfo CCFileStruct;

/**
 * @brief  This structure contains the information encapsuled in the record header
 *         with few more for SW purpose
 */
sRecordInfo RecordStruct;

/**
  * @}
  */

/** @defgroup TAGTYPE4_NDEF_INTERFACE_Private_Functions  TAGTYPE4_NDEF_INTERFACE Private Functions
  * @{
  */

/**
  * @}
  */


/** @defgroup libTT4_Public_Functions
  * @{
  *  @brief  This file is used to access tag type 4.
  */ 

/**
  * @brief  This fonction initialize Tag Type 4
  * @param  None 
  * @retval SUCCESS : Initialization done
  */
uint16_t TT4_Init (void)
{
  uint16_t status = NDEF_SUCCESS;
  uint8_t CCBuffer[15];
  sCCFileInfo *pCCFile;
  
  pCCFile = &CCFileStruct;

  status = NFC_TT4_Initialization( CCBuffer, sizeof(CCBuffer));
  
  if( status == NDEF_SUCCESS)
  {
    pCCFile->NumberCCByte = (uint16_t) ((CCBuffer[0x00]<<8) | CCBuffer[0x01]);
    pCCFile->Version = CCBuffer[0x02];
    pCCFile->MaxReadByte = (uint16_t) ((CCBuffer[0x03]<<8) | CCBuffer[0x04]);
    pCCFile->MaxWriteByte = (uint16_t) ((CCBuffer[0x05]<<8) | CCBuffer[0x06]);
    pCCFile->TField = CCBuffer[0x07];
    pCCFile->LField = CCBuffer[0x08];
    pCCFile->FileID = (uint16_t) ((CCBuffer[0x09]<<8) | CCBuffer[0x0A]);
    pCCFile->NDEFFileMaxSize = (uint16_t) ((CCBuffer[0x0B]<<8) | CCBuffer[0x0C]);
    pCCFile->ReadAccess = CCBuffer[0x0D];
    pCCFile->WriteAccess = CCBuffer[0x0E];  
  }
  
  return status;
}  

/**
  * @brief  This fonction read NDEF file
  * @param  pNDEF : pointer on buffer to fill with NDEF data
  * @retval SUCCESS : NDEF file data have been retrieve
  * @retval ERROR : Not able to get NDEF file data
  */
uint16_t TT4_ReadNDEF(uint8_t *pNDEF)
{
  uint16_t status = NDEF_ERROR;
  sCCFileInfo *pCCFile;
  
  pCCFile = &CCFileStruct;
  
  if(NFC_TT4_OpenNDEFSession(pCCFile->FileID, NFC_TT4_ASK_FOR_SESSION) == NDEF_SUCCESS)
  {
    status = NDEF_ReadNDEF(pNDEF);
    NFC_TT4_CloseNDEFSession(pCCFile->FileID);
  }
  
  return status;
}

/**
  * @brief  This fonction write NDEF file
  * @param  pNDEF : pointer on buffer that contain the NDEF data
  * @retval SUCCESS : NDEF file data have been stored
  * @retval ERROR : Not able to store NDEF file
  */
uint16_t TT4_WriteNDEF(uint8_t *pNDEF)
{
  uint16_t status = NDEF_ERROR;
  sCCFileInfo *pCCFile;
  
  pCCFile = &CCFileStruct;
  
  if(NFC_TT4_OpenNDEFSession(pCCFile->FileID, NFC_TT4_ASK_FOR_SESSION) == NDEF_SUCCESS)
  {
    status = NDEF_WriteNDEF( pNDEF);
    NFC_TT4_CloseNDEFSession(pCCFile->FileID);
  }
  
  return status;
}

/**
  * @brief  This fonction read NDEF file if NDEF is identified as URI
  * @param  pURI : pointer on URI structure to fill with read data
  * @retval SUCCESS : URI structure has been updated
  * @retval ERROR : Not able to fill URI structure
  */
uint16_t TT4_ReadURI(sURI_Info *pURI)
{
  uint16_t status = NDEF_ERROR;
  sCCFileInfo *pCCFile;
  sRecordInfo *pRecordStruct;
  
  pCCFile = &CCFileStruct;
  pRecordStruct = &RecordStruct;
  
  
  if(NFC_TT4_OpenNDEFSession(pCCFile->FileID, NFC_TT4_ASK_FOR_SESSION) == NDEF_SUCCESS)
  {
    if(NDEF_IdentifyNDEF( pRecordStruct, NDEF_Buffer) == NDEF_SUCCESS)
    {
      status = NDEF_ReadURI(pRecordStruct, pURI);
    }
    NFC_TT4_CloseNDEFSession(pCCFile->FileID);
  }
  
  return status;
}

/**
  * @brief  This fonction write NDEF file from data given in the URI structure
  * @param  pURI : pointer on URI structure to prepare NDEF
  * @retval SUCCESS : NDEF URI stored
  * @retval ERROR : Not able to store NDEF URI
  */
uint16_t TT4_WriteURI(sURI_Info *pURI)
{
  uint16_t status = NDEF_ERROR;
  sCCFileInfo *pCCFile;
  
  pCCFile = &CCFileStruct;
  
  if(NFC_TT4_OpenNDEFSession(pCCFile->FileID, NFC_TT4_ASK_FOR_SESSION) == NDEF_SUCCESS)
  {
    status = NDEF_WriteURI ( pURI );
    NFC_TT4_CloseNDEFSession(pCCFile->FileID);
  }
  
  return status;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

