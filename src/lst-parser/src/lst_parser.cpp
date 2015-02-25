#include <stack>
#include "room/utils/c++14.h"
#include "room/lst_parser.h"
#include "room/ast/lst/atom.h"
#include "room/ast/lst/set.h"

namespace lexer = room::lexer;

std::unique_ptr<room::ast::Symbol>
room::lst::parse(std::function<lexer::Token ()> nextToken)
{   
    bool quoted = false;    // next symbol should be quoted in case this flag is set

    std::stack<std::unique_ptr<room::ast::Set>> sets;
    HERE NEED THE RESTRUCTURATION
    sets.emplace(new room::ast::Set(quoted));   // root

    for(auto token = nextToken();
        std::get<lexer::TokenClass>(token) != lexer::token::Class::End;
        token = nextToken()) {

        switch(std::get<lexer::TokenClass>(token)) {
        case lexer::token::Class::Atom:
            sets.top()->elements.emplace_back(new room::ast::Atom(std::get<lexer::TokenLexeme>(token), quoted));
            quoted = false;
            break;
        case lexer::token::Class::SpaceBegin:
            break;
        case lexer::token::Class::SpaceEnd:
            break;
        case lexer::token::Class::Quotation:
            quoted = true;
            break;
        default:
            ;   // error
        }
    }

    return nullptr;
}
