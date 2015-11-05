#include <catch.hpp>
#include <sstream>
#include "room/lst/parser.h"
#include "testing/facilities.h"

namespace test = room::testing;
namespace lex = room::lexer;

TEST_CASE("Empty source") {
    auto pgm = test::Program();

    auto token = pgm.tokens().begin();
    auto end = pgm.tokens().end();

    auto lst = room::lst::parse([token, end]() mutable {
        return token != end ? *token++
                            : lex::Token{lex::Token::Category::End, "<end>"};
    });

    REQUIRE(lst != nullptr);

    auto rootSet = dynamic_cast<room::lst::Set *>(lst.get());
    REQUIRE(rootSet != nullptr);
    REQUIRE(rootSet->elements.empty());

    REQUIRE(pgm.hasSame(lst));
}
