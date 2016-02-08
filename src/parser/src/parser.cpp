#include "room/parser.h"
#include "room/parser/ast.h"
#include "detail/alpha-parser.h"
#include "detail/atom-set.h"
#include <vector>

using room::parser::Ast;
namespace detail = room::parser::detail;


static Ast makeAst(std::unique_ptr<detail::AtomSet>);

Ast room::parse(std::function<lexer::Token()> generator)
{
    return makeAst(detail::makeAtomSet(std::move(generator)));
}

class AstNode {
public:
    virtual ~AstNode() {}
};

struct Subject;
struct Space;
struct Emission;


//Subject makeSubject(Set);
//Space makeSpace(Set);
//Emission makeEmission();
//Transformation makeTransformation(Subject, bool sequential);
//BehaviourExtension makeBehaviourExtension(Subject);
//BehaviourExtension makeBehaviourExtension(Subject, Space);




using Handler = std::function<std::unique_ptr<detail::AtomSet>(Subject &sbj, std::unique_ptr<detail::AtomSet> seq)>;

Handler findPrefix(const detail::AtomSet &seq);

class Subject : public AstNode {
public:
    Subject(std::unique_ptr<detail::AtomSet> seq)
    {
        while(seq) {
            auto prefix = findPrefix(*seq);
            seq = prefix(*this, std::move(seq));
        }
    }

    void addEmmision(Subject, Space);
};

class Space : public AstNode {

};

//class PrefixTree {
//public:
//    template<class... ClauseDenotation>
//    PrefixTrie(ClauseDenotation&&... clauseDenotation)
//    {
//        // insert chain
//        auto insert = [this](auto&& cd){
//            auto chain = addChainFor(std::forward<decltype(cd)>(cd));
////            if (chain.handler == nullptr) {
////                chain.setHandler(std::forward<ClauseDenotation>(clauseDenotation));
////            } else {
////                throw "handler exist";
////            }
//        };

//        using swallow = int[];
//        (void)swallow{
//            int{}, (insert(std::forward<ClauseDenotation>(clauseDenotation)), int{})...
//        };
//    }

//    template<class T, class... Chain>
//    int addChainFor(const ClauseDenotation<T, Chain...> &clauseDenotation)
//    {/*
//        ptr = nullptr;

//        using swallow = int[];
//        (void)swallow{
//            int{}, (ptr = insert<Chain>())..., int{}
//        };

//        return ptr;*/
//    }
    
//private:
//    template<class T>
//    Node * insert()
//    {
//        root.type
//    }

//    Node *root;
//};

class Node {
public:
    virtual ~Node() {}

    virtual Handler search(const detail::AtomSet &elt) const
    {
        if (check(elt)) {
            if (elt.sibling) for (auto &&child : children) {
                if (auto h = child->search(*elt.sibling)) {
                    return h;
                }
            }

            return handler();
        }

        return nullptr;
    }

private:
    virtual Handler handler() const = 0;
    virtual bool check(const detail::AtomSet &) const = 0;

protected:
    std::vector<std::unique_ptr<Node>> children;
};

class Root : public Node {
public:
    Handler search(const detail::AtomSet &elt) const override
    {
        for (auto &&child : children) {
            if (auto h = child->search(elt)) {
                return h;
            }
        }

        return nullptr;
    }

private:
    bool check(const detail::AtomSet &) const override
    {
        return true;
    }
};

class Set : public Node {

    bool check(const detail::AtomSet &elt) const override
    {
        return elt.type == detail::AtomSet::Type::Set;
    }

    Handler handler() const override
    {
        return h;
    }

    Handler h;
};

class AnyAtom : public Node {

    bool check(const detail::AtomSet &elt) const override
    {
        return elt.type == detail::AtomSet::Type::Atom;
    }

    Handler handler() const override
    {
        return h;
    }

    Handler h;
};

class EmmisionAtom : public Node {

    bool check(const detail::AtomSet &elt) const override
    {
        return (elt.type == detail::AtomSet::Type::Atom) &&
                (elt.asAtom().name == ">>");
    }

    Handler handler() const override
    {
        return h;
    }

    Handler h;
};

class TransformationAtom : public Node {

    bool check(const detail::AtomSet &elt) const override
    {
        return (elt.type == detail::AtomSet::Type::Atom) &&
                (elt.asAtom().name == "->");
    }

    Handler handler() const override
    {
        return h;
    }

    Handler h;
};

class MultipleTransformationAtom : public Node {

    bool check(const detail::AtomSet &elt) const override
    {
        return (elt.type == detail::AtomSet::Type::Atom) &&
                (elt.asAtom().name == "=>");
    }

    Handler handler() const override
    {
        return h;
    }

    Handler h;
};

class BehaviourExtensionAtom : public Node {

    bool check(const detail::AtomSet &elt) const override
    {
        return (elt.type == detail::AtomSet::Type::Atom) &&
                (elt.asAtom().name == "like");
    }

    Handler handler() const override
    {
        return h;
    }

    Handler h;
};

Handler findPrefix(const detail::AtomSet &seq)
{
//    static PrefixTree prefixTree{
//        makeClauseDenotation<Set, EmmisionAtom, Set>([](Subject &sbj, detail::AtomSet seq){
////            sbj.addEmmision(
////                Subject{std::move(std::get<0>(seq))},
////                Space{std::move(std::get<2>(seq))}
////            );    // std::get<1>(seq) is an Atom(>>)
//        })
//    };

//    auto leafe = tree.add<Set>();
//    leafe = leafe.add<Emission>();
//    leafe = leafe.add<Set>();
//    leafe.setHandler([](Subject &sbj, std::unique_ptr<detail::AtomSet> seq){

//    });

//    tree.add<Set>().add<Emission>().add<Set>() .setHandler([](Subject &sbj, std::unique_ptr<detail::AtomSet> seq){

//    });
}

