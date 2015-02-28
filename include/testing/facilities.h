#ifndef TESTING_FACILITIES_H
#define TESTING_FACILITIES_H

#include <string>
#include "room/utils/c++14.h"
#include "room/ast/lst/atom.h"
#include "room/ast/lst/set.h"

//
// Explanation of swallowing:
//
// template<class... T>
// std::string cat(T&&... elts) {
//     std::string str;
//     using swallow = int[];
//     (void)swallow{0, (void(str.append(elts).append(" ")), 0)...};
//     return str;
// }
//
#define EVAL_FOR_EACH(expr) \
    (void)(int[]){(void(expr), 0)...}


namespace testing {

enum {
    Source,
    LST
};

std::pair<std::string, std::unique_ptr<room::ast::Symbol>>
Atom(const std::string &name, bool quoted = false)
{
    return {quoted ? "'" + name : name, std::make_unique<room::ast::Atom>(name, quoted)};
}

std::pair<std::string, std::unique_ptr<room::ast::Symbol>>
QuotedAtom(const std::string &name)
{
    return Atom(name, true);
}

template<class... T>
std::pair<std::string, std::unique_ptr<room::ast::Symbol>>
Set(T&&... elt)
{
    std::string src = "{";
    EVAL_FOR_EACH(src.append(std::get<Source>(elt)).append(" "));
    src += "}";

    auto root = std::make_unique<room::ast::Set>(false);
    EVAL_FOR_EACH(root->elements.emplace_back(std::move(std::get<LST>(elt))));

    return {src, std::move(root)};
}

template<class... T>
std::pair<std::string, std::unique_ptr<room::ast::Symbol>>
QuotedSet(T&&... elt)
{
    std::string src = "'{";
    EVAL_FOR_EACH(src.append(std::get<Source>(elt)).append(" "));
    src += "}";

    auto root = std::make_unique<room::ast::Set>(true);
    EVAL_FOR_EACH(root->elements.emplace_back(std::move(std::get<LST>(elt))));

    return {src, std::move(root)};
}

template<class... T>
std::pair<std::string, std::unique_ptr<room::ast::Symbol>> Program(T&&... elt) {
    std::string src;
    EVAL_FOR_EACH(src.append(std::get<Source>(elt)).append(" "));

    auto root = std::make_unique<room::ast::Set>(false);
    EVAL_FOR_EACH(root->elements.emplace_back(std::move(std::get<LST>(elt))));

    return {src, std::move(root)};
}

} // namespace testing

#endif // TESTING_FACILITIES_H


