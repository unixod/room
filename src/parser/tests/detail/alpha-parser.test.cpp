#include <catch.hpp>
#include "testing/facilities.h"
#include "detail/alpha-parser.h"
#include "detail/atom-set.h"

using room::parser::detail::makeAtomSet;
using room::parser::detail::AtomSet;
using room::lexer::Token;

auto tokenize(const char *)
{
    return []{return Token{Token::Category::End, "none"};};
}

std::string stringify(std::unique_ptr<AtomSet> std)
{
    return "none";
}

TEST_CASE("processing valid sequence of tokens")
{
    auto references = {
        "",                         // empty token set
        "a b c",                    // only atoms
        "() () (() () ( () ) )",    // only sets
        "a (b c) d",                // sets and atoms
    };

    for (auto &&ref : references) {
        REQUIRE(stringify(makeAtomSet(tokenize(ref))) == ref);
    }
}
