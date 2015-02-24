#ifndef ROOM_LST_PARSER_H
#define ROOM_LST_PARSER_H

#include <memory>
#include <functional>
#include "room/lexer/token.h"
#include "room/ast/symbol.h"

namespace room {
namespace lst {

std::unique_ptr<ast::Symbol> parse(std::function<lexer::Token()> generator);

} // namespace lst
} // namespace room

#endif // ROOM_LST_PARSER_H
