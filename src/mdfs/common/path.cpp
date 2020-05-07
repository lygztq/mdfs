#include "mdfs/common/path.hpp"

using string = std::string;

namespace mdfs { namespace common {

static std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ") {
    str.erase(0, str.find_first_not_of(chars));
    return str;
}
 
static std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ") {
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}
 
static std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ") {
    return ltrim(rtrim(str, chars), chars);
}

static void split(const string & s, std::vector<string> & tokens, const string & delimiters=" ") {
    string::size_type lastPos = s.find_first_not_of(delimiters, 0);
    string::size_type pos = s.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos) {
        tokens.emplace_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, lastPos);
    }
}

void Path::process(const std::string &p) {
    this->m_pathList.clear();
    
    // determine default delimiters
    if (this->m_delimiters.empty()) m_delimiters = DEFAULT_PATH_DELIMITER;
    
    // trim
    m_fullPath = p; trim(m_fullPath);

    if (!m_fullPath.empty()) {
        // if absolute path
        if (m_fullPath.find_first_of(m_delimiters, 0) == 0)
            m_pathList.emplace_back("");
        
        split(m_fullPath, this->m_pathList, m_delimiters);
    }
}

} // namespace common
} // namespace mdfs