#ifndef MDFS_NAMENODE_INODE_DIRECTORY_HPP_
#define MDFS_NAMENODE_INODE_DIRECTORY_HPP_

#include <unordered_map>

#include "mdfs/namenode/inode.hpp"

namespace mdfs { namespace namenode {

class INodeDirectory : public INode {
private:
    std::unordered_map<std::string, INode*> m_children; // localname, ptr

public:

    INode * removeChild(std::string name) {
        auto iter = m_children.find(name);
        if (iter == m_children.end()) return nullptr;
        m_children.erase(iter);
        iter->second->setParent(nullptr);
        return iter->second;
    }

    INode * removeChild(INode * node) {
        return removeChild(node->getName());
    }

    bool insertChild(INode * node) {
        if (m_children.find(node->getName()) != m_children.end()) {
            std::cerr << node->getName() << " has already exists in " << this->absolutePath() << std::endl;
            return false;
        }
        m_children[node->getName()] = node;
        node->setParent(this);
        return true;
    }
};

} // namespace namenode    
} // namespace mdfs


#endif // MDFS_NAMENODE_INODE_DIRECTORY_HPP_
