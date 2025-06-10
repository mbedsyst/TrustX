#ifndef INC_TYPES_H_
#define INC_TYPES_H_

#include <stdint.h>
#include <stdbool.h>



// === Operation Status Codes ===
typedef enum
{
    OPERATION_SUCCESS = 0,
    OPERATION_INVALID_CMD,
    OPERATION_INVALID_OPTION,
    OPERATION_INVALID_DATA,
    OPERATION_FAILURE,
    OPERATION_KEY_NOT_FOUND,
    OPERATION_RANDOM_GEN_FAIL,
    OPERATION_ENCRYPTION_FAIL,
    OPERATION_DECRYPTION_FAIL,
    OPERATION_HASH_FAIL,
    OPERATION_STORAGE_FAIL,
    OPERATION_UNKNOWN_ERROR
} OperationStatus_t;

// === Packet Structure ===
/*typedef struct
{
    uint32_t transaction_id;
    uint8_t command;
    uint8_t option;
    uint16_t input_size;
    uint8_t* input_data;
} ParsedPacket_t;

typedef struct
{
    uint32_t transaction_id;
    uint16_t output_size;
    uint8_t* output_data;
} ResponsePacket_t;*/


#endif /* INC_TYPES_H_ */
