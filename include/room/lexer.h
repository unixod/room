#ifndef ROOM_LEXER_H
#define ROOM_LEXER_H

#include <memory>
#include "room/source.h"

namespace room {

class Lexer {
public:
    Lexer(std::unique_ptr<Source>);
};

}

#endif // ROOM_LEXER_H
