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

std::string unescape(std::string lexeme) {

    if ((lexeme.size() > 2) &&          // is multi-escaped
            (lexeme.front() == '|') &&
            (lexeme.back() == '|')) {
        lexeme.erase(lexeme.size() - 1).erase(0, 1);
    } else {
        auto trash = std::remove(lexeme.begin(), lexeme.end(), '\\');
        lexeme.erase(trash, lexeme.end());
    }

    return lexeme;
}

TEST_CASE("Atoms tokenization") {
    // atom lexemes variations
    const auto lexemes = std::vector<std::string>{
        //
        // simple atoms
        //
        "a", "abc", "1a", "1abc"
        "a2", "a2bc", "1+2", "+",

        //
        // atoms with escaping
        //
        "\\a", "a\\ \\ b", "\\'",

        //
        // atoms with multi escaping
        //
        "|a|", "|a  b|", "|'|"
    };

    // concatenate ones into one string (room programm)
    std::ostringstream pgm;
    std::copy(lexemes.begin(),
              lexemes.end(),
              std::ostream_iterator<std::string>(pgm, " ")); // in this test we use blank space as delimiter of atoms

    auto lexer = tokenize(pgm.str());

    for (auto &lexeme : lexemes) {
        auto token = lexer.next();
        REQUIRE(is<core::tokens::Atom>(token));
//        REQUIRE(token.lexeme() == unescape(lexeme));
    }

    // End token verification
    REQUIRE(is<core::tokens::End>(lexer.next()));
}
