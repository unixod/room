#ifndef ROOM_PARSER_DETAIL_ALPHA_PARSER_H
#define ROOM_PARSER_DETAIL_ALPHA_PARSER_H

#include <functional>
#include <memory>
#include "room/lexer/token.h"

namespace room {
namespace parser {
namespace detail {

class AtomSet;

std::unique_ptr<AtomSet> makeAtomSet(std::function<lexer::Token()> generator);

} // namespace detail
} // namespace parser
} // namespace room

#endif // ROOM_PARSER_DETAIL_ALPHA_PARSER_H
