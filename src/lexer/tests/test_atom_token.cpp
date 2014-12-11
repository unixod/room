#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_SFINAE
#include <catch.hpp>
#include <sstream>
#include <iterator>
#include "room/lexer.h"
#include "room/utils/source_string.h"
#include "room/utils/c++14.h"

namespace core = room::core;
namespace utils = room::utils;
namespace token = room::core::token;

/// Catch helpers
namespace std {
std::ostream & operator << (std::ostream &out, const core::Token &token) {

    switch(std::get<core::TokenClass>(token)) {
    case token::Class::SpaceBegin: out << "SpaceBegin"; break;
    case token::Class::SpaceEnd:   out << "SpaceEnd";   break;
    case token::Class::Atom:       out << "Atom";       break;
    case token::Class::End:        out << "End";        break;
    default:                       out << "Unknown";
    }

    return std::get<core::TokenClass>(token) == token::Class::Atom ?
                out << "{" << std::get<core::TokenLexeme>(token) << "}" : out;
}
}

/// Helpers
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

typedef std::vector<core::Token> Tokens;

namespace tok {
template<class... T>
Tokens set(T&&... tokens) {
    return {std::forward<T>(tokens)...};
}

core::Token atom(const std::string &lexeme) {
    return {token::Class::Atom, lexeme};
}

core::Token end() {
    return {token::Class::End, ""};
}

core::Token spaceBegin() {
    return {token::Class::SpaceBegin, ""};
}

core::Token spaceEnd() {
    return {token::Class::SpaceEnd, ""};
}
}

Tokens tokenize(std::string pgm) {
    auto lexer = room::Lexer{
        std::make_unique<utils::SourceString>(
            pgm
        )
    };

    Tokens out;

    while(lexer.hasNext()) {
        out.push_back(lexer.next());
    }

    return out;
}

TEST_CASE("Atoms tokenization") {
    SECTION("in singular space") {
        SECTION("without escaping") {
            REQUIRE(tokenize("a abc") == tok::set(tok::atom("a"), tok::atom("abc"), tok::end()));
            REQUIRE(tokenize("1 12 32") == tok::set(tok::atom("1"), tok::atom("12"), tok::atom("32"), tok::end()));
            REQUIRE(tokenize("1a a2b") == tok::set(tok::atom("1a"), tok::atom("a2b"), tok::end()));
            REQUIRE(tokenize(". ...") == tok::set(tok::atom("."), tok::atom("..."), tok::end()));
        }
    }

    SECTION("in singular space") {
        REQUIRE(tokenize("(abc)") == tok::set(tok::spaceBegin(),
                                              tok::atom("a"), tok::atom("b"), tok::atom("c"),
                                              tok::spaceEnd(),
                                              tok::end()));

        REQUIRE(tokenize("(a abc)") == tok::set(tok::spaceBegin(),
                                                tok::atom("a"), tok::atom(" "), tok::atom("a"), tok::atom("b"), tok::atom("c"),
                                                tok::spaceEnd(),
                                                tok::end()));
    }
}
