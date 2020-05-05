#ifndef MDFS_COMMON_ID_MANAGER_HPP_
#define MDFS_COMMON_ID_MANAGER_HPP_

#include <unordered_set>
#include <list>
#include <cstdint>
#include <type_traits>
#include <iostream>
#include <fstream>

#include "mdfs/common/types.hpp"

namespace mdfs { namespace common {

template <typename IdType>
IdType IdMax() {
    if (std::is_same<IdType, uint16_t>) return UINT16_MAX;
    else if (std::is_same<IdType, uint32_t>) return UINT32_MAX;
    else if (std::is_same<IdType, uint64_t>) return UINT64_MAX;
    else return UINT_MAX;
}
template uint64_t IdMax<uint64_t>();
template uint32_t IdMax<uint32_t>();
template uint16_t IdMax<uint16_t>();

// A manager class used to generate
// and manage id for objects.
template <typename IdType>
class IdManager {
private:
    IdType m_currentNumber;
    std::unordered_set<IdType> m_idset;
    std::list<IdType> m_idPool;

    void insertId(IdType id) {
        m_idPool.push_back(id);
        m_idset.insert(id);
    }
public:
    IdManager():m_currentNumber(0) {}
    IdManager(IdType size):m_currentNumber(size) {
        for (IdType i = 0; i < size; ++i) {
            insertId(i);
        }
    }

    IdType getId() {
        if (m_idPool.empty()) {
            if (m_currentNumber == IdMax<IdType>()) {
                std::cerr << "Warning: All possible ids have been allocated." << std::endl;
            }
            IdType id = m_currentNumber;
            ++m_currentNumber;
            return id;
        }
        else {
            IdType id = m_idPool.front();
            m_idPool.pop_front();
            m_idset.erase(id);
            return id;
        }
    }

    bool returnId(IdType id) {
        if (idInUse(id)) {
            insertId(id); return true;
        }
        return false;
    }

    bool idInUse(IdType id) {
        return m_idset.find(id) == m_idset.end() && id < m_currentNumber;
    }

    void clear() {
        m_currentNumber = 0;
        m_idset.clear();
        m_idPool.clear();
    }

    // ofs must be binary
    void store(std::ofstream & ofs) {
        IdType * buffer = new IdType[m_idPool.size() + 2];
        buffer[0] = m_currentNumber;
        buffer[1] = static_cast<IdType>(m_idPool.size());
        size_t i = 2;
        auto iter = m_idPool.begin();
        while (iter != m_idPool.end()) {
            buffer[i] = *iter; i++; iter++;
        }
        ofs.write((char *)buffer, (m_idPool.size() + 2) * sizeof(IdType));
        delete[] buffer;
    }

    void load(std::ifstream & ifs) {
        IdType buffer = new IdType[m_idPool.size() + 2];
        ifs.read((char *)buffer, (m_idPool.size() + 2) * sizeof(IdType));
        this->clear();
        m_currentNumber = buffer[0];
        size_t size = static_cast<size_t>(buffer[1]);
        for (size_t i = 2; i < size; ++i) {
            insertId(buffer[i]);
        }
        delete[] buffer;
    }
};

template class IdManager<uint64_t>;
template class IdManager<uint32_t>;
template class IdManager<uint16_t>;

} // namespace common
} // namespace mdfs


#endif // MDFS_COMMON_ID_MANAGER_HPP_
