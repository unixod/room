#ifndef ROOM_PARSER_DETAIL_ALPHA_PARSER_H
#define ROOM_PARSER_DETAIL_ALPHA_PARSER_H

#include <functional>
#include "room/lexer/token.h"

namespace room {
namespace parser {
namespace detail {

class AtomTree;

AtomTree alpha_parse(std::function<lexer::Token()> generator);

} // namespace detail
} // namespace parser
} // namespace room

#endif // ROOM_PARSER_DETAIL_ALPHA_PARSER_H
