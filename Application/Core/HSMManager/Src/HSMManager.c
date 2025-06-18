#include "../../HSMManager/Inc/HSMManager.h"
#include "Logger.h"
#include "PacketParser.h"
#include "SaltManager.h"
#include "KeyManager.h"
#include "FlashManager.h"

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

static const char* GetCommandName(uint8_t cmd)
{
    switch (cmd)
    {
        case CMD_ENCRYPT:   	return "Encryption Operation";
        case CMD_DECRYPT:   	return "Decryption Operation";
        case CMD_HASH:      	return "Hashing Operation";
        case CMD_RANDOM:   		return "RNG Operation";
        case CMD_OTP:   		return "OTP Operation";
        case CMD_KEY_GEN:   	return "Key Generation Operation";
        case CMD_KEY_STORE:     return "Key Store Operation";
        case CMD_KEY_ERASE:   	return "Key Erase Operation";
        case CMD_PING:   		return "Device Ping Operation";
        default:            	return "Unknown Command";
    }
}

static const char* GetOptionName(uint8_t option)
{
    switch (option)
    {
    	case OPTION_ENCRYP_KEY128:	return "128-bit Encryption";
		case OPTION_ENCRYP_KEY256:	return "256-bit Encryption";
		case OPTION_DECRYP_KEY128:	return "128-bit Decryption";
		case OPTION_DECRYP_KEY256:	return "256-bit Decryption";
		case OPTION_HASH_SHA224:	return "SHA224 Hashing Algorithm";
		case OPTION_HASH_SHA256:	return "SHA256 Hashing Algorithm";
		case OPTION_HASH_SHA384:	return "SHA384 Hashing Algorithm";
		case OPTION_HASH_SHA512:	return "SHA512 Hashing Algorithm";
		case OPTION_HMAC_SHA224:	return "SHA224 HMAC Algorithm";
		case OPTION_HMAC_SHA256:	return "SHA256 HMAC Algorithm";
		case OPTION_RNG_0004:		return "4-byte Random Number Generation";
		case OPTION_RNG_0008:		return "8-byte Random Number Generation";
		case OPTION_RNG_0016:		return "16-byte Random Number Generation";
		case OPTION_RNG_0024:		return "24-byte Random Number Generation";
		case OPTION_RNG_0032:		return "32-byte Random Number Generation";
		case OPTION_RNG_0048:		return "48-byte Random Number Generation";
		case OPTION_RNG_0064:		return "64-byte Random Number Generation";
		case OPTION_RNG_0066:		return "66-byte Random Number Generation";
		case OPTION_RNG_0128:		return "128-byte Random Number Generation";
		case OPTION_RNG_0256:		return "256-byte Random Number Generation";
		case OPTION_RNG_0512:		return "512-byte Random Number Generation";
		case OPTION_RNG_1024:		return "1024-byte Random Number Generation";
		case OPTION_RNG_2048:		return "2048-byte Random Number Generation";
		case OPTION_RNG_4096:		return "4096-byte Random Number Generation";
		default:					return "Unknown Command"; 	break;
    }
}

static void LogParsedPacket(const ParsedPacket_t* packet)
{
    if (!packet)
    {
        log_error("ParsedPacket is NULL");
        return;
    }

    log_debug("PARSED PACKET:");
    log_debug("TXID       : %u", packet->transactionID);
    log_debug("CMD        : %s", GetCommandName(packet->cmd));
    log_debug("OPTION     : %s", GetOptionName(packet->option));
    log_debug("INPUT SIZE : %hu bytes", packet->inputSize);

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
    // log_debug("EOD FLAG    : %04X", packet->out_eod_flag);
}

static void LogResponsePacket(const ResponsePacket_t* packet)
{
    if (!packet)
    {
        log_error("ResponsePacket is NULL");
        return;
    }

    log_debug("RESPONSE PACKET");
    log_debug("TXID        : %u", packet->transactionID);
    log_debug("OUTPUT SIZE : %u bytes", packet->outputSize);

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
    log_debug("EOD FLAG    : %04X", packet->out_eod_flag);
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
            log_debug("                   %s", line);  // 17-character indent to align with line 1
    }
}

void HSMManager_Init(void)
{
	log_init(LOG_LEVEL_DEBUG);
	log_info("Initializing HSM Device.");
	HAL_Delay(1000);
	log_info("Turning LED ON.");
	BSP_LED_Toggle(LED_GREEN);  HAL_Delay(1000);
	BSP_LED_Toggle(LED_RED);    HAL_Delay(1000);
	BSP_LED_Toggle(LED_YELLOW); HAL_Delay(1000);
	log_clearline();
	log_info("Turning LED OFF.");
	BSP_LED_Toggle(LED_GREEN);  HAL_Delay(1000);
	BSP_LED_Toggle(LED_RED);    HAL_Delay(1000);
	BSP_LED_Toggle(LED_YELLOW); HAL_Delay(1000);
	log_clearline();

	SaltManager_Init();
	FlashManager_Init();
	KeyManager_Init();

	log_info("Turning LED ON.");
	BSP_LED_Toggle(LED_GREEN);  HAL_Delay(1000);
	BSP_LED_Toggle(LED_RED);    HAL_Delay(1000);
	BSP_LED_Toggle(LED_YELLOW); HAL_Delay(1000);
	log_clearline();
	log_info("Turning LED OFF.");
	BSP_LED_Toggle(LED_GREEN);  HAL_Delay(1000);
	BSP_LED_Toggle(LED_RED);    HAL_Delay(1000);
	BSP_LED_Toggle(LED_YELLOW); HAL_Delay(1000);
	log_clearline();
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
			 response.transactionID = request.transactionID;
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
