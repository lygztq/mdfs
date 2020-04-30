#ifndef MDFS_COMMON_BLOCK_HPP_
#define MDFS_COMMON_BLOCK_HPP_

#include <cstdint>
#include <string>

#include "mdfs/common/types.hpp"
#include "mdfs/common/id_manager.hpp"

namespace mdfs { namespace common {

using BlockIdManager = IdManager<uint64_t>;

class Block {
public:
    Block() = default;
    Block(BlockId id, uint64_t numBytes, uint64_t stamp)
    : m_bid(id), m_numBytes(numBytes), m_stamp(stamp) {}
    Block(int id, int numBytes, int stamp)
    : m_bid(id), m_numBytes(numBytes), m_stamp(stamp) {}

    BlockId getId() { return m_bid; }
    void setId(BlockId id) { m_bid = id; }

    uint64_t getNumBytes() { return m_numBytes; }
    void setNumBytes(uint64_t num) { m_numBytes = num; }

    uint64_t getStamp() { return m_stamp; }
    void setStamp(uint64_t stamp) { m_stamp = stamp; }

private:
    BlockId m_bid;
    uint64_t m_numBytes;
    uint64_t m_stamp;

public:
    const static std::string BLOCK_FILE_PREFIX;
    const static std::string METADATA_EXTENSION;
    static BlockIdManager m_idManager;
};

} // namespace common
} // namespace mdfs

#endif // MDFS_COMMON_BLOCK_HPP_
