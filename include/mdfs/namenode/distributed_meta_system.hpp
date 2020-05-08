#ifndef MDFS_NAMENODE_DISTRIBUTED_META_SYSTEM_HPP
#define MDFS_NAMENODE_DISTRIBUTED_META_SYSTEM_HPP

#include <unordered_map>
#include <ctime>

#include "mdfs/namenode/filesys_tree.hpp"
#include "mdfs/common/constants.hpp"
#include "mdfs/protos/block_info.pb.h"

namespace mdfs { namespace namenode {

using BlockDataNodeTable = common::LookUpTable<common::BlockId, std::vector<std::string>>;

class DistributeMetaSystem {
private:
    FileSystemTree m_fileTree;
    std::unordered_map<std::string, time_t> m_datanodeList; // datanode name and last heart beat time.
    BlockDataNodeTable m_blkDataNodeMap;
    uint64_t m_blockSize;
public:
    DistributeMetaSystem() = default;
    DistributeMetaSystem(const std::vector<std::string> & datanodes) {
        m_blockSize = common::BLOCK_SIZE;
        for (const auto & node : datanodes)
            m_datanodeList[node] = common::INIT_HEARTBEAT_TIME;
    }

    // copy is not allowed
    DistributeMetaSystem(const DistributeMetaSystem &) = delete;
    DistributeMetaSystem(const DistributeMetaSystem &&) = delete;

    // file system operation
    common::OperationStatusCode read(const std::string & path, std::vector<mdfs::message::BlockInfo> & blocks);
    common::OperationStatusCode write(const std::string & path, mdfs::message::BlockInfo & return_blk);
    common::OperationStatusCode create(const std::string & path);
    common::OperationStatusCode makedir(const std::string & path);
    common::OperationStatusCode remove(const std::string & path, std::vector<mdfs::message::BlockInfo> & blks_to_remove);
    common::OperationStatusCode move(const std::string & src, const std::string & des);
    common::OperationStatusCode copy(const std::string & src, const std::string & des);
    common::OperationStatusCode addBlock(const std::string & path, mdfs::message::BlockInfo & return_blk);
    common::OperationStatusCode rename(const std::string & src, const std::string & newName);
    common::OperationStatusCode blockAck(const std::string & path, common::BlockId bid, std::string datanode, bool duplicated, uint64_t ackSize);

    // name node management
    void addDataNode();
    void checkDataNode();
    void RecvHeartBeat(std::string datanode);
};

} // namespace namenode
} // namespace mdfs


#endif // MDFS_NAMENODE_DISTRIBUTED_META_SYSTEM_HPP
