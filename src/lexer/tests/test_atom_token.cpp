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

typedef std::vector<core::Token> Tokens;

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

template<class... T>
Tokens tokens(T&&... tokens) {
    return {std::forward<T>(tokens)...};
}

TEST_CASE("Atoms tokenization") {
    SECTION("in singular space") {
        SECTION("without escaping") {
            REQUIRE(tokenize("a abc") == tokens(core::Token{token::Class::Atom, "a"},
                                                core::Token{token::Class::Atom, "abc"},
                                                core::Token{token::Class::End, ""}));

            REQUIRE(tokenize("1 12 32") == tokens(core::Token{token::Class::Atom, "1"},
                                                  core::Token{token::Class::Atom, "12"},
                                                  core::Token{token::Class::Atom, "32"},
                                                  core::Token{token::Class::End, ""}));

            REQUIRE(tokenize("1a a2b") == tokens(core::Token{token::Class::Atom, "1a"},
                                                 core::Token{token::Class::Atom, "a2b"},
                                                 core::Token{token::Class::End, ""}));

            REQUIRE(tokenize(". ...") == tokens(core::Token{token::Class::Atom, "."},
                                                core::Token{token::Class::Atom, "..."},
                                                core::Token{token::Class::End, ""}));
        }
    }

    SECTION("in singular space") {
        REQUIRE(tokenize("(abc)") == tokens(core::Token{token::Class::SpaceBegin, ""},
                                            core::Token{token::Class::Atom, "a"},
                                            core::Token{token::Class::Atom, "b"},
                                            core::Token{token::Class::Atom, "c"},
                                            core::Token{token::Class::SpaceEnd, ""},
                                            core::Token{token::Class::End, ""}));
    }
}
