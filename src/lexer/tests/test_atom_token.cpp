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
namespace token = core::tokens;

/// Helpers
template<class T>
bool is(const room::core::Token *t) {
    return dynamic_cast<typename
            std::add_const<T>::type *>(t);
}

std::vector<std::unique_ptr<core::Token>>
tokenize(std::string pgm) {
    auto lexer = room::Lexer{
        std::make_unique<utils::SourceString>(
            pgm
        )
    };

    std::vector<std::unique_ptr<core::Token>> out;

    while(lexer.hasNext()) {
        out.push_back(lexer.next());
    }

    return out;
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

template<class... Tokens>
std::vector<std::reference_wrapper<const core::Token>> tokens(Tokens&&... tokens) {
    return {tokens...};
}

bool operator == (const std::vector<std::unique_ptr<core::Token>> &tokenized,
                  const std::vector<std::reference_wrapper<const core::Token>> &expected) {
    return tokenized.size() == expected.size() &&
           std::equal(tokenized.begin(), tokenized.end(), expected.begin(),
                      [] (const std::unique_ptr<core::Token> &givenToken, const core::Token &expectedToken){
        return false;
    });
}


TEST_CASE("Atoms tokenization") {
//    SECTION("in regular space") {
//        // atom lexemes
//        const auto lexemes = std::vector<std::string>{
//            //
//            // simple atoms
//            //
//            "a", "abc", "1a", "1abc"
//            "a2", "a2bc", "1+2", "+",

//            //
//            // atoms with escaping
//            //
//            "\\a", "a\\ \\ b", "\\'",

//            //
//            // atoms with multi escaping
//            //
//            "|a|", "|a  b|", "|'|"
//        };

//        // concatenate ones into one string (room programm)
//        std::ostringstream pgm;
//        std::copy(lexemes.begin(),
//                  lexemes.end(),
//                  std::ostream_iterator<std::string>(pgm, " ")); // in this test we use blank space as delimiter of atoms

//        auto lexer = tokenize(pgm.str());

//        for (auto &lexeme : lexemes) {
//            auto token = lexer.next();
//            REQUIRE(is<core::tokens::Atom>(token));
//    //        REQUIRE(token.lexeme() == unescape(lexeme));
//        }

//        // End token verification
//        REQUIRE(is<core::tokens::End>(lexer.next()));
//    }

    SECTION("in singular space") {
        REQUIRE(tokenize("(abc)") == tokens(token::Atom{"a"}, token::Atom{"b"}, token::Atom{"c"}));
    }

    SECTION("in singular space") {
        FAIL("not implemented yet...");
    }
}
