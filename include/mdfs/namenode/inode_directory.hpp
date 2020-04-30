#ifndef MDFS_NAMENODE_INODE_DIRECTORY_HPP_
#define MDFS_NAMENODE_INODE_DIRECTORY_HPP_

#include <unordered_map>

#include "mdfs/namenode/inode.hpp"

namespace mdfs { namespace namenode {

class INodeDirectory : public INode {
private:
    std::unordered_map<std::string, INode*> m_children; // localname, ptr

public:
    virtual ~INodeDirectory();
    // node management functions
    // remove this child node from subtree but not delete it.
    INode * removeChild(std::string name);
    INode * removeChild(INode * node) {
        return removeChild(node->getName());
    }
    bool insertChild(INode * node);
    INode * getChild(std::string name) {
        auto iter = m_children.find(name);
        return (iter == m_children.end()) ? nullptr : iter->second;
    }

    // block collection
    virtual void destroyAndCollectBlocks(std::vector<common::Block> & vec);
    virtual void collectBlocks(std::vector<common::Block> & vec) const;

    // list
    virtual std::string listSelf() const;

    virtual void destroySubTree();
};

} // namespace namenode    
} // namespace mdfs


#endif // MDFS_NAMENODE_INODE_DIRECTORY_HPP_
