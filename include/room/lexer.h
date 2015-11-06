#ifndef ROOM_LEXER_H
#define ROOM_LEXER_H

#include <memory>
#include <iterator>
#include <iosfwd>
#include "room/lexer/token.h"

namespace room {

class Lexer {
public:
    Lexer(std::istream &);
    Lexer(Lexer &&);

    ~Lexer();

    lexer::Token nextToken();
    std::size_t currentOffset() const noexcept;

private:
    class Private;
    std::unique_ptr<Private> _impl;
};

}

#endif // ROOM_LEXER_H
