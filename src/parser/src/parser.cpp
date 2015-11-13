#include "room/parser.h"
#include "room/parser/ast.h"
#include "detail/alpha-parser.h"
#include "detail/atom-tree.h"

using room::parser::Ast;
namespace detail = room::parser::detail;


namespace {

Ast makeAst(detail::AtomTree);

} // namespace


Ast room::parse(std::function<lexer::Token()> generator)
{
    auto atomTree = detail::alpha_parse(std::move(generator));

    return makeAst(std::move(atomTree));
}


namespace {

Ast makeAst(detail::AtomTree atomTree)
{
}

} // namespace
