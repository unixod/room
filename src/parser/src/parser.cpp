#include "room/parser.h"
#include "room/parser/ast.h"
#include "detail/alpha-parser.h"
#include "detail/atom-set.h"

using room::parser::Ast;
namespace detail = room::parser::detail;

static Ast makeAst(detail::AtomSet);


Ast room::parse(std::function<lexer::Token()> generator)
{
    auto atomSet = detail::alphaParse(std::move(generator));

    return makeAst(std::move(atomSet));
}


static Ast makeAst(detail::AtomSet atomTree)
{
}


