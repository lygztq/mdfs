#include "mdfs/common/permission.hpp"

namespace mdfs { namespace common {

ClientId PermissionFormat::getClientId(uint64_t permission) {
    PermissionFormatArgs args = PermissionFormat::getArgs(PermissionFormatConst::CLIENT);
    return static_cast<ClientId>(getValue(permission, args));
}

GroupId PermissionFormat::getGroupId(uint64_t permission) {
    PermissionFormatArgs args = PermissionFormat::getArgs(PermissionFormatConst::GROUP);
    return static_cast<GroupId>(getValue(permission, args));
}

ModeType PermissionFormat::getMode(uint64_t permission) {
    PermissionFormatArgs args = PermissionFormat::getArgs(PermissionFormatConst::MODE);
    return static_cast<ModeType>(getValue(permission, args));
}

uint64_t PermissionFormat::getPermissionCode(ClientId cid, GroupId gid, ModeType mode) {
    PermissionFormatArgs client_args = getArgs(PermissionFormatConst::CLIENT);
    PermissionFormatArgs group_args = getArgs(PermissionFormatConst::GROUP);
    uint64_t permission = mode;
    permission += (cid << client_args.second);
    permission += (gid << group_args.second);
    return permission;
}

uint64_t PermissionFormat::getValue(uint64_t permission, PermissionFormatArgs args) {
    int len = args.first;
    int bias = args.second;
    permission >>= bias;
    uint64_t mask = UINT64_MAX >> (64 - len);
    return mask ^ permission;
}

PermissionFormatArgs PermissionFormat::getArgs(PermissionFormatConst args) {
    uint32_t i_args = static_cast<uint32_t>(args);
    return PermissionFormatArgs(i_args >> 16, i_args ^ 0xffff);
}

} // namespace mdfs
} // namespace common
