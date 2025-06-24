#ifndef INC_CONSTANTS_H_
#define INC_CONSTANTS_H_

// === Command Codes (CMD field) ===
#define CMD_ENCRYPT               0x01 // 0b00000001
#define CMD_DECRYPT               0x02 // 0b00000010
#define CMD_HASH                  0x03 // 0b00000011
#define CMD_HMAC				  0x04 // 0b00000100
#define CMD_RANDOM                0x05 // 0b00000100
#define CMD_OTP					  0x06 // 0b00000101
#define CMD_KEY_GEN	              0x07 // 0b00000101
#define CMD_KEY_STORE             0x08 // 0b00000101
#define CMD_KEY_DELETE            0x09 // 0b00000101
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
// -- HMAC Modes
#define OPTION_HMAC_SHA224        0x31 // 0b00100101
#define OPTION_HMAC_SHA256        0x32 // 0b00100110
// -- Random
#define OPTION_RNG_0004           0x41 // 0b00110001
#define OPTION_RNG_0008           0x42 // 0b00110010
#define OPTION_RNG_0016           0x43 // 0b00110011
#define OPTION_RNG_0024           0x44 // 0b00110100
#define OPTION_RNG_0032           0x45 // 0b00110101
#define OPTION_RNG_0048           0x46 // 0b00110110
#define OPTION_RNG_0064           0x47 // 0b00110111
#define OPTION_RNG_0066			  0x48 // 0b00111000
#define OPTION_RNG_0128           0x49 // 0b00111001
#define OPTION_RNG_0256           0x4A // 0b00111010
#define OPTION_RNG_0384			  0x4B // 0b00111011
#define OPTION_RNG_0512           0x4C // 0b00111100
#define OPTION_RNG_1024           0x4D // 0b00111101
#define OPTION_RNG_2048           0x4E // 0b00111110
#define OPTION_RNG_4096           0x4F // 0b00111111
// -- Key Manager
#define OPTION_KEY_GEN			  0x51
#define OPTION_KEY_STORE		  0x52
#define OPTION_KEY_DELETE		  0x53

// -- Encryption Key State
#define ENC_KEY_BYOK			  0xF1
#define ENC_KEY_GYOK			  0xF2
#define ENC_KEY_DABA			  0xF3
// -- Encryption IV State
#define ENC_IV_BYIV				  0xE1
#define ENC_IV_GYIV				  0xE2

// -- Decryption Key State
#define DEC_KEY_BYOK			  0xD1
#define DEC_KEY_DABA			  0xD2
// -- Decryption IV State
#define DEC_IV_BYIV				  0xC1
#define DEC_IV_DABA               0xC2

// -- HMAC Key State
#define HMAC_KEY_BYOK			  0xB1
#define HMAC_KEY_GYOK			  0xB2
#define HMAC_KEY_DABA			  0xB3

// === End of Data Flag (EOD field) ===
#define IN_EOD_FLAG				  0xDEADBEEF
#define OUT_EOD_FLAG			  0xCAFEBABE

#endif /* INC_CONSTANTS_H_ */
