#ifndef MDFS_NAMENODE_INODE_DIRECTORY_HPP_
#define MDFS_NAMENODE_INODE_DIRECTORY_HPP_

#include <unordered_map>

#include "mdfs/namenode/inode.hpp"

namespace mdfs { namespace namenode {

class INodeDirectory : public INode {
private:
    std::unordered_map<std::string, INode*> m_children; // localname, ptr

public:
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
        auto iter = m_children.find(name);
        return (iter == m_children.end()) ? nullptr : iter->second;
    }
    virtual void destroyAndCollectBlocks(std::vector<common::Block> & vec) override;
    virtual void collectBlocks(std::vector<common::Block> & vec) const override;
    virtual std::string listSelf() const override;
    virtual void destroySubTree() override;
    virtual INode * removeChild(std::string name) override;
    INode * removeChild(INode * node) { return removeChild(node->getName()); }
    virtual bool insertChild(INode * node) override;

    virtual void store(std::ofstream & ofs) const {}
    virtual void load(std::ifstream & ifs) {}
};

} // namespace namenode    
} // namespace mdfs


#endif // MDFS_NAMENODE_INODE_DIRECTORY_HPP_
