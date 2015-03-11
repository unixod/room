#ifndef ROOM_SST_PARSER_H
#define ROOM_SST_PARSER_H

#include <memory>
#include "room/lst/symbol.h"
#include "room/sst/space.h"
#include "room/sst/subject.h"
#include "room/sst/perspective.h"

namespace room {
namespace sst {

std::unique_ptr<sst::Space> parseSpace(std::unique_ptr<lst::Symbol>);
std::unique_ptr<sst::Subject>  parseSubject(std::unique_ptr<lst::Symbol>);
std::unique_ptr<sst::Perspective> parsePerspective(std::unique_ptr<lst::Symbol>);

} // namespace lst
} // namespace room

#endif // ROOM_SST_PARSER_H
