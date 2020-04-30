#ifndef MDFS_NAMENODE_INODE_FILE_HPP_
#define MDFS_NAMENODE_INODE_FILE_HPP_

#include "mdfs/namenode/inode.hpp"

namespace mdfs { namespace namenode {

class INodeFile : public INode {
private:
    uint32_t m_blockSize; // num of byte per block
    std::vector<common::Block> m_blocks;
    bool m_using; // is writing
public:
    INodeFile() = default;
    INodeFile(uint32_t blockSize):m_blockSize(blockSize), m_using(false) {}

    bool isUsing() { return m_using; }
    bool tryUsing() {
        if (m_using) return false;
        m_using = true;
        return true;
    }
    void release() { m_using = false; }
    void addBlock(common::Block blk);
    std::vector<common::Block> getBlocks();

    virtual void destroyAndCollectBlocks(std::vector<common::Block> & vec);
    virtual void collectBlocks(std::vector<common::Block> & vec);

    virtual std::string listSelf() const;
    virtual void destroySubTree();
};

} // namespace namenode
} // namespace mdfs


#endif // MDFS_NAMENODE_INODE_FILE_HPP_
