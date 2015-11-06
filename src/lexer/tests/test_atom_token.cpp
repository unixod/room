#include <catch.hpp>
#include <sstream>
#include <iterator>
#include "room/lexer.h"

namespace lexer = room::lexer;

/// Catch helpers
namespace std {

std::ostream & operator << (std::ostream &out, const lexer::Token &token)
{

    switch(token.category) {
    case lexer::Token::Category::Atom:        out << "Atom";
        out << "{" << token.lexeme << "}";
        break;
    case lexer::Token::Category::Error:       out << "Error";
        out << "{" << token.lexeme << "}";
        break;
    case lexer::Token::Category::SpaceBegin:  out << "SpaceBegin"; break;
    case lexer::Token::Category::SpaceEnd:    out << "SpaceEnd";   break;
    case lexer::Token::Category::Quotation:   out << "Quotation";  break;
    case lexer::Token::Category::End:         out << "End";        break;
    default:                        out << "Unknown";
    }

    return out;
}

} // namespace std

typedef std::vector<lexer::Token> Tokens;

namespace tok {

template<class... T>
Tokens set(T&&... tokens)
{
    return {std::forward<T>(tokens)...};
}

lexer::Token spaceBegin()
{
    return {lexer::Token::Category::SpaceBegin, ""};
}

lexer::Token spaceEnd()
{
    return {lexer::Token::Category::SpaceEnd, ""};
}

lexer::Token quote()
{
    return {lexer::Token::Category::Quotation, "'"};
}

lexer::Token atom(const std::string &lexeme)
{
    return {lexer::Token::Category::Atom, lexeme};
}

lexer::Token error(const std::string &lexeme)
{
    return {lexer::Token::Category::Error, lexeme};
}

} // namespace tok

Tokens tokenize(std::string pgm)
{
    std::istringstream src{pgm};
    auto lexer = room::Lexer{src};

    Tokens out;

    for(lexer::Token token = lexer.nextToken();
        token.category != lexer::Token::Category::End;
        token = lexer.nextToken()) {

        out.push_back(token);

        if (token.category == lexer::Token::Category::Error) {
            out.back().lexeme = "offset: " + std::to_string(lexer.currentOffset());
            break;
        }
    }

    return out;
}

TEST_CASE("Atoms tokenization") {
    SECTION("in regular space") {
        REQUIRE(
            tokenize("a abc")
                ==
            tok::set(tok::atom("a"), tok::atom("abc"))
        );

        REQUIRE(
            tokenize("1 12 32")
                ==
            tok::set(tok::atom("1"), tok::atom("12"), tok::atom("32"))
        );

        REQUIRE(
            tokenize("1a a2b")
                    ==
            tok::set(tok::atom("1a"), tok::atom("a2b"))
        );

        REQUIRE(
            tokenize(". ...")
                ==
            tok::set(tok::atom("."), tok::atom("..."))
        );


        SECTION("escaping") {
            REQUIRE(
                tokenize("\\'a")
                    ==
                tok::set(tok::atom("'a"))
            );

            REQUIRE(
                tokenize("a\\ abc")
                    ==
                tok::set(tok::atom("a abc"))
            );

            REQUIRE(
                tokenize("a a\\b\\c")
                    ==
                tok::set(tok::atom("a"), tok::atom("abc"))
            );
        }

        SECTION("multiescaping") {
            REQUIRE(
                tokenize("|'a|")
                    ==
                tok::set(tok::atom("'a"))
            );

            REQUIRE(
                tokenize("|abc|")
                    ==
                tok::set(tok::atom("abc"))
            );

            REQUIRE(
                tokenize("|a bc|")
                    ==
                tok::set(tok::atom("a bc"))
            );

            REQUIRE(
                tokenize("|a\\ bc|")
                    ==
                tok::set(tok::atom("a\\ bc"))
            );
        }

        SECTION("quotations") {
            REQUIRE(
                tokenize("a'bc")
                    ==
                tok::set(tok::atom("a'bc"))
            );

            REQUIRE(
                tokenize("a' bc")
                    ==
                tok::set(tok::atom("a'"), tok::atom("bc"))
            );

            REQUIRE(
                tokenize("'a")
                    ==
                tok::set(tok::quote(), tok::atom("a"))
            );

            REQUIRE(
                tokenize("a 'abc")
                    ==
                tok::set(tok::atom("a"), tok::quote(), tok::atom("abc"))
            );
        }
    }

    SECTION("in singular space") {
        REQUIRE(
            tokenize("(abc)")
                ==
            tok::set(tok::spaceBegin(),
                     tok::atom("a"), tok::atom("b"), tok::atom("c"),
                     tok::spaceEnd())
        );

        REQUIRE(
            tokenize("(a abc)")
                ==
            tok::set(tok::spaceBegin(),
                     tok::atom("a"), tok::atom(" "), tok::atom("a"), tok::atom("b"), tok::atom("c"),
                     tok::spaceEnd())
        );

        REQUIRE(
            tokenize("({a}b)")
                ==
            tok::set(tok::spaceBegin(),
                     tok::atom("{"), tok::atom("a"), tok::atom("}"), tok::atom("b"),
                     tok::spaceEnd())
        );

        REQUIRE(
            tokenize("(|ab)")
                ==
            tok::set(tok::spaceBegin(),
                     tok::atom("|"), tok::atom("a"), tok::atom("b"),
                     tok::spaceEnd())
        );

        REQUIRE(
            tokenize("('ab)")
                ==
            tok::set(tok::spaceBegin(),
                     tok::quote(), tok::atom("a"), tok::atom("b"),
                     tok::spaceEnd())
        );

        REQUIRE(
            tokenize("(a'b)")
                ==
            tok::set(tok::spaceBegin(),
                     tok::atom("a"), tok::quote(), tok::atom("b"),
                     tok::spaceEnd())
        );

        REQUIRE(
            tokenize("(a\"b)")
                ==
            tok::set(tok::spaceBegin(),
                     tok::atom("a"), tok::atom("\""), tok::atom("b"),
                     tok::spaceEnd())
        );

        REQUIRE(
            tokenize("(a\\'b)")
                ==
            tok::set(tok::spaceBegin(),
                     tok::atom("a"), tok::atom("'"), tok::atom("b"),
                     tok::spaceEnd())
        );

        REQUIRE(
            tokenize("(a\\)b)")
                ==
            tok::set(tok::spaceBegin(),
                     tok::atom("a"), tok::atom(")"), tok::atom("b"),
                     tok::spaceEnd())
        );
    }
}

TEST_CASE("Error handling") {
    REQUIRE(
        tokenize("abc ''d efg")
            ==
        tok::set(tok::atom("abc"),
                 tok::error("offset: 4"))
    );

    REQUIRE(
        tokenize("' a")
            ==
        tok::set(tok::error("offset: 0"))
    );
}
