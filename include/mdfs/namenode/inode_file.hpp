#ifndef MDFS_NAMENODE_INODE_FILE_HPP_
#define MDFS_NAMENODE_INODE_FILE_HPP_

#include "mdfs/namenode/inode.hpp"

namespace mdfs { namespace namenode {

class INodeFile : public INode {
private:
    uint64_t m_blockSize; // num of byte per block
    std::vector<common::Block> m_blocks;
    bool m_writing; // is writing
public:
    INodeFile() = default;
    INodeFile(uint64_t blockSize, const std::string & name, INode * parent,
              uint64_t permission, uint64_t modificationTime, uint64_t accessTime)
              : m_blockSize(blockSize)
              , m_writing(false)
              , INode(name, parent, permission, modificationTime, accessTime) {}

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
    std::vector<common::Block> getBlocks() const { return m_blocks; }
    common::Block getBlock(size_t idx) const { return m_blocks[idx]; }
    common::Block LastBlock() const { return m_blocks.back(); }
    bool getBlockById(common::BlockId bid, common::Block & returnBlk) {
        for (const auto & blk : m_blocks) {
            if (blk.getId() == bid) {
                returnBlk = blk;
                return true;
            }
        }
        return false;
    }
    bool setBlockById(common::BlockId bid, common::Block newValue) {
        if (newValue.getId() != bid) return false;
        for (auto & blk : m_blocks) {
            if (blk.getId() == bid) {
                blk = newValue;
                return true;
            }
        }
        return false;
    }
    bool fullBlock(size_t idx) const { return m_blocks[idx].getNumBytes() >= m_blockSize; }
    bool fullLastBlock() const { return m_blocks.back().getNumBytes() >= m_blockSize; }
    size_t numBlock() const { return m_blocks.size(); }
    uint64_t getBlockSize() const { return m_blockSize; }

    // =====================
    // file tree operations
    // =====================
    virtual bool isDirectory() const override { return false; }
    virtual bool isFile() const override { return true; }
    virtual void destroyAndCollectBlocks(std::unordered_map<common::Block, size_t> & collect_blks) override;
    virtual INode* getChild(std::string childName) const { return nullptr; }
    virtual void collectBlocks(std::unordered_map<common::Block, size_t> & collect_blks) const override;

    virtual std::string listSelf() const override;
    virtual void destroySubTree() override;
    virtual INode * removeChild(std::string name) { return nullptr; }
    virtual bool insertChild(INode * node) { return false; }
    virtual INode * deepcopy() const override {
        INode * copy = new INodeFile(m_blockSize, getName(), getParentPtr(), getPermission(), getModificationTime(), getAccessTime());
        return copy;
    }

    virtual void store(std::ofstream & ofs) const {}
    virtual void load(std::ifstream & ifs) {}
};

} // namespace namenode
} // namespace mdfs


#endif // MDFS_NAMENODE_INODE_FILE_HPP_
