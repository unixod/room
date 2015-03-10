#ifndef ROOM_SST_PARSER_H
#define ROOM_SST_PARSER_H

#include <memory>
#include "room/lst/symbol.h"
#include "room/ast/space.h"
#include "room/ast/subject.h"
#include "room/ast/perspective.h"

namespace room {
namespace sst {

std::unique_ptr<ast::Space> parseSpace(std::unique_ptr<lst::Symbol>);
std::unique_ptr<ast::Subject>  parseSubject(std::unique_ptr<lst::Symbol>);
std::unique_ptr<ast::Perspective> parsePerspective(std::unique_ptr<lst::Symbol>);

} // namespace lst
} // namespace room

#endif // ROOM_SST_PARSER_H
