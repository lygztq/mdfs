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

// operation status code
enum class OperationStatusCode {
    SUCCESS = 0,
    FAILED,
    NOT_EXIST,
    NOT_FILE,
    NOT_DIR,
    ALREADY_EXIST,
    ROOT_ERROR, // cannot delete or move root
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

// command sent by client to namenode
enum class ClientCommand {
    NONE = 0,
    READ = 1,
    WRITE = 2,
    REMOVE = 3,
    CREATE = 4,
    MKDIR = 5,
    NEW_BLCOK = 6,
    LIST = 7,
    COPY = 8,
    MOVE = 9
};

// command used for data transformation
enum class DataTransferCommand {
    READ,
    WRITE
};

// command used by namenode to manage file system
enum class ManageCommand {
    DELETE,
    READ_FROM,
    WRITE_TO,
    DO_CHECKSUM
};

} // namespace common
} // namespace msfs


#endif //MDFS_COMMON_TYPES_HPP_
