#ifndef MDFS_COMMON_WRITABLE_HPP_
#define MDFS_COMMON_WRITABLE_HPP_

#include <iostream>
#include <string>

namespace mdfs { namespace common {

struct WritableInterface
{
    virtual void fromStream(std::istream & is) = 0;
    virtual void toStream(std::ostream & os) const = 0;
    virtual void storeTo(const std::string & path) const = 0;
    virtual void loadFrom(const std::string & path) = 0;
    virtual ~WritableInterface() {}
};

} // namespace common
} // namespace mdfs


#endif // MDFS_COMMON_WRITABLE_HPP_
