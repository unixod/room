#include <stack>
#include "room/common.h"
#include "detail/alpha-parser.h"
#include "detail/atom-set.h"

namespace detail = room::parser::detail;

std::unique_ptr<detail::AtomSet>
detail::makeAtomSet(std::function<room::lexer::Token()> nextToken)
{
    using room::lexer::Token;

    std::unique_ptr<AtomSet> result;
    std::unique_ptr<AtomSet> *insertionPoint = &result;
    std::stack<decltype(insertionPoint)> insetionLevelStack;

    for (bool quoted = false, endReached = false; !endReached;) {
        auto token = nextToken();

        switch (token.category) {
        case Token::Category::Atom:
            *insertionPoint = std::make_unique<AtomSet>(AtomSet::AtomDescription{
                                                            quoted, token.lexeme
                                                        });

            insertionPoint = &(*insertionPoint)->sibling;
            quoted = false;
            break;

        case Token::Category::SpaceBegin: {
            *insertionPoint = std::make_unique<AtomSet>(AtomSet::SetDescription{
                                                            quoted, nullptr
                                                        });


            auto &&child = &(*insertionPoint)->asSet().child;

            insertionPoint = &(*insertionPoint)->sibling;
            insetionLevelStack.push(insertionPoint);

            insertionPoint = child;

            quoted = false;
            break;
        }

        case Token::Category::SpaceEnd:
            if (insetionLevelStack.empty()) {
                throw std::runtime_error{ERROR_MSG "unexpected [SpaceEnd]"};
            } else if (quoted == true) {
                throw std::runtime_error{ERROR_MSG "[SpaceEnd] can't be quoted"};
            }
            insertionPoint = insetionLevelStack.top();
            insetionLevelStack.pop();
            break;

        case Token::Category::Quotation:
            if (quoted == true) {
                throw std::runtime_error{ERROR_MSG "contiguous quotations are not allowed"};
            }
            quoted = true;
            break;

        case Token::Category::End:
            if ((!insetionLevelStack.empty()) || (quoted == true)) {
                throw std::runtime_error{ERROR_MSG "unexpected end of space"};
            }
            endReached = true;
            break;

        case Token::Category::Error:
            throw std::runtime_error{ERROR_MSG "unexpected end of space"};
            break;
        }
    }

    return result;
}
