#ifndef MDFS_COMMON_LOOKUP_TABLE_HPP_
#define MDFS_COMMON_LOOKUP_TABLE_HPP_

#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>

#include "mdfs/common/writable.hpp"
#include "mdfs/common/types.hpp"

namespace mdfs { namespace common {

template <typename KeyType, typename ValueType>
class LookUpTable : public SerializableInterface {
private:
    std::unordered_map<KeyType, ValueType> m_map;

public:
    LookUpTable() = default;

    // look up function
    bool has(const KeyType & key) const { return m_map.find(key) != m_map.end(); }
    void set(const KeyType & key, const ValueType & value) { m_map[key] = value; }
    // insert a new (key, value) pair, ignore new value if key already exists.
    void insert(const KeyType & key, const ValueType & value) {
        if (!has(key)) m_map[key] = value;
    }
    ValueType & operator[](const KeyType & key) { return m_map[key]; }
    const ValueType & operator[](const KeyType & key) const { return m_map[key]; }

    void clear() { m_map.clear(); }

    // stream
    void fromStream(std::istream & is) {
        size_t size;
        is >> size;
        for (size_t i = 0; i < size; ++i) {
            KeyType key;
            ValueType value;
            is >> key >> value;
            m_map[key] = value;
        }
    }
    void toStream(std::ostream & os) const {
        os << m_map.size() << '\n';
        for (auto iter = m_map.begin(); iter != m_map.end(); ++iter) {
            os << iter->first << '\t' << iter->second << '\n';
        }
    }

    // // store
    // void storeTo(const std::string &path) const {
    //     std::ofstream outSteam;
    //     outSteam.open(path, std::ios::out);
    //     this->toStream(outSteam);
    //     outSteam.close();
    // }
    // void loadFrom(const std::string & path) {
    //     std::ifstream inStream;
    //     inStream.open(path, std::ios::in);
    //     this->fromStream(inStream);
    //     inStream.close();
    // }

    void store(std::ofstream & ofs) const {
        size_t length = m_map.size();
        KeyType * keyBuffer = new KeyType[length];
        ValueType * valueBuffer = new ValueType[length];
        int count = 0;
        for (const auto & item : m_map) {
            keyBuffer[count] = item.first;
            valueBuffer[count] = item.second;
            ++count;
        }
        ofs.write((char *)&length, sizeof(size_t));
        ofs.write((char *)keyBuffer, sizeof(KeyType) * length);
        ofs.write((char *)valueBuffer, sizeof(ValueType) * length);
        delete [] keyBuffer;
        delete [] valueBuffer;
    }

    void load(std::ifstream & ifs) {
        size_t length = 0;
        ifs.read((char *)&length, sizeof(size_t));
        this->clear();
        KeyType * keyBuffer = new KeyType[length];
        ValueType * valueBuffer = new ValueType[length];
        ifs.read((char *)keyBuffer, sizeof(KeyType) * length);
        ifs.read((char *)valueBuffer, sizeof(ValueType) * length);
        for (size_t i = 0; i<length; ++i)
            m_map[keyBuffer[i]] = valueBuffer[i];
        delete [] keyBuffer;
        delete [] valueBuffer;
    }
};

template <typename KeyType, typename ValueType>
std::ostream & operator<<(std::ostream & os, const LookUpTable<KeyType, ValueType> & obj) {
    obj.toStream(os);
    return os;
}

template <typename KeyType, typename ValueType>
std::istream & operator>>(std::istream & is, LookUpTable<KeyType, ValueType> & obj) {
    obj.fromStream(is);
    return is;
}

template class LookUpTable<std::string, ClientId>;

} // namespace common
} // namespace mdfs


#endif //MDFS_COMMON_LOOKUP_TABLE_HPP_
