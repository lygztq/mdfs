#ifndef MDFS_NAMENODE_CLIENT_LIST_HPP_
#define MDFS_NAMENODE_CLIENT_LIST_HPP_

#include <cstdint>
#include <vector>
#include <string>

#include "mdfs/common/lookup_table.hpp"
#include "mdfs/common/types.hpp"
#include "mdfs/common/constants.hpp"
#include "mdfs/common/writable.hpp"

namespace mdfs { namespace namenode {

using ClientNameToId = common::LookUpTable<std::string, common::ClientId>;

class ClientList : public common::SerializableInterface {
private:
    std::vector<std::string> m_names; // vector of IP address
    ClientNameToId m_nameToId;

public:
    common::ClientId addClient(const std::string & name) {
        common::ClientId cid = static_cast<common::ClientId>(m_names.size());
        if (cid > common::MAX_CLIENT_ID) {
            std::cerr << "Warning: Total client number has exceed limit." << std::endl
                      << "This addClient operation (username: " << name << ") will be ignored." << std::endl;
            return common::NULL_CLIENT_ID;
        }
        m_names.push_back(name);
        m_nameToId.insert(name, cid);
        return cid;
    }

    common::ClientId getClientId(const std::string & name) const {
        if (m_nameToId.has(name)) return m_nameToId[name];
        else return common::NULL_CLIENT_ID;
    }

    std::string getClientName(common::ClientId cid) const {
        if (!common::validClientId(cid) || cid >= length()) {
            std::cerr << "Invalid client Id or this client id does not exist." << std::endl;
            return nullptr;
        }
        return m_names[static_cast<size_t>(cid)];
    }

    // stream
    void fromStream(std::istream & is) {
        size_t size;
        is >> size;
        for (size_t i = 0; i < size; ++i) {
            std::string name;
            is >> name;
            m_names[i] = name;
            m_nameToId.insert(name, i);
        }
    }

    void toStream(std::ostream & os) const {
        os << m_names.size() << '\n';
        for (const auto & name : m_names) {
            os << name << '\n';
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

    void store(std::ofstream & ofs) const {}

    void load(std::ifstream & ifs) {}

    size_t length() const { return m_names.size(); }
};

} // namespace namenode
} // namespace mdfs


#endif // MDFS_NAMENODE_CLIENT_LIST_HPP_
