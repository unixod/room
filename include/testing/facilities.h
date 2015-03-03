#ifndef TESTING_FACILITIES_H
#define TESTING_FACILITIES_H

#include <string>
#include "room/utils/c++14.h"
#include "room/ast/lst/atom.h"
#include "room/ast/lst/set.h"

//
// explanation of swallowing:
//
// template<class... T>
// std::string cat(T&&... elts) {
//     std::string str;
//     using swallow = int[];
//     (void)swallow{0, (void(str.append(elts).append(" ")), 0)...};
//     return str;
// }
//
// example:
// template<class... T>
// std::string cat(T&&... ss) {
//     std::string str;
//     EVAL_FOR_EACH(str.append(ss));
//     return str;
// }
#define EVAL_FOR_EACH(expr) \
    (void)(int[]){(void(expr), 0)...}


namespace room {
namespace testing {
namespace lst {

enum {
    Tokens,
    LST
};

typedef std::vector<lexer::Token> TokenSet;

std::pair<TokenSet, std::unique_ptr<ast::Symbol>>
Atom(const std::string &name, bool quoted = false)
{
    return {quoted ? TokenSet{lexer::Token{lexer::token::Class::Quotation, "'"}, lexer::Token{lexer::token::Class::Atom, name}}
                   : TokenSet{lexer::Token{lexer::token::Class::Atom, name}}, std::make_unique<ast::Atom>(name, quoted)};
}

std::pair<TokenSet, std::unique_ptr<ast::Symbol>>
QuotedAtom(const std::string &name)
{
    return Atom(name, true);
}

template<class... T>
std::pair<TokenSet, std::unique_ptr<ast::Symbol>>
Set(T&&... elt)
{
    TokenSet tokens{lexer::Token{lexer::token::Class::SpaceBegin, "{"}};
    EVAL_FOR_EACH(tokens += std::get<Tokens>(elt));
    tokens.emplace_back(lexer::token::Class::SpaceEnd, "}");

    auto root = std::make_unique<ast::Set>(false);
    EVAL_FOR_EACH(root->elements.emplace_back(std::move(std::get<LST>(elt))));

    return {tokens, std::move(root)};
}

template<class... T>
std::pair<std::string, std::unique_ptr<ast::Symbol>>
QuotedSet(T&&... elt)
{
    TokenSet tokens{lexer::Token{lexer::token::Class::Quotation, "'"},
                    lexer::Token{lexer::token::Class::SpaceBegin, "{"}};
    EVAL_FOR_EACH(tokens += std::get<Tokens>(elt));
    tokens.emplace_back(lexer::token::Class::SpaceEnd, "}");

    auto root = std::make_unique<ast::Set>(false);
    EVAL_FOR_EACH(root->elements.emplace_back(std::move(std::get<LST>(elt))));

    return {tokens, std::move(root)};
}

//template<class... T>
//std::pair<TokenSet, std::unique_ptr<ast::Symbol>> Program(T&&... elt) {
//    TokenSet tokens;
//    EVAL_FOR_EACH(tokens += std::get<Tokens>(elt));
//    tokens.emplace_back(lexer::token::Class::End, "<end>");

//    auto root = std::make_unique<ast::Set>(false);
//    EVAL_FOR_EACH(root->elements.emplace_back(std::move(std::get<LST>(elt))));

//    return {tokens, std::move(root)};
//}

class Program {
public:
    template<class... T>
    Program(T&&... elt)
    {
        auto &tokens = _data.first;

        EVAL_FOR_EACH(tokens += std::get<Tokens>(elt));
        tokens.emplace_back(lexer::token::Class::End, "<end>");

        auto &root = _data.second;
        root = std::make_unique<ast::Set>(false);
        EVAL_FOR_EACH(root->elements.emplace_back(std::move(std::get<LST>(elt))));
    }

    const TokenSet & tokens() const {
        return _data.first;
    }

    bool lstIsEqualTo(const ast::Symbol &) const
    {
        return false;
    }

private:
    std::pair<TokenSet, std::unique_ptr<ast::Set>> _data;
};

} // namespace lst
} // namespace testing
} // namespace room

#endif // TESTING_FACILITIES_H


