#include "mdfs/namenode/inode_file.hpp"

namespace mdfs { namespace namenode {

void INodeFile::addBlock(common::Block blk) {
        if (blk.getNumBytes() > m_blockSize) {
            std::cerr << "Warning: wrong block size" << std::endl;
        }
        m_blocks.push_back(blk);
}

void INodeFile::destroyAndCollectBlocks(std::unordered_map<common::Block, size_t> & collect_blks) {
    collectBlocks(collect_blks);
}

void INodeFile::collectBlocks(std::unordered_map<common::Block, size_t> & collect_blks) const {
    for (const auto & blk : m_blocks) {
        if (collect_blks.find(blk) == collect_blks.end()) collect_blks[blk] = 1;
        else collect_blks[blk] += 1;
    }
}

std::string INodeFile::listSelf() const {
    return getName();
}

void INodeFile::destroySubTree() { return; }


} // namespace namenode    
} // namespace mdfs
