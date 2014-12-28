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
    case token::Class::SpaceBegin:  out << "SpaceBegin"; break;
    case token::Class::SpaceEnd:    out << "SpaceEnd";   break;
    case token::Class::Quotation:   out << "Quotation";  break;
    case token::Class::Atom:        out << "Atom";       break;
    case token::Class::End:         out << "End";        break;
    case token::Class::Error:       out << "Error";      break;
    default:                        out << "Unknown";
    }

    return std::get<core::TokenClass>(token) == token::Class::Atom ?
                out << "{" << std::get<core::TokenLexeme>(token) << "}" : out;
}

bool operator == (const core::Token &t1, const core::Token &t2) {
    auto t1Class = std::get<core::TokenClass>(t1);
    return t1Class == std::get<core::TokenClass>(t2) &&
            (t1Class == core::token::Class::SpaceBegin ||
             t1Class == core::token::Class::SpaceEnd ||
             std::get<core::TokenLexeme>(t1) == std::get<core::TokenLexeme>(t2));

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

core::Token spaceBegin() {
    return {token::Class::SpaceBegin, ""};
}

core::Token spaceEnd() {
    return {token::Class::SpaceEnd, ""};
}

core::Token quote() {
    return {token::Class::Quotation, "'"};
}

core::Token atom(const std::string &lexeme) {
    return {token::Class::Atom, lexeme};
}

}

Tokens tokenize(std::string pgm) {
    std::istringstream src{pgm};
    auto lexer = room::Lexer{
        src
    };

    Tokens out;
    core::Token token;

    while(lexer >> token) {
        out.push_back(token);
    }

    return out;
}

TEST_CASE("Atoms tokenization") {
    SECTION("in regular space") {
        SECTION("without escaping") {
            REQUIRE(tokenize("a abc") == tok::set(tok::atom("a"), tok::atom("abc")));
            REQUIRE(tokenize("1 12 32") == tok::set(tok::atom("1"), tok::atom("12"), tok::atom("32")));
            REQUIRE(tokenize("1a a2b") == tok::set(tok::atom("1a"), tok::atom("a2b")));
            REQUIRE(tokenize(". ...") == tok::set(tok::atom("."), tok::atom("...")));
        }

        SECTION("with escaping") {
            REQUIRE(tokenize("a\\ abc") == tok::set(tok::atom("a abc")));
        }

        SECTION("with quotations") {
            REQUIRE(tokenize("'a abc") == tok::set(tok::quote(), tok::atom("a"), tok::atom("abc")));
            REQUIRE(tokenize("a 'abc") == tok::set(tok::atom("a"), tok::quote(), tok::atom("abc")));
            REQUIRE(tokenize("a a'bc") == tok::set(tok::atom("a"), tok::atom("a'bc")));
            REQUIRE(tokenize("a' abc") == tok::set(tok::atom("a'"), tok::atom("abc")));
            REQUIRE(tokenize("a abc'") == tok::set(tok::atom("a"), tok::atom("abc'")));

        }
    }

    SECTION("in singular space") {
        REQUIRE(tokenize("(abc)") == tok::set(tok::spaceBegin(),
                                              tok::atom("a"), tok::atom("b"), tok::atom("c"),
                                              tok::spaceEnd()));

        REQUIRE(tokenize("(a abc)") == tok::set(tok::spaceBegin(),
                                                tok::atom("a"), tok::atom(" "), tok::atom("a"), tok::atom("b"), tok::atom("c"),
                                                tok::spaceEnd()));
    }
}
