#ifndef ROOM_LEXER_H
#define ROOM_LEXER_H

#include <memory>
#include "room/source.h"
#include "room/core/token.h"

namespace room {

class Lexer {
public:
    Lexer(std::unique_ptr<Source>);

    /**
     * @brief hasNext check the token
     * @return true - if the lexer has another token in its input, else false.
     *
     * This method may block why waiting for input to tokenize.
     */
    bool hasNext();

    /**
     * @brief next returns the complete token from the lexer
     * @return token
     */
    core::Token next();
};

}

#endif // ROOM_LEXER_H
