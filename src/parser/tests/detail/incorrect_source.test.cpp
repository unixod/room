#include <catch.hpp>
#include <sstream>
#include "room/lst/parser.h"
#include "testing/facilities.h"

namespace test = room::testing;
namespace lex = room::lexer;

TEST_CASE("Error token") {
    auto pgm = test::Program(test::Atom("a"),
                             test::Error("some error"),
                             test::Atom("b"));

    auto token = pgm.tokens().begin();
    auto end = pgm.tokens().end();


    REQUIRE_THROWS_AS(
        room::lst::parse([token, end]() mutable {
            return token != end ? *token++
                                : lex::Token{lex::Token::Category::End, "<end>"};
        }),
        std::domain_error
    );
}

TEST_CASE("Unexpected SpaceEnd token") {
    auto pgm = test::Program(test::Atom("a"),
                             test::Custom(lex::Token{lex::Token::Category::SpaceEnd, "}"}),
                             test::Atom("b"));

    auto token = pgm.tokens().begin();
    auto end = pgm.tokens().end();


    REQUIRE_THROWS_AS(
        room::lst::parse([token, end]() mutable {
            return token != end ? *token++
                                : lex::Token{lex::Token::Category::End, "<end>"};
        }),
        std::domain_error
    );
}

TEST_CASE("Unexpected end of space") {
    auto pgm = test::Program(test::Atom("a"),
                             test::Custom(lex::Token{lex::Token::Category::SpaceBegin, "{"}),
                             test::Atom("b"));

    auto token = pgm.tokens().begin();
    auto end = pgm.tokens().end();


    REQUIRE_THROWS_AS(
        room::lst::parse([token, end]() mutable {
            return token != end ? *token++
                                : lex::Token{lex::Token::Category::End, "<end>"};
        }),
        std::domain_error
    );
}