#include <catch.hpp>
#include <sstream>
#include "room/lexer.h"
#include "room/lst_parser.h"
#include "testing/facilities.h"

namespace tst = testing::lst;

TEST_CASE("Empty source") {
    auto pgm = tst::Program();

    std::istringstream src{std::get<tst::Source>(pgm)};
    auto lexer = room::Lexer{src};

    auto lst = room::lst::parse([&]{
        room::lexer::Token token;
        lexer >> token;
        return token;
    });

    REQUIRE(lst != nullptr);

    auto rootSet = dynamic_cast<room::ast::Set *>(lst.get());
    REQUIRE(rootSet != nullptr);
    REQUIRE(rootSet->elements.empty());
}
