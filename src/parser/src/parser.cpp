#include "room/parser.h"
#include "room/parser/ast.h"
#include "detail/alpha-parser.h"

using room::parser::Ast;
namespace detail = room::parser::detail;

Ast room::parse(std::function<lexer::Token()> generator)
{

}
