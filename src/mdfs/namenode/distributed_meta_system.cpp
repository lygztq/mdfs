#include <unordered_set>
#include <algorithm>

#include "mdfs/namenode/distributed_meta_system.hpp"

namespace mdfs { namespace namenode {

using StatusCode = common::OperationStatusCode;
using Block = common::Block;
using BlockInfo = message::BlockInfo;

void DistributeMetaSystem::sampleAllDataNode(size_t numNodes, std::vector<std::string> & retNodes) const {
    std::vector<std::string> validNode;

    // get all live data nodes
    for (const auto & node : m_datanodeList) {
        if (std::time(0) - node.second <= common::MAX_HEARTBEAT_TOLERANCE)
            validNode.push_back(node.first);
    }

    // Knuth-Durstenfeld shuffle
    size_t validSize = validNode.size();
    numNodes = (numNodes > validSize) ? validSize : numNodes;
    if (numNodes == validSize) retNodes = validNode;
    bool inverse = false;
    if (numNodes > validSize / 2) {
        inverse = true;
        numNodes = validSize - numNodes;
    }
    for (size_t i=0; i < numNodes; ++i) {
        int idx = std::rand() % (validSize - i); // sample random number in [0, validSize-i]
        std::swap(validNode[idx], validNode[validSize-i-1]);
    }
    if (inverse) retNodes = std::vector<std::string>(validNode.begin(), validNode.begin() + validSize - numNodes);
    else retNodes = std::vector<std::string>(validNode.begin() + validSize - numNodes, validNode.end());
}

void DistributeMetaSystem::sampleDataNode(const std::vector<std::string> & nodeList, size_t numNodes, std::vector<std::string> & retNodes) const {
    std::vector<std::string> validNode;
    
    // get all live data nodes
    for (const auto & node : nodeList) {
        if (std::time(0) - m_datanodeList.at(node) <= common::MAX_HEARTBEAT_TOLERANCE)
            validNode.push_back(node);
    }

    // Knuth-Durstenfeld shuffle
    size_t validSize = validNode.size();
    numNodes = (numNodes > validSize) ? validSize : numNodes;
    if (numNodes == validSize) retNodes = validNode;
    bool inverse = false;
    if (numNodes > validSize / 2) {
        inverse = true;
        numNodes = validSize - numNodes;
    }
    for (size_t i=0; i < numNodes; ++i) {
        int idx = std::rand() % (validSize - i); // sample random number in [0, validSize-i]
        std::swap(validNode[idx], validNode[validSize-i-1]);
    }
    if (inverse) retNodes = std::vector<std::string>(validNode.begin(), validNode.begin() + validSize - numNodes);
    else retNodes = std::vector<std::string>(validNode.begin() + validSize - numNodes, validNode.end());
}

bool DistributeMetaSystem::DistributeBlocks(const std::vector<Block> & blks, std::vector<BlockInfo> & infos, bool read=false) const {
    for (const auto & blk : blks) {
        infos.emplace_back(BlockInfo());
        auto info = infos.back();
        info.set_id(blk.getId());
        info.set_numbytes(blk.getNumBytes());
        info.set_stamp(blk.getStamp());
        info.set_blocksize(m_blockSize);
        
        if (read) {
            if (!m_blkDataNodeMap.has(blk.getId())) return false;
            auto targetDataNodes = m_blkDataNodeMap[blk.getId()];
            for (const auto & node : targetDataNodes) {
                if (std::time(0) - m_datanodeList.at(node) < common::MAX_HEARTBEAT_TOLERANCE)
                    info.add_datanode(node);
            }
        } else {
            std::vector<std::string> sampleRes;
            sampleAllDataNode(1, sampleRes);
            info.add_datanode(sampleRes.front());
        }
    }
    return true;
}

StatusCode DistributeMetaSystem::read(const std::string & path, std::vector<BlockInfo> & blockinfos) const {
    std::vector<Block> blks;
    StatusCode code = m_fileTree.read(path, blks);
    if (code != StatusCode::SUCCESS) return code;

    if (!DistributeBlocks(blks, blockinfos)) return StatusCode::FAILED;
    return StatusCode::SUCCESS;
}

StatusCode DistributeMetaSystem::write(const std::string & path, BlockInfo & blockInfo) {
    Block blk;
    size_t remainSize;
    StatusCode code = m_fileTree.write(path, blk, remainSize);
    if (code != StatusCode::SUCCESS) return code;
    blockInfo.set_id(blk.getId());
    blockInfo.set_stamp(blk.getStamp());
    blockInfo.set_numbytes(blk.getNumBytes());
    blockInfo.set_blocksize(m_blockSize);
    std::vector<std::string> dataNodes;
    sampleAllDataNode(1, dataNodes);
    blockInfo.add_datanode(dataNodes.front());
    return StatusCode::SUCCESS;
}

StatusCode DistributeMetaSystem::create(const std::string & path) {
    return m_fileTree.create(path);
}

StatusCode DistributeMetaSystem::makedir(const std::string & path) {
    return m_fileTree.makedir(path);
}

StatusCode DistributeMetaSystem::remove(const std::string & path, std::vector<mdfs::message::BlockInfo> & blockInfos) {
    std::vector<Block> removedBlks;
    auto code = m_fileTree.remove(path, removedBlks);
    if (code != StatusCode::SUCCESS) return code;

    if(!DistributeBlocks(removedBlks, blockInfos)) code = StatusCode::WARNING;

    for (const auto & blk : removedBlks) {
        m_blkDataNodeMap.erase(blk.getId());
    }
    return code;
}

StatusCode DistributeMetaSystem::move(const std::string & src, const std::string & des) {
    return m_fileTree.move(src, des);
}

StatusCode DistributeMetaSystem::copy(const std::string & src, const std::string & des) {
    return m_fileTree.copy(src, des);
}

StatusCode DistributeMetaSystem::addBlock(const std::string & path, mdfs::message::BlockInfo & info) {
    Block blk;
    auto code = m_fileTree.addBlock(path, blk);
    if (code != StatusCode::SUCCESS) return code;

    std::vector<BlockInfo> infos;
    if (DistributeBlocks({blk}, infos)) code = StatusCode::WARNING;
    info = infos.front();
    return code;
}

StatusCode DistributeMetaSystem::rename(const std::string & src, const std::string & newName) {
    return m_fileTree.rename(src, newName);
}

StatusCode DistributeMetaSystem::blockAck(const std::string & path, common::BlockId bid, std::string datanode, bool duplicated, uint64_t ackSize) {
    StatusCode code = StatusCode::UNKNOWN;
    if (!duplicated) {
        code = m_fileTree.blockAck(path, bid, ackSize);
        if (code != StatusCode::SUCCESS) return code;
    }

    auto & nodelist = m_blkDataNodeMap[bid];
    if (std::find(nodelist.begin(), nodelist.end(), datanode) == nodelist.end()) nodelist.push_back(datanode);
    return StatusCode::SUCCESS;
}

void DistributeMetaSystem::addDataNode(std::string datanode) {
    m_datanodeList[datanode] = std::time(0);
}

void DistributeMetaSystem::checkDataNode(std::vector<mdfs::message::BlockInfo> & blockInfos) {
    // NOT YET IMPLEMENT
    return;
}

void DistributeMetaSystem::RecvHeartBeat(std::string datanode) {
    m_datanodeList[datanode] = std::time(0);
}

} // namespace namenode
} // namespace mdfs
