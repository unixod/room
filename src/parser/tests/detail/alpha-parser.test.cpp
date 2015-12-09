#include <catch.hpp>
#include "detail/alpha-parser.h"
#include "detail/atom-set.h"

using room::parser::detail::makeAtomSet;
using room::parser::detail::AtomSet;
using room::lexer::Token;

/* tokenizer is a simplified version of Room's lexer
 * simplification is as follows:
 *   1. There are no singular spaces;
 *   2. Atom's name must consists of only characters in range from 'a' to 'z';
 *   3. Atom's name must be one-character;
 *   3. There are neither comments nor escaping nor multiline support;
 */
auto tokenize(const char *form)
{
    std::vector<Token> tokens;

    while(char thing = *form++) {
        switch(thing){
        case '\'':
            tokens.emplace_back(Token::Category::Quotation, "");
            break;
        case '{':
            tokens.emplace_back(Token::Category::SpaceBegin, "");
            break;
        case '}':
            tokens.emplace_back(Token::Category::SpaceEnd, "");
            break;
        default:    // skip all things except characters in range from 'a' to 'z'
            if ((thing >= 'a') && (thing <= 'z')) {
                auto atomName = std::string{form-1, form}; // string representation of thing
                tokens.emplace_back(Token::Category::Atom, atomName);
            }
            break;
        }
    }

    return [i = std::size_t(0), tokens]() mutable {
        if (i < tokens.size()) {
            return tokens[i++];
        } else {
            return Token{Token::Category::End, ""};
        }
    };
}

std::string stringify(std::unique_ptr<AtomSet> atomSet)
{
    std::string result;

    for(; atomSet; atomSet = std::move(atomSet->sibling)) {
        switch (atomSet->type) {
        case AtomSet::Type::Atom: {
            auto&& atom = atomSet->asAtom();

            if (atom.quoted) {
                result += '\'';
            }
            result += atom.name;

            break;
        }
        case AtomSet::Type::Set: {
            auto&& set = atomSet->asSet();

            if (set.quoted) {
                result += '\'';
            }
            result += '{';
            result += stringify(std::move(set.child));
            result += "}";
            break;
        }
        case AtomSet::Type::Undefined:
            throw std::runtime_error{"stringify: something goes wrong"};
        }

        result += ' ';
    }


    // trim trailing whitespaces
    auto rTrimPos = result.find_last_not_of(' ');
    if (rTrimPos == std::string::npos) {
        result.clear();
    } else if (rTrimPos + 1 < result.size()){
        result.erase(rTrimPos + 1);
    }

    return result;
}

TEST_CASE("making AtomSet from correct formed sequence of tokens")
{
    auto sequences = {
        "",                         // empty token set
        "a b 'c",                   // only atoms
        "{} {} '{{} {} '{'{}}}",    // only sets
        "'a {b 'c} d",              // sets and atoms
    };

    for (auto &&seq : sequences) {
        CAPTURE(seq);
        REQUIRE(stringify(makeAtomSet(tokenize(seq))) == seq);
    }
}

TEST_CASE("handling of incorrect formed sequence of tokens")
{
    auto sequences = {
        "}",                        // unexpected SpaceEnd
        "{'}",                      // quoted SpaceEnd
        "''a",                      // contiues quotation
        "{",                        // unexpected end of space
        "a '"                       // unexpected end of space
    };

    for (auto &&seq : sequences) {
        CAPTURE(seq);
        REQUIRE_THROWS_AS(makeAtomSet(tokenize(seq)), std::runtime_error);
    }
}

