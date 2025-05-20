#include "../../HSMManager/Inc/HSMManager.h"
#include "Logger.h"
#include "PacketParser.h"
#include "usbd_cdc_if.h"
#include "stm32h5xx_nucleo.h"

#define bytesPerLine		16
#define CDC_BLOCK_SIZE		64
#define MAX_USB_DATA_SIZE 	65536

extern uint8_t usb_rx_buffer[MAX_USB_DATA_SIZE];
extern uint32_t usb_rx_index;
extern volatile uint8_t tx_complete;
extern volatile bool usb_rx_complete;

#include <stdio.h>
#include <stdint.h>

static void LogParsedPacket(const ParsedPacket_t* packet)
{
    if (!packet)
    {
        log_error("ParsedPacket is NULL");
        return;
    }

    log_debug("PARSED PACKET:");
    log_debug("TXID       : %08X", packet->transactionID);
    log_debug("CMD        : %02X", packet->cmd);
    log_debug("OPTION     : %02X", packet->option);
    log_debug("INPUT SIZE : %04X", packet->inputSize);

    const uint8_t* data = packet->inputData;
    uint16_t size = packet->inputSize;

    for (uint16_t i = 0; i < size; i += bytesPerLine)
    {
        char line[bytesPerLine * 3 + 1] = {0};
        char* ptr = line;

        for (uint16_t j = 0; j < bytesPerLine && (i + j) < size; ++j)
        {
            ptr += sprintf(ptr, "%02X ", data[i + j]);
        }

        if (i == 0)
        {
        	log_debug("INPUT DATA : %s", line);
        }
        else
        {
        	log_debug("             %s", line);
        }
    }
}

static void LogResponsePacket(const ResponsePacket_t* packet)
{
    if (!packet)
    {
        log_error("ResponsePacket is NULL");
        return;
    }

    log_debug("RESPONSE PACKET");
    log_debug("TXID        : %08X", packet->transactionID);
    log_debug("OUTPUT SIZE : %04X", packet->outputSize);
    log_debug("EOD FLAG    : %08X", packet->out_eod_flag);

    const uint8_t* data = packet->outputData;
    uint16_t size = packet->outputSize;

    for (uint16_t i = 0; i < size; i += bytesPerLine)
    {
        char line[bytesPerLine * 3 + 1] = {0};
        char* ptr = line;

        for (uint16_t j = 0; j < bytesPerLine && (i + j) < size; ++j)
        {
            ptr += sprintf(ptr, "%02X ", data[i + j]);
        }

        if (i == 0)
            log_debug("OUTPUT DATA : %s", line);
        else
            log_debug("              %s", line);
    }
}

static void LogTransmitBuffer(const uint8_t* usb_tx_buffer, uint32_t usb_tx_index)
{
    if (!usb_tx_buffer || usb_tx_index == 0)
    {
        log_debug("USB TX buffer is empty or NULL.");
        return;
    }


    for (uint32_t i = 0; i < usb_tx_index; i += bytesPerLine)
    {
        char line[bytesPerLine * 3 + 1] = {0};
        char* ptr = line;

        for (uint32_t j = 0; j < bytesPerLine && (i + j) < usb_tx_index; ++j)
        {
            ptr += sprintf(ptr, "%02X ", usb_tx_buffer[i + j]);
        }

        if (i == 0)
            log_debug("USB Tx [%lu bytes]: %s", usb_tx_index, line);
        else
            log_debug("                 %s", line);  // 17-character indent to align with line 1
    }
}

void HSMManager_Init(void)
{

  log_init(LOG_LEVEL_DEBUG);
  log_info("HSM Initialized.");
  HAL_Delay(1000);
  log_info("Turning LED ON.");
  BSP_LED_Toggle(LED_GREEN);  HAL_Delay(1000);
  BSP_LED_Toggle(LED_RED);    HAL_Delay(1000);
  BSP_LED_Toggle(LED_YELLOW); HAL_Delay(1000);
  log_info("Turning LED OFF.");
  BSP_LED_Toggle(LED_GREEN);  HAL_Delay(1000);
  BSP_LED_Toggle(LED_RED);    HAL_Delay(1000);
  BSP_LED_Toggle(LED_YELLOW); HAL_Delay(1000);
  log_info("Turning LED ON.");
  BSP_LED_Toggle(LED_GREEN);  HAL_Delay(1000);
  BSP_LED_Toggle(LED_RED);    HAL_Delay(1000);
  BSP_LED_Toggle(LED_YELLOW); HAL_Delay(1000);
  log_info("Turning LED OFF.");
  BSP_LED_Toggle(LED_GREEN);  HAL_Delay(1000);
  BSP_LED_Toggle(LED_RED);    HAL_Delay(1000);
  BSP_LED_Toggle(LED_YELLOW); HAL_Delay(1000);
}


OperationStatus_t HSMManager_ProcessCommand(void)
{
	 if (usb_rx_complete)
	 {
		 ParsedPacket_t request;
		 ResponsePacket_t response;
		 uint8_t usb_tx_buffer[MAX_USB_DATA_SIZE];
		 uint16_t usb_tx_index;
		 ParseStatus_t status = PacketParser_Parse(usb_rx_buffer, usb_rx_index, &request);

		 if (status == PARSE_SUCCESS)
		 {
			 log_debug("Packet Parsed Successfully:");
			 LogParsedPacket(&request);
			 OperationDispatcher_Dispatch(&request, &response);
			 LogResponsePacket(&response);
			 usb_tx_index = response.outputSize + 10;
			 PacketBuilder_Build(&response, usb_tx_buffer, &usb_tx_index);
			 LogTransmitBuffer(usb_tx_buffer, usb_tx_index);
			 USB_Transmit(usb_tx_buffer, usb_tx_index);
		 }
		 else
		 {
			 log_error("Parsing failed with status code: %d", status);
		 }
	 }
	 return 0;
}

	/* ToDo
	 *
	 * Check usb_rx_buffer
	 * Parse the input data stream
	 * Print the parsed out data structure using logger
	 * Call OperationDispatcher_Dispatch()
	 * Print the operation output data using logger
	 * Build the output data stream
	 * Call USB_Transmit()
	 *
	 * */


/*
 * 	log_info("Processing new HSM command...");

    // Validate packet contents, do CRC checks if needed
    // Pass to dispatcher
    return OperationDispatcher_Dispatch(request, response);
 *
 * */
