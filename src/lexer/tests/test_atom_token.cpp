#include <catch.hpp>
#include <sstream>
#include <iterator>
#include "room/lexer.h"
#include "room/utils/source_string.h"
#include "room/utils/c++14.h"

namespace utils = room::utils;
namespace lexer = room::lexer;
namespace token = room::lexer::token;

/// Catch helpers
namespace std {
std::ostream & operator << (std::ostream &out, const lexer::Token &token)
{

    switch(std::get<lexer::TokenClass>(token)) {
    case token::Class::Atom:        out << "Atom";
        out << "{" << std::get<lexer::TokenLexeme>(token) << "}";
        break;
    case token::Class::Error:       out << "Error";
        out << "{" << std::get<lexer::TokenLexeme>(token) << "}";
        break;
    case token::Class::SpaceBegin:  out << "SpaceBegin"; break;
    case token::Class::SpaceEnd:    out << "SpaceEnd";   break;
    case token::Class::Quotation:   out << "Quotation";  break;
    case token::Class::End:         out << "End";        break;
    default:                        out << "Unknown";
    }

    return out;
}

bool operator == (const lexer::Token &t1, const lexer::Token &t2)
{
    auto t1Class = std::get<lexer::TokenClass>(t1);
    return t1Class == std::get<lexer::TokenClass>(t2) &&
            (t1Class == token::Class::SpaceBegin ||
             t1Class == token::Class::SpaceEnd ||
             std::get<lexer::TokenLexeme>(t1) == std::get<lexer::TokenLexeme>(t2));

}
} // namespace std

/// Helpers
std::string unescape(std::string lexeme)
{
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

typedef std::vector<lexer::Token> Tokens;

namespace tok {
template<class... T>
Tokens set(T&&... tokens)
{
    return {std::forward<T>(tokens)...};
}

lexer::Token spaceBegin()
{
    return {token::Class::SpaceBegin, ""};
}

lexer::Token spaceEnd()
{
    return {token::Class::SpaceEnd, ""};
}

lexer::Token quote()
{
    return {token::Class::Quotation, "'"};
}

lexer::Token atom(const std::string &lexeme)
{
    return {token::Class::Atom, lexeme};
}

lexer::Token error(const std::string &lexeme)
{
    return {token::Class::Error, lexeme};
}
} // namespace tok

Tokens tokenize(std::string pgm)
{
    std::istringstream src{pgm};
    auto lexer = room::Lexer{
        src
    };

    Tokens out;
    lexer::Token token;

    while(lexer >> token) {
        out.push_back(token);

        if (std::get<lexer::TokenClass>(token) == token::Class::Error) {
            std::get<lexer::TokenLexeme>(out.back()) = "offset: " + std::to_string(lexer.currentOffset());
            break;
        }
    }

    return out;
}

TEST_CASE("Atoms tokenization") {
    SECTION("in regular space") {
        REQUIRE(tokenize("a abc") == tok::set(tok::atom("a"), tok::atom("abc")));
        REQUIRE(tokenize("1 12 32") == tok::set(tok::atom("1"), tok::atom("12"), tok::atom("32")));
        REQUIRE(tokenize("1a a2b") == tok::set(tok::atom("1a"), tok::atom("a2b")));
        REQUIRE(tokenize(". ...") == tok::set(tok::atom("."), tok::atom("...")));


        SECTION("escaping") {
            REQUIRE(tokenize("\\'a") == tok::set(tok::atom("'a")));
            REQUIRE(tokenize("a\\ abc") == tok::set(tok::atom("a abc")));
            REQUIRE(tokenize("a a\\b\\c") == tok::set(tok::atom("a"), tok::atom("abc")));
        }

        SECTION("multiescaping") {
            REQUIRE(tokenize("|'a|") == tok::set(tok::atom("'a")));
            REQUIRE(tokenize("|abc|") == tok::set(tok::atom("abc")));
            REQUIRE(tokenize("|a bc|") == tok::set(tok::atom("a bc")));
            REQUIRE(tokenize("|a\\ bc|") == tok::set(tok::atom("a\\ bc")));
        }

        SECTION("quotations") {
            REQUIRE(tokenize("a'bc") == tok::set(tok::atom("a'bc")));
            REQUIRE(tokenize("a' bc") == tok::set(tok::atom("a'"), tok::atom("bc")));
            REQUIRE(tokenize("'a") == tok::set(tok::quote(), tok::atom("a")));
            REQUIRE(tokenize("a 'abc") == tok::set(tok::atom("a"), tok::quote(), tok::atom("abc")));
        }
    }

    SECTION("in singular space") {
        REQUIRE(tokenize("(abc)") == tok::set(tok::spaceBegin(),
                                              tok::atom("a"), tok::atom("b"), tok::atom("c"),
                                              tok::spaceEnd()));

        REQUIRE(tokenize("(a abc)") == tok::set(tok::spaceBegin(),
                                                tok::atom("a"), tok::atom(" "), tok::atom("a"), tok::atom("b"), tok::atom("c"),
                                                tok::spaceEnd()));

        REQUIRE(tokenize("({a}b)") == tok::set(tok::spaceBegin(),
                                                tok::atom("{"), tok::atom("a"), tok::atom("}"), tok::atom("b"),
                                                tok::spaceEnd()));

        REQUIRE(tokenize("(|ab)") == tok::set(tok::spaceBegin(),
                                              tok::atom("|"), tok::atom("a"), tok::atom("b"),
                                              tok::spaceEnd()));

        REQUIRE(tokenize("('ab)") == tok::set(tok::spaceBegin(),
                                              tok::quote(), tok::atom("a"), tok::atom("b"),
                                              tok::spaceEnd()));

        REQUIRE(tokenize("(a'b)") == tok::set(tok::spaceBegin(),
                                              tok::atom("a"), tok::quote(), tok::atom("b"),
                                              tok::spaceEnd()));

        REQUIRE(tokenize("(a\"b)") == tok::set(tok::spaceBegin(),
                                                tok::atom("a"), tok::atom("\""), tok::atom("b"),
                                                tok::spaceEnd()));

        REQUIRE(tokenize("(a\\'b)") == tok::set(tok::spaceBegin(),
                                                tok::atom("a"), tok::atom("'"), tok::atom("b"),
                                                tok::spaceEnd()));

        REQUIRE(tokenize("(a\\)b)") == tok::set(tok::spaceBegin(),
                                                tok::atom("a"), tok::atom(")"), tok::atom("b"),
                                                tok::spaceEnd()));
    }
}

TEST_CASE("Error handling") {
    REQUIRE(tokenize("abc ''d efg") == tok::set(tok::atom("abc"), tok::error("offset: 4")));
    REQUIRE(tokenize("' a") == tok::set(tok::error("offset: 0")));
}
