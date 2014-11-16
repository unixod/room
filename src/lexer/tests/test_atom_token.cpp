#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <sstream>
#include <iterator>
#include "room/lexer.h"
#include "room/utils/source_string.h"
#include "room/utils/make_unique.h"
#include "room/core/tokens/atom.h"
#include "room/core/tokens/end.h"

namespace core = room::core;
namespace utils = room::utils;

/// Helpers
room::Lexer tokenize(std::string programm) {
    return room::Lexer{
        std::make_unique<utils::SourceString>(
            programm
        )
    };
}

template<class T>
bool is(const std::unique_ptr<room::core::Token> &t) {
    return dynamic_cast<typename
            std::add_const<T>::type *>(t.get());
}

TEST_CASE("Atoms tokenization") {
    // atoms lexemes variations
    const auto lexemes = std::vector<std::string>{
        "a", "abc", "1a", "1abc"
        "a2", "a2bc", "1+2", "+",
    };

    // concatenate ones into one string (room programm)
    std::ostringstream pgm;
    std::copy(lexemes.begin(),
              lexemes.end(),
              std::ostream_iterator<std::string>(pgm, " ")); // in this test we use blank space as delimiter of atoms

    auto lexer = tokenize(pgm.str());

    for (auto &lexeme : lexemes) {
        auto token = lexer.next();
        CAPTURE(lexeme);
        REQUIRE(is<core::tokens::Atom>(token));
    }

    // End token verification
    REQUIRE(is<core::tokens::End>(lexer.next()));
}
