#include <ctime>
#include "mdfs/namenode/filesys_tree.hpp"
#include "mdfs/common/path.hpp"

namespace mdfs { namespace namenode {
using Path = common::Path;
using StatusCode = common::OperationStatusCode;

INode * FileSystemTree::getNodeFromPath(const std::string & des) const {
    Path path(des);
    INode * nodePtr = m_root;
    while (path.nextLayer()) {
        nodePtr = nodePtr->getChild(path.current());
        if (nodePtr == nullptr) return nodePtr;
    }
    return nodePtr;
}

INode * FileSystemTree::getParentFromPath(const std::string & des) const {
    Path path(des);
    INode * nodePtr = m_root;
    while (!path.inLeaf()) {
        nodePtr = nodePtr->getChild(path.current());
        path.nextLayer();
        if (nodePtr == nullptr) return nodePtr;
    }
    return nodePtr;
}

StatusCode FileSystemTree::read(const std::string & des, std::vector<common::Block> & blk_list) const {
    INode * nodePtr = getNodeFromPath(des);
    if (nodePtr == nullptr) return StatusCode::NOT_EXIST;
    if (!nodePtr->isFile()) return StatusCode::NOT_FILE;
    INodeFile * fileNode = dynamic_cast<INodeFile*>(nodePtr);
    blk_list = fileNode->getBlocks();
    return StatusCode::SUCCESS;
}

StatusCode FileSystemTree::write(const std::string & des, common::Block & return_blk, uint64_t & remainSize) {
    INode * nodePtr = getNodeFromPath(des);
    if (nodePtr == nullptr) return StatusCode::NOT_EXIST;
    if (!nodePtr->isFile()) return StatusCode::NOT_FILE;
    INodeFile * fileNode = dynamic_cast<INodeFile*>(nodePtr);
    
    if (fileNode->fullLastBlock()) {
        return_blk = common::Block(common::Block::m_idManager.getId(), 0u, (uint64_t)std::time(0));
        m_count[return_blk.getId()] = 1;
        fileNode->addBlock(return_blk);
        remainSize = fileNode->getBlockSize();
    }
    else {
        return_blk = fileNode->LastBlock();
        remainSize = fileNode->getBlockSize() - return_blk.getNumBytes();
    }
    return StatusCode::SUCCESS;
}

// StatusCode

StatusCode FileSystemTree::create(const std::string & des) {
    Path path(des);
    INode * nodePtr = m_root;
    while (!path.inLeaf()) {
        nodePtr = nodePtr->getChild(path.current());
        path.nextLayer();
        if (nodePtr == nullptr) return StatusCode::NOT_EXIST;
    }
    if (!nodePtr->isDirectory()) return StatusCode::NOT_DIR;
    INodeDirectory * dirPtr = dynamic_cast<INodeDirectory*>(nodePtr);
    INodeFile * newNode = new INodeFile(common::BLOCK_SIZE, path.current(), nodePtr, 
                                        FIELD_PLACEHOLDER, FIELD_PLACEHOLDER, 
                                        FIELD_PLACEHOLDER);
    if (!dirPtr->insertChild(newNode)) {
        delete newNode;
        return StatusCode::ALREADY_EXIST;
    }
    return StatusCode::SUCCESS;
}

StatusCode FileSystemTree::makedir(const std::string & des) {
    Path path(des);
    INode * nodePtr = m_root;
    while (!path.inLeaf()) {
        nodePtr = nodePtr->getChild(path.current());
        path.nextLayer();
        if (nodePtr == nullptr) return StatusCode::NOT_EXIST;
    }
    if (!nodePtr->isDirectory()) return StatusCode::NOT_DIR;
    INodeDirectory * dirPtr = dynamic_cast<INodeDirectory*> (nodePtr);
    INodeDirectory * newNode = new INodeDirectory(path.current(), nodePtr, FIELD_PLACEHOLDER,
                                                  FIELD_PLACEHOLDER, FIELD_PLACEHOLDER);
    if (!dirPtr->insertChild(newNode)) {
        delete newNode;
        return StatusCode::ALREADY_EXIST;
    }
    return StatusCode::SUCCESS;
}

StatusCode FileSystemTree::remove(const std::string & des, std::vector<common::Block> & blksToRemove) {
    // get and check
    INode * nodeToDel = getNodeFromPath(des);
    if (nodeToDel == nullptr) return StatusCode::NOT_EXIST;
    if (nodeToDel->isRoot()) return StatusCode::ROOT_ERROR; // we do not remove root

    // perform operation
    std::unordered_map<common::Block, size_t> collect_blks;
    nodeToDel->destroyAndCollectBlocks(collect_blks);
    nodeToDel->removeNode();
    delete nodeToDel;

    blksToRemove.clear();
    for (const auto & item : collect_blks) {
        common::BlockId bid = item.first.getId();
        if (m_count[bid] == item.second) {
            m_count.erase(bid);
            blksToRemove.push_back(item.first);
        }
        else m_count[bid] -= item.second;
    }
    
    return StatusCode::SUCCESS;
}

StatusCode FileSystemTree::move(const std::string & src, const std::string & des) {
    // get and check
    INode * nodeToMove = getNodeFromPath(src);
    if (nodeToMove == nullptr) return StatusCode::NOT_EXIST;
    if (nodeToMove->isRoot()) return StatusCode::ROOT_ERROR;
    INode * desNode = getNodeFromPath(des);
    if (desNode == nullptr) return StatusCode::NOT_EXIST;
    if (!desNode->isDirectory()) return StatusCode::NOT_DIR;
    if (desNode->hasChild(nodeToMove->getName())) return StatusCode::ALREADY_EXIST;

    // perform operation
    nodeToMove->removeNode();
    nodeToMove->setParent(desNode);
    desNode->insertChild(nodeToMove); // already check before
    return StatusCode::SUCCESS;
}

StatusCode FileSystemTree::copy(const std::string & src, const std::string & des) {
    // get and check
    INode * nodeToCopy = getNodeFromPath(src);
    if (nodeToCopy == nullptr) return StatusCode::NOT_EXIST;
    if (nodeToCopy->isRoot()) return StatusCode::ROOT_ERROR;
    INode * desNode = getNodeFromPath(des);
    if (desNode == nullptr) return StatusCode::NOT_EXIST;
    if (!desNode->isDirectory()) return StatusCode::NOT_DIR;
    if (desNode->hasChild(nodeToCopy->getName())) return StatusCode::ALREADY_EXIST;

    // perform operation
    std::unordered_map<common::Block, size_t> collect_blks;
    nodeToCopy->collectBlocks(collect_blks);
    desNode->insertChild(nodeToCopy->deepcopy()); // already check before
    for (const auto & b : collect_blks) {
        m_count[b.first.getId()] += b.second;
    }
    return StatusCode::SUCCESS;
}

StatusCode FileSystemTree::addBlock(const std::string & des, common::Block & return_blk) {
    INode * nodePtr = getNodeFromPath(des);
    if (nodePtr == nullptr) return StatusCode::NOT_EXIST;
    if (!nodePtr->isFile()) return StatusCode::NOT_FILE;
    INodeFile * fileNode = dynamic_cast<INodeFile*>(nodePtr);
    
    return_blk = common::Block(common::Block::m_idManager.getId(), 0u, (uint64_t)std::time(0));
    m_count[return_blk.getId()] = 1;
    fileNode->addBlock(return_blk);
    return StatusCode::SUCCESS;
}

StatusCode FileSystemTree::rename(const std::string & src, const std::string & newName) {
    INode * nodePtr = getNodeFromPath(src);
    if (nodePtr == nullptr) return StatusCode::NOT_EXIST;
    if (nodePtr->isRoot()) return StatusCode::ROOT_ERROR;
    
    std::string oldName = nodePtr->getName();
    INode * parent = nodePtr->getParentPtr();
    parent->removeChild(oldName);
    nodePtr->setName(newName);
    parent->insertChild(nodePtr);
    return StatusCode::SUCCESS;
}

StatusCode FileSystemTree::blockAck(const std::string & src, common::BlockId bid, uint64_t ackSize) {
    INode * nodePtr = getNodeFromPath(src);
    if (nodePtr == nullptr) return StatusCode::NOT_EXIST;
    if (!nodePtr->isFile()) return StatusCode::NOT_FILE;
    INodeFile * fileNode = dynamic_cast<INodeFile*>(nodePtr);

    common::Block blk;
    if(!fileNode->getBlockById(bid, blk)) return StatusCode::NOT_EXIST;
    blk.setNumBytes(blk.getNumBytes() + ackSize);
    fileNode->setBlockById(bid, blk);
    return StatusCode::SUCCESS;
}

} // namespace namenode
} // namespace mdfs
