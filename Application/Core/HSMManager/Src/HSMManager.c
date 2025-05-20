#include "../../HSMManager/Inc/HSMManager.h"
#include "Logger.h"
#include "PacketParser.h"
#include "usbd_cdc_if.h"
#include "stm32h5xx_nucleo.h"

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

    log_debug("Parsed Packet:");
    log_debug("TXID      : %08X", packet->transactionID);
    log_debug("CMD       : %02X", packet->cmd);
    log_debug("OPTION    : %02X", packet->option);
    log_debug("INPUT SIZE: %04X", packet->inputSize);

    char inputHex[3 * MAX_INPUT_DATA_SIZE + 1] = {0};
    char *ptr = inputHex;

    for (uint16_t i = 0; i < packet->inputSize; i++)
    {
        ptr += sprintf(ptr, "%02X ", packet->inputData[i]);
    }

    log_debug("INPUT DATA: %s", inputHex);
}

static void LogResponsePacket(const ResponsePacket_t* packet)
{
    if (!packet)
    {
        log_error("ResponsePacket is NULL");
        return;
    }

    log_debug("Response Packet:");
    log_debug("TXID        : %08X", packet->transactionID);
    log_debug("OUTPUT SIZE : %04X", packet->outputSize);

    char outputHex[3 * MAX_OUTPUT_DATA_SIZE + 1] = {0};
    char *ptr = outputHex;

    for (uint16_t i = 0; i < packet->outputSize; i++)
    {
        ptr += sprintf(ptr, "%02X ", packet->outputData[i]);
    }

    log_debug("OUTPUT DATA : %s", outputHex);
    log_debug("EOD FLAG    : %08X", packet->out_eod_flag);
}

static void LogTransmitBuffer(const uint8_t* usb_tx_buffer, uint32_t usb_tx_index)
{
    if (!usb_tx_buffer || usb_tx_index == 0)
    {
        log_debug("USB TX buffer is empty or NULL.");
        return;
    }

    char hexStr[3 * CDC_BLOCK_SIZE + 1] = {0}; // Adjust buffer size if needed
    char* ptr = hexStr;

    for (uint32_t i = 0; i < usb_tx_index && i < CDC_BLOCK_SIZE; ++i)
    {
        ptr += sprintf(ptr, "%02X ", usb_tx_buffer[i]);
    }

    log_debug("USB TX [%lu bytes]: %s", usb_tx_index, hexStr);
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
