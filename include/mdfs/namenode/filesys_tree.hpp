#ifndef MDFS_NAMENODE_FILESYS_TREE_HPP_
#define MDFS_NAMENODE_FILESYS_TREE_HPP_

#include "mdfs/namenode/inode.hpp"
#include "mdfs/namenode/inode_directory.hpp"
#include "mdfs/namenode/inode_file.hpp"
#include "mdfs/common/block.hpp"

namespace mdfs { namespace namenode {

class FileSystemTree {
private:
    INode * m_root;
public:
    FileSystemTree() { m_root = new INodeDirectory(); }
    ~FileSystemTree() { delete m_root; }

    std::vector<common::Block> read(const std::string & path) const;
    common::Block write(const std::string & path);
    common::Block create(const std::string & path);
    void makedir(const std::string & path);
    void remove(const std::string & path);
    common::Block addBlock(const std::string & path);
};

} // namespace namenode
} // namespace mdfs


#endif // MDFS_NAMENODE_FILESYS_TREE_HPP_
