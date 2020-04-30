#ifndef MDFS_NAMENODE_INODE_HPP_
#define MDFS_NAMENODE_INODE_HPP_

#include <cstdint>
#include <string>
#include <ctime>
#include <iostream>

#include "mdfs/common/permission.hpp"
#include "mdfs/common/types.hpp"
#include "mdfs/common/constants.hpp"
#include "mdfs/common/block.hpp"
// #include "mdfs/common/unique.hpp"

namespace mdfs { namespace namenode {

class INodeDirectory;

// Abstract class for index node in file system.
class INode {
private:
    std::string m_name;
    INode * m_parent;
    uint64_t m_permission;
    uint64_t m_modificationTime;
    uint64_t m_accessTime;

public:
    INode() = default;
    virtual ~INode();

    INode(const std::string & name, INode * parent,
          uint64_t permission, uint64_t modificationTime, uint64_t accessTime)
    : m_name(name), m_parent(parent), m_permission(permission)
    , m_modificationTime(modificationTime), m_accessTime(accessTime) {}

    INode(char * name, INode * parent,
          uint64_t permission, uint64_t modificationTime, uint64_t accessTime)
    : m_name(name), m_parent(parent), m_permission(permission)
    , m_modificationTime(modificationTime), m_accessTime(accessTime) {}

    std::string getName() const { return m_name; }
    INode* getParentPtr() const { return m_parent; }
    // INode getParent() const { return *m_parent; }
    common::ClientId getClientId() const { return common::PermissionFormat::getClientId(m_permission); }
    common::GroupId getGroupId() const { return common::PermissionFormat::getGroupId(m_permission); }
    common::ModeType getMode() const { return common::PermissionFormat::getMode(m_permission); }
    uint64_t getModificationTime() const { return m_modificationTime; }
    uint64_t getAccessTime() const { return m_accessTime; }

    void setName(const std::string & name) { m_name = name; }
    void setParent(INode* parent) { m_parent = parent; }
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

    bool isRoot() const { return m_name.size() == 0; }
    std::string absolutePath() const { 
        if (isRoot()) return m_name;
        return m_parent->absolutePath() + '/' + m_name;
    }

    // remove self from subtree.
    bool removeNode() {
        if (m_parent) {
            INodeDirectory * parent = dynamic_cast<INodeDirectory*>(m_parent);
            if (parent) parent->removeChild(m_name);
            else return false;
            return true;
        }
        return true;
    }

    // delete the subtree (but not the root) and return all blocks.
    virtual void destroyAndCollectBlocks(std::vector<common::Block> & vec) = 0;
    virtual void collectBlocks(std::vector<common::Block> & vec) const = 0;

    virtual std::string listSelf() const = 0;

    virtual void destroySubTree() = 0;

    // std::shared_ptr<INode> copyPtr() const { return std::make_shared<INode>(this); }
};

void deleteINode(INode * node) {
    node->removeNode();
    node->destroySubTree();
    delete node;
}

} // namespace namenode
} // namespace mdfs


#endif // MDFS_NAMENODE_INODE_HPP_
