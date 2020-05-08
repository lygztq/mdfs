#ifndef MDFS_COMMON_CONSTANTS_HPP_
#define MDFS_COMMON_CONSTANTS_HPP_

#include "mdfs/common/types.hpp"

namespace mdfs { namespace common {

// clientId constant
const ClientId NULL_CLIENT_ID = 0;
const ClientId MAX_CLIENT_ID = 0xffffff;

inline bool validClientId(ClientId cid) { return cid > NULL_CLIENT_ID && cid <= MAX_CLIENT_ID; }

const uint64_t BLOCK_SIZE = 16777216; // 2 ^ 24 Byte = 16MB
const time_t INIT_HEARTBEAT_TIME = 0L;

// const uint64_t FIELD_PLACEHOLDER = 0;
#define FIELD_PLACEHOLDER 0

} // namespace common
} // namespace mdfs


#endif // MDFS_COMMON_CONSTANTS_HPP_
