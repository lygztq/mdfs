#ifndef MDFS_NAMENODE_INODE_HPP_
#define MDFS_NAMENODE_INODE_HPP_

#include <cstdint>
#include <string>
#include <ctime>

#include "mdfs/common/permission.hpp"
#include "mdfs/common/types.hpp"
#include "mdfs/common/constants.hpp"

namespace mdfs { namespace namenode {

class INode {
private:
    std::string m_name;
    uint64_t m_permission;
    uint64_t m_modificationTime;
    uint64_t m_accessTime;

public:
    INode() = default;
    INode(std::string name, uint64_t permission, uint64_t modificationTime, uint64_t accessTime)
    : m_name(name), m_permission(permission), m_modificationTime(modificationTime), m_accessTime(accessTime) {}
    INode(char * name, uint64_t permission, uint64_t modificationTime, uint64_t accessTime)
    : m_name(name), m_permission(permission), m_modificationTime(modificationTime), m_accessTime(accessTime) {}

    std::string getName() const { return m_name; }
    common::ClientId getClientId() const { return common::PermissionFormat::getClientId(m_permission); }
    common::GroupId getGroupId() const { return common::PermissionFormat::getGroupId(m_permission); }
    common::ModeType getMode() const { return common::PermissionFormat::getMode(m_permission); }
    uint64_t getModificationTime() const { return m_modificationTime; }
    uint64_t getAccessTime() const { return m_accessTime; }

    void setName(const std::string & name) { m_name = name; }
    void setPermission(common::ClientId cid, common::GroupId gid, common::ModeType mode) { 
        m_permission = common::PermissionFormat::getPermissionCode(cid, gid, mode); 
    }
    void setClientId(common::ClientId cid) { setPermission(cid, getGroupId(), getMode()); }
    void setGroupId(common::GroupId gid) { setPermission(getClientId(), gid, getMode()); }
    void setMode(common::ModeType mode) { setPermission(getClientId(), getGroupId(), mode); }
    void setModificationTime(uint64_t time) { m_modificationTime = time; }
    void setAccessTime(uint64_t time) { m_accessTime = time; }
    void updateModificationTime() { m_modificationTime = static_cast<uint64_t>(time(nullptr)); }
    void updateAccessTime() { m_accessTime = static_cast<uint64_t>(time(nullptr)); }
};

} // namespace namenode
} // namespace mdfs


#endif // MDFS_NAMENODE_INODE_HPP_