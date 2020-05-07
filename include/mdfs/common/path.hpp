#ifndef MDFS_COMMON_PATH_HPP_
#define MDFS_COMMON_PATH_HPP_

#include <vector>
#include <string>

namespace mdfs { namespace common {

const std::string CURRENT_RELATIVE_PATH = ".";
const std::string PARENT_RELATIVE_PATH = "..";
const std::string ROOT_PATH = "";
const std::string DEFAULT_PATH_DELIMITER = "/";

// path process class
// Note: can only process absolute path
struct Path {
private:
    std::string m_fullPath;
    std::vector<std::string> m_pathList;
    size_t m_currentLayer;
    std::string m_delimiters;

public:
    Path() = default;
    Path(const std::string &p):m_fullPath(p), m_currentLayer(0), m_delimiters("/") { process(p); }

    void setDelimiters(const std::string &d) { m_delimiters = d; }
    std::string current() const { return m_pathList[m_currentLayer]; }
    std::string fullPath() const { return m_fullPath; }
    void process(const std::string &p);
    bool nextLayer() { 
        if (m_currentLayer < m_pathList.size() - 1) {
            m_currentLayer += 1;
            return true;
        }
        return false;
    }
    bool prevLayer() { 
        if (m_currentLayer > 1) {
            m_currentLayer -= 1;
            return true;
        }
        return false;
    }
    bool inLeaf() const { return m_currentLayer < m_pathList.size() - 1; }
};

} // namespace common
} // namespace mdfs


#endif // MDFS_COMMON_PATH_HPP_
