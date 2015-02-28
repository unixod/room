#include <catch.hpp>
#include <sstream>
#include "room/lst_parser.h"
#include "testing/facilities.h"

namespace tst = testing::lst;

TEST_CASE("Empty source") {
    auto pgm = tst::Program();
    auto token = std::get<tst::Source>(pgm).begin();
    auto end = std::get<tst::Source>(pgm).end();

    auto lst = room::lst::parse([token, end]() mutable {
        return token != end ? *token++ : room::lexer::Token{room::lexer::token::Class::End, "<end>"};
    });

    REQUIRE(lst != nullptr);

    auto rootSet = dynamic_cast<room::ast::Set *>(lst.get());
    REQUIRE(rootSet != nullptr);
    REQUIRE(rootSet->elements.empty());
}
