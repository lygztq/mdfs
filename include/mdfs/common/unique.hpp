#ifndef MDFS_COMMON_UNIQUE_HPP_
#define MDFS_COMMON_UNIQUE_HPP_

#include <cstdint>

namespace mdfs
{

template <typename IdType>
class UniqueInterface {
    virtual static IdType generateId() = 0;
    virtual IdType getId() const = 0;
    virtual void setId() = 0;

};

} // namespace mdfs


#endif // MDFS_COMMON_UNIQUE_HPP_
