#include <cstdint>
#include <string>

class Block {
public:
    Block() = default;
    Block(uint64_t id, uint64_t numBytes, uint64_t stamp)
    : m_bid(id), m_numBytes(numBytes), m_stamp(stamp) {}
    Block(int id, int numBytes, int stamp)
    : m_bid(id), m_numBytes(numBytes), m_stamp(stamp) {}

    uint64_t getId() { return m_bid; }
    void setId(uint64_t id) { m_bid = id; }

    uint64_t getNumBytes() { return m_numBytes; }
    void setNumBytes(uint64_t num) { m_numBytes = num; }

    uint64_t getStamp() { return m_stamp; }
    void setStamp(uint64_t stamp) { m_stamp = stamp; }
private:
    uint64_t m_bid;
    uint64_t m_numBytes;
    uint64_t m_stamp;
public:
    const static std::string BLOCK_FILE_PREFIX;
    const static std::string METADATA_EXTENSION;
};
