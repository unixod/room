#include <stack>
#include "room/lst/parser.h"
#include "room/lst/atom.h"
#include "room/lst/set.h"

namespace lexer = room::lexer;
using lexer::Token;

std::unique_ptr<room::lst::Symbol>
room::lst::parse(std::function<lexer::Token ()> nextToken)
{   
    bool quoted = false;    // next symbol should be quoted in case this flag is set

    auto root = std::make_unique<room::lst::Set>(quoted);

    std::stack<room::lst::Set *> currentSet{{root.get()}};

    for(auto token = nextToken();
        token.category != Token::Category::End;
        token = nextToken()) {

        switch(token.category) {
        case Token::Category::Atom:
            currentSet.top()->elements.emplace_back(new room::lst::Atom(std::move(token.lexeme), quoted));
            quoted = false;
            break;
        case Token::Category::SpaceBegin:
            room::lst::Set *subset;
            currentSet.top()->elements.emplace_back(subset = new room::lst::Set(quoted));
            currentSet.push(subset);
            break;
        case Token::Category::SpaceEnd:
            if (currentSet.size() == 1) {
                throw std::domain_error{"room::lst::parse, unexpected SpaceEnd token"};
            }
            currentSet.pop();
            break;
        case Token::Category::Quotation:
            quoted = true;
            break;
        default:
            throw std::domain_error{"room::lst::parse, unexpected token"};
        }
    }

    return (currentSet.size() == 1) ? std::move(root)
                                    : throw std::domain_error{"room::lst::parse, unexpected end of space"};
}
