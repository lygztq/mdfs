#include "mdfs/namenode/inode_file.hpp"

namespace mdfs { namespace namenode {

void INodeFile::addBlock(common::Block blk) {
    while (tryUsing()) {
        if (blk.getNumBytes() > m_blockSize) {
            std::cerr << "Warning: wrong block size" << std::endl;
        }
        m_blocks.push_back(blk);
    }
    release();
}

std::vector<common::Block> INodeFile::getBlocks() {
    std::vector<common::Block> blks;
    while (tryUsing()) {
        blks = m_blocks;
    }
    release();
    return blks;
}

void INodeFile::destroyAndCollectBlocks(std::vector<common::Block> & vec) {
    while (tryUsing()) {
        for (const auto & blk : m_blocks)
            vec.push_back(blk);
    }
    release();
}

void INodeFile::collectBlocks(std::vector<common::Block> & vec) {
    destroyAndCollectBlocks(vec);
}

std::string INodeFile::listSelf() const {
    return getName();
}

void INodeFile::destroySubTree() { return; }


} // namespace namenode    
} // namespace mdfs
