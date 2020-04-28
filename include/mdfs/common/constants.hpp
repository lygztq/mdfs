#ifndef MDFS_COMMON_CONSTANTS_HPP_
#define MDFS_COMMON_CONSTANTS_HPP_

#include "mdfs/common/types.hpp"

namespace mdfs { namespace common {

// clientId constant
const ClientId NULL_CLIENT_ID = 0;
const ClientId MAX_CLIENT_ID = 0xffffff;

inline bool validClientId(ClientId cid) { return cid > NULL_CLIENT_ID && cid <= MAX_CLIENT_ID; } 

} // namespace common
} // namespace mdfs


#endif // MDFS_COMMON_CONSTANTS_HPP_