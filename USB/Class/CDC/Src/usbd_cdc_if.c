/**
  ******************************************************************************
  * @file    usbd_cdc_if_template.c
  * @author  MCD Application Team
  * @brief   Generic media access Layer.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* BSPDependencies
- "stm32xxxxx_{eval}{discovery}{nucleo_144}.c"
- "stm32xxxxx_{eval}{discovery}_io.c"
EndBSPDependencies */

/* Includes ------------------------------------------------------------------*/
#include <usbd_cdc_if.h>
#include "stdbool.h"
#include "Logger.h"

/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/** Received data over USB are stored in this buffer      */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];
/** Data to send over USB CDC are stored in this buffer   */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];
extern USBD_HandleTypeDef hUsbDeviceFS;

#define CDC_BLOCK_SIZE  	64
#define MAX_USB_DATA_SIZE 	65536

uint8_t usb_rx_buffer[MAX_USB_DATA_SIZE];
uint32_t usb_rx_index = 0;
volatile uint8_t tx_complete = 1;
volatile bool usb_rx_complete = false;

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_CDC
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_CDC_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_CDC_Private_Defines
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_CDC_Private_Macros
  * @{
  */

/**
  * @}
  */


/** @defgroup USBD_CDC_Private_FunctionPrototypes
  * @{
  */

static int8_t TEMPLATE_Init(void);
static int8_t TEMPLATE_DeInit(void);
static int8_t TEMPLATE_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length);
static int8_t TEMPLATE_Receive(uint8_t *pbuf, uint32_t *Len);
static int8_t TEMPLATE_TransmitCplt(uint8_t *pbuf, uint32_t *Len, uint8_t epnum);

USBD_CDC_ItfTypeDef USBD_CDC_Template_fops =
{
  TEMPLATE_Init,
  TEMPLATE_DeInit,
  TEMPLATE_Control,
  TEMPLATE_Receive,
  TEMPLATE_TransmitCplt
};

USBD_CDC_LineCodingTypeDef linecoding =
{
  115200, /* baud rate*/
  0x00,   /* stop bits-1*/
  0x00,   /* parity - none*/
  0x08    /* nb. of bits 8*/
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  TEMPLATE_Init
  *         Initializes the CDC media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t TEMPLATE_Init(void)
{
      USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
      USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
      return (0);
}
/**
  * @brief  TEMPLATE_DeInit
  *         DeInitializes the CDC media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t TEMPLATE_DeInit(void)
{
  /*
     Add your deinitialization code here
  */
  return (0);
}

uint8_t USB_Transmit(uint8_t *data, uint32_t len)
{
    uint32_t offset = 0;
    while (offset < len)
    {
        uint16_t chunk_len = ((len - offset) >= CDC_BLOCK_SIZE) ? CDC_BLOCK_SIZE : (len - offset);
        tx_complete = 0;
        USBD_CDC_SetTxBuffer(&hUsbDeviceFS, &data[offset], chunk_len);

        // Prepare log string
        char log_line[CDC_BLOCK_SIZE * 3 + 1] = {0};
        char *ptr = log_line;
        for (uint16_t i = 0; i < chunk_len; i++)
        {
            ptr += sprintf(ptr, "%02X ", data[offset + i]);
        }

        // Log the data and its size
        log_info("USB Tx [%d bytes]: %s", chunk_len, log_line);

        if (USBD_CDC_TransmitPacket(&hUsbDeviceFS) != USBD_OK)
        {
            return USBD_FAIL;
        }

        uint32_t timeout = 10000;
        while (tx_complete == 0 && timeout--);
        if (timeout == 0)
        {
            log_error("USB TX Timeout");
            return USBD_FAIL;
        }

        offset += chunk_len;
        for (volatile int i = 0; i < 200; i++);
    }
    return USBD_OK;
}

/**
  * @brief  TEMPLATE_Control
  *         Manage the CDC class requests
  * @param  Cmd: Command code
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t TEMPLATE_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length)
{
  UNUSED(length);

  switch (cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:
      /* Add your code here */
      break;

    case CDC_GET_ENCAPSULATED_RESPONSE:
      /* Add your code here */
      break;

    case CDC_SET_COMM_FEATURE:
      /* Add your code here */
      break;

    case CDC_GET_COMM_FEATURE:
      /* Add your code here */
      break;

    case CDC_CLEAR_COMM_FEATURE:
      /* Add your code here */
      break;

    case CDC_SET_LINE_CODING:
      linecoding.bitrate    = (uint32_t)(pbuf[0] | (pbuf[1] << 8) | \
                                         (pbuf[2] << 16) | (pbuf[3] << 24));
      linecoding.format     = pbuf[4];
      linecoding.paritytype = pbuf[5];
      linecoding.datatype   = pbuf[6];

      /* Add your code here */
      break;

    case CDC_GET_LINE_CODING:
      pbuf[0] = (uint8_t)(linecoding.bitrate);
      pbuf[1] = (uint8_t)(linecoding.bitrate >> 8);
      pbuf[2] = (uint8_t)(linecoding.bitrate >> 16);
      pbuf[3] = (uint8_t)(linecoding.bitrate >> 24);
      pbuf[4] = linecoding.format;
      pbuf[5] = linecoding.paritytype;
      pbuf[6] = linecoding.datatype;

      /* Add your code here */
      break;

    case CDC_SET_CONTROL_LINE_STATE:
      /* Add your code here */
      break;

    case CDC_SEND_BREAK:
      /* Add your code here */
      break;

    default:
      break;
  }

  return (0);
}

/**
  * @brief  TEMPLATE_Receive
  *         Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will issue a NAK packet on any OUT packet received on
  *         USB endpoint until exiting this function. If you exit this function
  *         before transfer is complete on CDC interface (ie. using DMA controller)
  *         it will result in receiving more data while previous ones are still
  *         not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t TEMPLATE_Receive(uint8_t *Buf, uint32_t *Len)
{
    uint32_t maxLen = (*Len > CDC_DATA_FS_MAX_PACKET_SIZE) ? CDC_DATA_FS_MAX_PACKET_SIZE : *Len;

    char hexStr[3 * CDC_DATA_FS_MAX_PACKET_SIZE + 1] = {0};
    for (uint32_t i = 0; i < maxLen; ++i)
    {
        snprintf(&hexStr[i * 3], 4, "%02X ", Buf[i]);
    }
    log_info("USB Rx [%lu bytes]: %s", *Len, hexStr);

    if ((usb_rx_index + *Len) < MAX_USB_DATA_SIZE)
    {
        memcpy(&usb_rx_buffer[usb_rx_index], Buf, *Len);
        usb_rx_index += *Len;
    }
    else
    {
        usb_rx_index = 0;
        log_error("USB buffer overflow");
        return USBD_FAIL;
    }

    if (*Len < CDC_DATA_FS_MAX_PACKET_SIZE)
    {
        usb_rx_complete = true;
    }

    USBD_CDC_ReceivePacket(&hUsbDeviceFS);
    return USBD_OK;
}

/**
  * @brief  TEMPLATE_TransmitCplt
  *         Data transmitted callback
  *
  *         @note
  *         This function is IN transfer complete callback used to inform user that
  *         the submitted Data is successfully sent over USB.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t TEMPLATE_TransmitCplt(uint8_t *Buf, uint32_t *Len, uint8_t epnum)
{
  UNUSED(Buf);
  UNUSED(Len);
  UNUSED(epnum);

  return 0;

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

