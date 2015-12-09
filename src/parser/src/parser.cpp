#include "room/parser.h"
#include "room/parser/ast.h"
#include "detail/alpha-parser.h"
#include "detail/atom-set.h"

using room::parser::Ast;
namespace detail = room::parser::detail;


static Ast makeAst(std::unique_ptr<detail::AtomSet>);

Ast room::parse(std::function<lexer::Token()> generator)
{
    return makeAst(detail::makeAtomSet(std::move(generator)));
}

static Ast makeAst(std::unique_ptr<detail::AtomSet> atomSet)
{
}


