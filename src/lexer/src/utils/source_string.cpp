#include "room/utils/source_string.h"

namespace utils = room::utils;

utils::SourceString::SourceString(std::string source) :
    _source(std::move(source))
{}
