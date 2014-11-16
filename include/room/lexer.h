#ifndef ROOM_LEXER_H
#define ROOM_LEXER_H

#include <memory>
#include "room/source.h"
#include "room/core/token.h"

namespace room {

class Lexer {
public:
    Lexer(std::unique_ptr<Source>);

    std::unique_ptr<core::Token> next();
};

}

#endif // ROOM_LEXER_H
