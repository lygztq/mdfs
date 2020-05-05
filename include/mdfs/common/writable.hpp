#ifndef MDFS_COMMON_WRITABLE_HPP_
#define MDFS_COMMON_WRITABLE_HPP_

#include <iostream>
#include <string>
#include <fstream>

namespace mdfs { namespace common {

// can (store to/load from) disk using fstream 
struct WritableInterface {
    virtual void store(std::ofstream & ofs) const = 0;
    virtual void load(std::ifstream & ifs) = 0;
    virtual ~WritableInterface() {}
};

// can (output to/input from) a stream
struct SerializableInterface : public WritableInterface
{
    virtual void fromStream(std::istream & is) = 0;
    virtual void toStream(std::ostream & os) const = 0;
    virtual ~SerializableInterface() {}
};

} // namespace common
} // namespace mdfs


#endif // MDFS_COMMON_WRITABLE_HPP_
