#ifndef ROOM_SOURCE_STRING_H
#define ROOM_SOURCE_STRING_H

#include <memory>
#include <string>
#include "room/source.h"

namespace room {
namespace utils {

class SourceString : public Source {
public:
    SourceString(std::string);

private:
    std::string _source;
};

} // namespace utils
} // namespace room

#endif // ROOM_SOURCE_STRING_H
