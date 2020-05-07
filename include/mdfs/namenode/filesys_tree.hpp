#ifndef MDFS_NAMENODE_FILESYS_TREE_HPP_
#define MDFS_NAMENODE_FILESYS_TREE_HPP_

#include "mdfs/namenode/inode.hpp"
#include "mdfs/namenode/inode_directory.hpp"
#include "mdfs/namenode/inode_file.hpp"
#include "mdfs/common/block.hpp"
#include "mdfs/common/types.hpp"
#include "mdfs/common/lookup_table.hpp"

namespace mdfs { namespace namenode {

using BlockCount = common::LookUpTable<common::BlockId, size_t>;

class FileSystemTree {
private:
    INode * m_root;     // root inode
    BlockCount m_count; // block reference count
public:
    FileSystemTree() { m_root = new INodeDirectory(); }
    ~FileSystemTree() { delete m_root; }

    common::OperationStatusCode read(const std::string & path, std::vector<common::Block> & blk_list) const;
    common::OperationStatusCode write(const std::string & path, common::Block & return_blk, uint64_t & remainSize);
    common::OperationStatusCode create(const std::string & path);
    common::OperationStatusCode makedir(const std::string & path);
    common::OperationStatusCode remove(const std::string & path, std::vector<common::Block> & blks_to_remove);
    common::OperationStatusCode move(const std::string & src, const std::string & des);
    common::OperationStatusCode copy(const std::string & src, const std::string & des);
    common::OperationStatusCode addBlock(const std::string & path, common::Block & return_blk);
    common::OperationStatusCode rename(const std::string & src, const std::string & newName);
    common::OperationStatusCode blockAck(const std::string & src, common::BlockId bid, uint64_t ackSize);

private:
    INode * getNodeFromPath(const std::string & path) const;
    INode * getParentFromPath(const std::string & path) const;
};

} // namespace namenode
} // namespace mdfs


#endif // MDFS_NAMENODE_FILESYS_TREE_HPP_
