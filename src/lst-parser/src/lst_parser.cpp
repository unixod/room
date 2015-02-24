#include "room/lst_parser.h"
#include "lst_symbols/atom.h"
#include "lst_symbols/set.h"

namespace lexer = room::lexer;

std::unique_ptr<room::ast::Symbol>
room::lst::parse(std::function<lexer::Token ()> nextToken)
{
    for(auto token = nextToken();
        std::get<lexer::TokenClass>(token) != lexer::token::Class::End;
        token = nextToken()) {

        switch(std::get<lexer::TokenClass>(token)) {
        case lexer::token::Class::Atom:
            break;
        case lexer::token::Class::SpaceBegin:
            break;
        case lexer::token::Class::SpaceEnd:
            break;
        case lexer::token::Class::Quotation:
            break;
        default:
            ;   // error
        }
    }

    return nullptr;
}
