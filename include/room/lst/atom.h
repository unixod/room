#ifndef ROOM_LST_ATOM_H
#define ROOM_LST_ATOM_H

#include <string>
#include "room/lst/symbol.h"

namespace room {
namespace lst {

class Atom : public Symbol {
public:
    Atom(std::string _name, bool quoted)
        : Symbol(quoted), name{std::move(_name)}
    {}

    const std::string name;
};

} // namespace lst
} // namespace room

#endif // ROOM_LST_ATOM_H


