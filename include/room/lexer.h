#ifndef ROOM_LEXER_H
#define ROOM_LEXER_H

#include <memory>
#include <iosfwd>
#include "room/core/token.h"

namespace room {

class Lexer {
public:
    Lexer(std::istream &);
    Lexer(Lexer &&);

    ~Lexer();

    Lexer & operator >> (lexer::Token &);
    operator bool() const;
    std::size_t currentOffset() const noexcept;

private:
    class Private;
    std::unique_ptr<Private> d_ptr;
};

}

#endif // ROOM_LEXER_H
