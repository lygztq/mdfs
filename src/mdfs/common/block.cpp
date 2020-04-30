#include "mdfs/common/block.hpp"

namespace mdfs { namespace common {
BlockIdManager Block::m_idManager = BlockIdManager();
const std::string Block::BLOCK_FILE_PREFIX = "blk";
const std::string Block::METADATA_EXTENSION = ".meta";


} // namespace common    
} // namespace mdfs

