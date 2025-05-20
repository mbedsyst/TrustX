#ifndef INC_CONSTANTS_H_
#define INC_CONSTANTS_H_

// === Command Codes (CMD field) ===
#define CMD_ENCRYPT               0x01 // 0b00000001
#define CMD_DECRYPT               0x02 // 0b00000010
#define CMD_HASH                  0x03 // 0b00000011
#define CMD_RANDOM                0x04 // 0b00000100
#define CMD_KEY_STORE             0x05 // 0b00000101
#define CMD_KEY_RETRIEVE          0x06 // 0b00000110
#define CMD_KEY_ERASE             0x07 // 0b00000111
#define CMD_PING                  0x0F // 0b00001111    For testing HSM connection

// === Option Codes (OPTION field) ===
// -- Encryption Modes
#define OPTION_ENCRYP_KEY128      0x01 // 0b00000001
#define OPTION_ENCRYP_KEY256      0x02 // 0b00000010
// -- Decryption Modes
#define OPTION_DECRYP_KEY128	  0x11 // 0b00010001
#define OPTION_DECRYP_KEY256	  0x12 // 0b00010010
// -- Hash Modes
#define OPTION_HASH_SHA224        0x21 // 0b00100001
#define OPTION_HASH_SHA256        0x22 // 0b00100010
#define OPTION_HASH_SHA384        0x23 // 0b00100011
#define OPTION_HASH_SHA512        0x24 // 0b00100100
#define OPTION_HMAC_SHA224        0x25 // 0b00100101
#define OPTION_HMAC_SHA256        0x26 // 0b00100110
// -- Random
#define OPTION_RNG_0004           0x31 // 0b00110001
#define OPTION_RNG_0008           0x32 // 0b00110010
#define OPTION_RNG_0016           0x33 // 0b00110011
#define OPTION_RNG_0032           0x34 // 0b00110100
#define OPTION_RNG_0064           0x35 // 0b00110101
#define OPTION_RNG_0128           0x36 // 0b00110110
#define OPTION_RNG_0256           0x37 // 0b00110111
#define OPTION_RNG_0512           0x38 // 0b00111000
#define OPTION_RNG_1024           0x39 // 0b00111001
#define OPTION_RNG_2048           0x3A // 0b00111010
#define OPTION_RNG_4096           0x3B // 0b00111011


// === End of Data Flag (EOD field) ===
#define IN_EOD_FLAG				  0xDEADBEEF
#define OUT_EOD_FLAG			  0xCAFEBABE

#endif /* INC_CONSTANTS_H_ */
