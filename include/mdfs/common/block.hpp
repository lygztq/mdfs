#ifndef MDFS_COMMON_BLOCK_HPP_
#define MDFS_COMMON_BLOCK_HPP_

#include <cstdint>
#include <string>

#include "mdfs/common/types.hpp"
#include "mdfs/common/id_manager.hpp"

namespace mdfs { namespace common {

using BlockIdManager = IdManager<uint64_t>;

// Block class used to describe a block
class Block {
public:
    Block() = default;
    Block(BlockId id, uint64_t numBytes, uint64_t stamp)
    : m_bid(id), m_numBytes(numBytes), m_stamp(stamp) {}
    Block(int id, int numBytes, int stamp)
    : m_bid(id), m_numBytes(numBytes), m_stamp(stamp) {}

    BlockId getId() const { return m_bid; }
    void setId(BlockId id) { m_bid = id; }

    uint64_t getNumBytes() const { return m_numBytes; }
    void setNumBytes(uint64_t num) { m_numBytes = num; }

    uint64_t getStamp() const { return m_stamp; }
    void setStamp(uint64_t stamp) { m_stamp = stamp; }

    // only compare Block id
    bool operator== (const Block & b) const {
        return b.m_bid == this->m_bid;
    }

private:
    BlockId m_bid;
    uint64_t m_numBytes;
    uint64_t m_stamp;

public:
    const static std::string BLOCK_FILE_PREFIX;
    const static std::string METADATA_EXTENSION;
    static BlockIdManager m_idManager;
};

inline bool sameBlock(const Block & b1, const Block & b2) {
    return b1 == b2 && b1.getId() == b2.getId() && b1.getNumBytes() == b2.getNumBytes();
}

} // namespace common
} // namespace mdfs

namespace std {

template <>
struct hash<mdfs::common::Block> {
    size_t operator() (const mdfs::common::Block & b) const noexcept {
        return hash<mdfs::common::BlockId>()(b.getId());
    }
};

} // namespace std

#endif // MDFS_COMMON_BLOCK_HPP_
