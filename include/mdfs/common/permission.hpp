#ifndef MDFS_COMMON_PERMISSION_HPP_
#define MDFS_COMMON_PERMISSION_HPP_

#include <cstdint>
#include <string>
#include <utility>

#include "mdfs/common/types.hpp"

namespace mdfs { namespace common {

// <len, bias>
using PermissionFormatArgs = std::pair<int, int>;

// len | bias
enum class PermissionFormatConst : uint32_t {
    MODE = (16 << 16) + 0,
    GROUP = (24 << 16) + 16,
    CLIENT = (24 << 16) + 16 + 24
};

struct PermissionFormat {
    static ClientId getClientId(uint64_t permission);
    static GroupId getGroupId(uint64_t permission);
    static ModeType getMode(uint64_t permission);
    static uint64_t getPermissionCode(ClientId cid, GroupId gid, ModeType mode);
    static uint64_t getValue(uint64_t permission, PermissionFormatArgs args);
    static PermissionFormatArgs getArgs(PermissionFormatConst args);
};

} // namespace common
} // namespace msfs


#endif //MDFS_COMMON_PERMISSION_HPP_