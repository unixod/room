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

typedef std::vector<core::Token> TokenSet;

TokenSet tokenize(std::string pgm) {
    auto lexer = room::Lexer{
        std::make_unique<utils::SourceString>(
            pgm
        )
    };

    TokenSet out;

    while(lexer.hasNext()) {
        out.push_back(lexer.next());
    }

    return out;
}

template<class... Tokens>
std::tuple<Tokens...> tokens(Tokens&&... tokens) {
    return std::forward_as_tuple(std::forward<Tokens>(tokens)...);
}

template<class Tokens, std::size_t... idx>
bool compare(const TokenSet &tokenized, Tokens &&expected, std::index_sequence<idx...>) {
    typedef typename std::remove_reference<Tokens>::type ExpectedSet;

    auto cmps = {
        tokenized[idx] == std::get<idx>(expected)
        ...
    };
    return std::all_of(cmps.begin(), cmps.end(), [](bool v){return v;});
}

template<class Tokens, std::size_t idx = 0>
bool operator == (const TokenSet &tokenized, Tokens &&expected) {
    typedef typename std::remove_reference<Tokens>::type ExpectedSet;
    constexpr auto expectedSetSize = std::tuple_size<ExpectedSet>::value;

    return expectedSetSize == tokenized.size() &&
            compare(tokenized, std::forward<Tokens>(expected),
                    std::make_index_sequence<expectedSetSize>());
}

//template<class Ch, class Traits, class Tuple, std::size_t... idx>
//void printTuple(std::basic_ostream<Ch, Traits> &out, const Tuple tpl, std::index_sequence<idx...>) {
//    using swallow = int[];
//    (void)swallow{0, (void(out << (idx == 0 ? "" : ",") << std::get<idx>(tpl)), 0)... };
//}

namespace std {
//template<class... Tokens>
//std::ostream & operator << (std::ostream &out, const std::tuple<Tokens...> &tokens) {
//    out << "bobo";
//    return out;
//}

std::ostream & operator << (std::ostream &out, const core::Token &token) {

    switch(std::get<core::TokenClass>(token)) {
    case core::token::Class::Space: out << "Space"; break;
    case core::token::Class::Atom:  out << "Atom";  break;
    case core::token::Class::End:   out << "End";   break;
    default: out << "Unknown";
    }

    return out << "{" << std::get<core::TokenLexeme>(token) << "}";
}
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
//        FAIL("not implemented yet...");
    }
}
