#ifndef MDFS_NAMENODE_INODE_FILE_HPP_
#define MDFS_NAMENODE_INODE_FILE_HPP_

#include "mdfs/namenode/inode.hpp"

namespace mdfs { namespace namenode {

class INodeFile : public INode {
private:
    uint32_t m_blockSize; // num of byte per block
    std::vector<common::Block> m_blocks;
    bool m_writing; // is writing
public:
    INodeFile() = default;
    INodeFile(uint32_t blockSize):m_blockSize(blockSize), m_writing(false) {}

    // =====================
    // read/write lock functions
    // =====================
    // bool isWriting() { return m_writing; }
    // bool tryUsing() {
    //     if (m_writing) return false;
    //     m_writing = true;
    //     return true;
    // }
    // void release() { m_writing = false; }

    // =====================
    // read/write blocks
    // =====================
    void addBlock(common::Block blk);
    std::vector<common::Block> getBlocks();

    // =====================
    // file tree operations
    // =====================
    virtual bool isDirectory() const override { return false; }
    virtual bool isFile() const override { return true; }
    virtual void destroyAndCollectBlocks(std::vector<common::Block> & vec) override;
    virtual INode* getChild(std::string childName) const { return nullptr; }
    virtual void collectBlocks(std::vector<common::Block> & vec) const override;

    virtual std::string listSelf() const override;
    virtual void destroySubTree() override;
    virtual INode * removeChild(std::string name) { return nullptr; }
    virtual bool insertChild(INode * node) { return false; }
};

} // namespace namenode
} // namespace mdfs


#endif // MDFS_NAMENODE_INODE_FILE_HPP_
