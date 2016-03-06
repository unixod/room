#include <vector>
#include "room/common.h"
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

class AstNode {
public:
    virtual ~AstNode() {}
};

struct Subject;
struct Space;

using Handler = std::function<std::unique_ptr<detail::AtomSet>(Subject &sbj, std::unique_ptr<detail::AtomSet> seq)>;

class Environment {
public:
    const detail::AtomSet& resolveInitialEntity(const detail::AtomSet &x) const;
};

Handler findPrefix(const Environment &env, const detail::AtomSet &seq);

class Subject : public AstNode {
public:
    Subject(std::unique_ptr<detail::AtomSet> seq)
    {
        while(seq) {
            auto prefix = findPrefix(environment(), *seq);
            seq = prefix(*this, std::move(seq));
        }
    }

    const Environment& environment() const;

    void addEmmision(Subject, Space);
    void addTransformation(Subject);
    void addMultyTransformation(Subject);
    void addBehaviourExtension(Subject);
    void addBehaviourExtension(Subject, Space);
};

class Space : public AstNode {
public:
    Space(std::unique_ptr<detail::AtomSet> seq)
    {

    }
};

class Node {
public:
    virtual ~Node() {}

    template<class T>
    Node * select()
    {
        static_assert(std::is_base_of<Node, T>::value, "");
        for (auto &&child : children) {
            if (dynamic_cast<std::add_const_t<T> *>(child.get())) {
                return child.get();
            }
        }

        children.emplace_back(new T);
        return children.back().get();
    }

    virtual Handler search(const Environment &env, const detail::AtomSet &x) const
    {    
        auto&& elt = env.resolveInitialEntity(x);

        if (check(elt)) {
            if (elt.sibling) for (auto &&child : children) {
                if (auto h = child->search(env, *elt.sibling)) {
                    return h;
                }
            }

            return handler();
        }

        return nullptr;
    }

    virtual void setHandler(Handler) = 0;

private:
    virtual Handler handler() const = 0;
    virtual bool check(const detail::AtomSet &) const = 0;

protected:
    std::vector<std::unique_ptr<Node>> children;
};

class Root : public Node {
public:
    Handler search(const Environment &env, const detail::AtomSet &elt) const override
    {
        for (auto &&child : children) {
            if (auto h = child->search(env, elt)) {
                return h;
            }
        }

        return nullptr;
    }

    void setHandler(Handler) override
    {
        throw std::runtime_error{INTERNAL_ERROR "attempt to setting handler for the root of Prefix trie"};
    }

private:
    bool check(const detail::AtomSet &) const override
    {
        return true;
    }

    Handler handler() const override
    {
        return nullptr;
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

    void setHandler(Handler hndl) override
    {
        h = std::move(hndl);
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

    void setHandler(Handler hndl) override
    {
        h = std::move(hndl);
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

    void setHandler(Handler hndl) override
    {
        h = std::move(hndl);
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

    void setHandler(Handler hndl) override
    {
        h = std::move(hndl);
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

    void setHandler(Handler hndl) override
    {
        h = std::move(hndl);
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

    void setHandler(Handler hndl) override
    {
        h = std::move(hndl);
    }

    Handler h;
};

class EqualityAtom : public Node {

    bool check(const detail::AtomSet &elt) const override
    {
        return (elt.type == detail::AtomSet::Type::Atom) &&
                (elt.asAtom().name == "=");
    }

    Handler handler() const override
    {
        return h;
    }

    void setHandler(Handler hndl) override
    {
        h = std::move(hndl);
    }

    Handler h;
};

#define WITH_VARIADIC(expr) \
    using swallow##__LINE__ = int[]; \
    (void)swallow__LINE__{int{}, (expr, 0)...}

template<class... T>
class Clause {
public:
    template<class... Args>
    Clause(Args&&... args)
        : handler{std::forward<Args>(args)...}
    {}

    Handler handler;
};

template<class... T>
void insertClause(Node *node, Clause<T...> &&clause)
{
    WITH_VARIADIC(
        node = node->select<T>()
    );

    node->setHandler(std::move(clause.handler));
}

template<class... Clause>
std::unique_ptr<Node> makeTrie(Clause&&... clauses)
{
    std::unique_ptr<Node> root = std::make_unique<Root>();

    WITH_VARIADIC(
        insertClause(root.get(), std::forward<Clause>(clauses))
    );

    return root;
}

Handler findPrefix(const Environment &env, const detail::AtomSet &seq)
{
    static auto trie = makeTrie(
        Clause<Set> {
            [](Subject &sbj, std::unique_ptr<detail::AtomSet> seq) {
//                auto& env = sbj.environment();
//                sbj.setPerspective(...)
                return seq;
            }
        },
        Clause<Set, EmmisionAtom, Set> {
            [](Subject &sbj, std::unique_ptr<detail::AtomSet> seq) {
//                auto& env = sbj.currentPerspectiveEnvironment();

                assert(seq->type == detail::AtomSet::Type::Set);
                auto sbjDenotation = std::move(seq);
                seq = std::move(sbjDenotation->sibling);

                // skip denotation atom
                assert(seq->type == detail::AtomSet::Type::Atom);
                seq = std::move(seq->sibling);

                assert(seq->type == detail::AtomSet::Type::Set);
                auto spaceDenotation = std::move(seq);
                seq = std::move(spaceDenotation->sibling);

                sbj.addEmmision(
                    Subject{std::move(sbjDenotation)},
                    Space{std::move(spaceDenotation)}
                );
                return seq;
            }
        },
        Clause<TransformationAtom, Set> {
            [](Subject &sbj, std::unique_ptr<detail::AtomSet> seq) {
                // skip denotation atom
                assert(seq->type == detail::AtomSet::Type::Atom);
                seq = std::move(seq->sibling);

                assert(seq->type == detail::AtomSet::Type::Set);
                auto sbjDenotation = std::move(seq);
                seq = std::move(sbjDenotation->sibling);

                sbj.addTransformation(
                    Subject{std::move(sbjDenotation)}
                );
                return seq;
            }
        },
        Clause<MultipleTransformationAtom, Set> {
            [](Subject &sbj, std::unique_ptr<detail::AtomSet> seq) {
                // skip denotation atom
                assert(seq->type == detail::AtomSet::Type::Atom);
                seq = std::move(seq->sibling);

                assert(seq->type == detail::AtomSet::Type::Set);
                auto sbjDenotation = std::move(seq);
                seq = std::move(sbjDenotation->sibling);

                sbj.addMultyTransformation(
                    Subject{std::move(sbjDenotation)}
                );
                return seq;
            }
        },
        Clause<BehaviourExtensionAtom, Set> {
            [](Subject &sbj, std::unique_ptr<detail::AtomSet> seq) {
                // skip denotation atom
                assert(seq->type == detail::AtomSet::Type::Atom);
                seq = std::move(seq->sibling);

                assert(seq->type == detail::AtomSet::Type::Set);
                auto sbjDenotation = std::move(seq);
                seq = std::move(sbjDenotation->sibling);

                sbj.addBehaviourExtension(
                    Subject{std::move(sbjDenotation)}
                );
                return seq;
            }
        },
        Clause<BehaviourExtensionAtom, Set, EmmisionAtom, Set> {
            [](Subject &sbj, std::unique_ptr<detail::AtomSet> seq) {
                // skip denotation atom
                assert(seq->type == detail::AtomSet::Type::Atom);
                seq = std::move(seq->sibling);

                assert(seq->type == detail::AtomSet::Type::Set);
                auto sbjDenotation = std::move(seq);
                seq = std::move(sbjDenotation->sibling);

                // skip denotation atom
                assert(seq->type == detail::AtomSet::Type::Atom);
                seq = std::move(seq->sibling);

                assert(seq->type == detail::AtomSet::Type::Set);
                auto spaceDenotation = std::move(seq);
                seq = std::move(spaceDenotation->sibling);

                sbj.addBehaviourExtension(
                    Subject{std::move(sbjDenotation)},
                    Space{std::move(spaceDenotation)}
                );
                return seq;
            }
        },
        Clause<AnyAtom, EqualityAtom, AnyAtom> {
            [](Subject &sbj, std::unique_ptr<detail::AtomSet> seq) {

                return seq;
            }
        },
        Clause<AnyAtom, EqualityAtom, Set> {
            [](Subject &sbj, std::unique_ptr<detail::AtomSet> seq) {

                return seq;
            }
        },
        Clause<Set, EqualityAtom, Set> {
            [](Subject &sbj, std::unique_ptr<detail::AtomSet> seq) {

                return seq;
            }
        },
        Clause<Set, EqualityAtom, AnyAtom> {
            [](Subject &sbj, std::unique_ptr<detail::AtomSet> seq) {

                return seq;
            }
        }
    );

    return trie->search(env, seq);
}
