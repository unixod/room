#include <catch.hpp>
#include <sstream>
#include "room/lexer.h"
#include "room/lst_parser.h"
#include "room/ast/lst/set.h"

TEST_CASE("Empty source") {
    std::istringstream empty;
    auto lexer = room::Lexer{empty};

    auto lst = room::lst::parse([&]{
        room::lexer::Token token;
        lexer >> token;
        return token;
    });


    REQUIRE(lst != nullptr);

    auto rootSet = dynamic_cast<room::ast::Set *>(lst.get());
    REQUIRE(rootSet != nullptr);
    REQUIRE(rootSet->elements.empty());

    REQUIRE(false);
}
