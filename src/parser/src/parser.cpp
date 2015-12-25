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


struct Subject;
struct Space;
struct Emission;
struct Transformation;
struct BehaviourExtension;
struct BehaviourExtension;

//Subject makeSubject(Set);
//Space makeSpace(Set);
Emission makeEmission();
Transformation makeTransformation(Subject, bool sequential);
BehaviourExtension makeBehaviourExtension(Subject);
BehaviourExtension makeBehaviourExtension(Subject, Space);


std::pair<Prefix, detail::AtomSet> findPrefix(detail::AtomSet seq);

class Subject {
public:
    Subject(detail::AtomSet seq)
    {
        while(seq) {
            std::tail(prefix, tail) = findPrefix(seq);
            seq = tail;
            prefix.handle(*this);
        }
    }

    void addEmmision(Subject, Space) {
    }
};

class Space {
public:
    Space(detail::AtomSet seq)
    {

    }
};

struct AnyAtom {};
struct Set {};

struct EmmisionAtom : AnyAtom {
    static bool operator ()(detail::AtomSet elt) {
        return (elt.type == detail::AtomSet::Type::Atom) &&
                (elt.asAtom().name == ">>");
    }
};


std::pair<Prefix, detail::AtomSet> findPrefix(detail::AtomSet seq)
{
    makeHandlerForPrefix<Set, EmmisionAtom, Set>([](Subject &sbj, detail::AtomSet seq){
        sbj.addEmmision(Subject(std::get<0>(seq)), Space(std::get<2>(seq)));    // std::get<1>(seq) is an Atom(>>)
    });
}

