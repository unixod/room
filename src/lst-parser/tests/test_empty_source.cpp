#include <catch.hpp>
#include <sstream>
#include "room/lst_parser.h"
#include "testing/facilities.h"

namespace test = room::testing::lst;
namespace lex = room::lexer;

TEST_CASE("Empty source") {
    auto pgm = test::Program();

    auto token = pgm.tokens().begin();
    auto end = pgm.tokens().end();

    auto lst = room::lst::parse([token, end]() mutable {
        return token != end ? *token++
                            : lex::Token{lex::token::Class::End, "<end>"};
    });

    REQUIRE(lst != nullptr);

    auto rootSet = dynamic_cast<room::ast::Set *>(lst.get());
    REQUIRE(rootSet != nullptr);
    REQUIRE(rootSet->elements.empty());

    REQUIRE(pgm.hasSame(lst));
}
