#ifndef ROOM_CORE_TOKENS_ATOM_H
#define ROOM_CORE_TOKENS_ATOM_H

#include <string>
#include "room/core/token.h"

namespace room {
namespace core {
namespace tokens {

class Atom : public Token {
public:
    Atom(std::string){}
};

} // namespace tokens
} // namespace core
} // namespace room

#endif // ROOM_CORE_TOKENS_ATOM_H
