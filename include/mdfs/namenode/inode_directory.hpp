#ifndef MDFS_NAMENODE_INODE_DIRECTORY_HPP_
#define MDFS_NAMENODE_INODE_DIRECTORY_HPP_

#include <unordered_map>

#include "mdfs/namenode/inode.hpp"

namespace mdfs { namespace namenode {

class INodeDirectory : public INode {
private:
    std::unordered_map<std::string, INode*> m_children; // localname, ptr

public:
    INodeDirectory() = default;
    INodeDirectory(const std::string & name, INode * parent,
                   uint64_t permission, uint64_t modificationTime, uint64_t accessTime) 
                   : INode(name, parent, permission, modificationTime, accessTime) {}
    virtual ~INodeDirectory() override;

    // =====================
    // file tree operations
    // =====================
    virtual bool isDirectory() const override { return true; }
    virtual bool isFile() const override { return false; }

    // =====================
    // node management functions
    // =====================

    // remove this child node from subtree but not delete it.
    virtual INode * getChild(std::string name) const override {
        if (name == "." || (name.empty() && this->isRoot())) return (INode*) this;
        auto iter = m_children.find(name);
        return (iter == m_children.end()) ? nullptr : iter->second;
    }
    virtual void destroyAndCollectBlocks(std::unordered_map<common::Block, size_t> & collect_blks) override;
    virtual void collectBlocks(std::unordered_map<common::Block, size_t> & collect_blks) const override;
    virtual std::string listSelf() const override;
    virtual void destroySubTree() override;
    virtual INode * removeChild(std::string name) override;
    INode * removeChild(INode * node) { return removeChild(node->getName()); }
    virtual bool insertChild(INode * node) override;
    virtual bool hasChild(std::string name) const override { return m_children.find(name) != m_children.end(); }
    virtual INode * deepcopy() const override {
        INode * copy = new INodeDirectory(getName(), getParentPtr(), getPermission(), getModificationTime(), getAccessTime());
        for (const auto & item : m_children) {
            INode * childCopy = item.second->deepcopy();
            copy->insertChild(childCopy);
        }
        return copy;
    }

    virtual void store(std::ofstream & ofs) const {}
    virtual void load(std::ifstream & ifs) {}
};

} // namespace namenode    
} // namespace mdfs


#endif // MDFS_NAMENODE_INODE_DIRECTORY_HPP_
