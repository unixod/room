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

    auto root = std::make_unique<room::ast::Set>(quoted);

    std::stack<room::ast::Set *> currentSet{{root.get()}};

    for(auto token = nextToken();
        std::get<lexer::TokenClass>(token) != lexer::token::Class::End;
        token = nextToken()) {

        switch(std::get<lexer::TokenClass>(token)) {
        case lexer::token::Class::Atom:
            currentSet.top()->elements.emplace_back(new room::ast::Atom(std::get<lexer::TokenLexeme>(token), quoted));
            quoted = false;
            break;
        case lexer::token::Class::SpaceBegin:
            room::ast::Set *subset;
            currentSet.top()->elements.emplace_back(subset = new room::ast::Set(quoted));
            currentSet.push(subset);
            break;
        case lexer::token::Class::SpaceEnd:
            if (currentSet.size() == 1) {
                throw std::domain_error{"room::lst::parse, unexpected end of space"};
            }
            currentSet.pop();
            break;
        case lexer::token::Class::Quotation:
            quoted = true;
            break;
        default:
            throw std::domain_error{"room::lst::parse, unexpected token"};
        }
    }

    return std::move(root);
}
