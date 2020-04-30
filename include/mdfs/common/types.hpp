#ifndef MDFS_COMMON_TYPES_HPP_
#define MDFS_COMMON_TYPES_HPP_

#include <cstdint>
#include <cstring>

namespace mdfs { namespace common {

using ClientId = uint32_t;
using GroupId = uint32_t;
using BlockId = uint64_t;
using INodeId = uint64_t;
using ModeType = uint16_t;

enum class OperationStatusCode {
    SUCCESS,
    FAILED,
    UNKNOWN
};

struct OperationStatus {
    OperationStatusCode status;
    int message_len;
    char * message;
    OperationStatus(OperationStatusCode code, char * message):status(code) {
        strcpy(this->message, message);
        message_len = strlen(this->message);
    }
};

} // namespace common
} // namespace msfs


#endif //MDFS_COMMON_TYPES_HPP_
