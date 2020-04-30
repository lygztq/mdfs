#include <sstream>

#include "mdfs/namenode/inode_directory.hpp"

namespace mdfs { namespace namenode {

void INodeDirectory::destroySubTree() {
    for (auto child : m_children) {
        child.second->destroySubTree();
        delete child.second;
    }
}

INodeDirectory::~INodeDirectory() {
    destroySubTree();
}

INode * INodeDirectory::removeChild(std::string name) {
    auto iter = m_children.find(name);
    if (iter == m_children.end()) return nullptr;
    m_children.erase(iter);
    iter->second->setParent(nullptr);
    return iter->second;
}

bool INodeDirectory::insertChild(INode * node) {
    if (m_children.find(node->getName()) != m_children.end()) {
        std::cerr << node->getName() << " has already exists in " << this->absolutePath() << std::endl;
        return false;
    }
    m_children[node->getName()] = node;
    node->setParent(this);
    return true;
}

void INodeDirectory::destroyAndCollectBlocks(std::vector<common::Block> & vec) {
    if (!m_children.empty()) {
        for (auto child : m_children) {
            child.second->destroyAndCollectBlocks(vec);
            delete child.second;
        }
        m_children.clear();
    }
}

void INodeDirectory::collectBlocks(std::vector<common::Block> & vec) const {
    if (!m_children.empty()) {
        for (const auto & child : m_children) {
            child.second->collectBlocks(vec);
        }
    }
}

std::string INodeDirectory::listSelf() const {
    std::ostringstream oss;
    if (!isRoot()) oss << ".." << std::endl;
    oss << "." << std::endl;
    for (const auto & child : m_children) {
        oss << child.second->getName() << std::endl;
    }
    return oss.str();
}

} // namespace namenode
} // namespace mdfs
