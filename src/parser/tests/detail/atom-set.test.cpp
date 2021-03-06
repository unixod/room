#include <catch.hpp>
#include "detail/atom-set.h"

using room::parser::detail::AtomSet;

/// Helpers
namespace Catch {

template<>
struct StringMaker<AtomSet::Type> {
    static std::string convert(const AtomSet::Type &type)
    {
        switch(type) {
        case AtomSet::Type::Atom:
            return "AtomSet::Type::Atom";
        case AtomSet::Type::Set:
            return "AtomSet::Type::Set";
        case AtomSet::Type::Undefined:
            return "AtomSet::Type::Undefined";
        }

        return "Unknown";
    }
};

} // namespace Catch

bool operator == (const AtomSet::SetDescription& lhs, const AtomSet::SetDescription &rhs)
{
    return (lhs.quoted == rhs.quoted) &&
            (lhs.child == rhs.child);
}

bool operator == (const AtomSet::AtomDescription& lhs, const AtomSet::AtomDescription &rhs)
{
    return (lhs.quoted == rhs.quoted) &&
            (lhs.name == rhs.name);
}


TEST_CASE("Initialization")
{
    SECTION("Atom")
    {
        for (auto &&quoted : {true, false}) {
            AtomSet node = AtomSet::AtomDescription {
                quoted, "node"
            };

            REQUIRE(node.type == AtomSet::Type::Atom);
            REQUIRE(node.sibling == nullptr);
            REQUIRE(node.asAtom().quoted == quoted);
            REQUIRE(node.asAtom().name == "node");
            REQUIRE(node.asAtom() == static_cast<const AtomSet&>(node).asAtom());

            SECTION("Move construction")
            {
                node.sibling = std::make_unique<AtomSet>(AtomSet::AtomDescription{
                                                             false, "sibling"
                                                         });

                AtomSet newNode = std::move(node);

                REQUIRE(node.type == AtomSet::Type::Undefined);

                REQUIRE(newNode.type == AtomSet::Type::Atom);
                REQUIRE(newNode.asAtom().quoted == quoted);
                REQUIRE(newNode.asAtom().name == "node");
                REQUIRE(newNode.asAtom() == static_cast<const AtomSet&>(newNode).asAtom());

                REQUIRE(newNode.sibling != nullptr);
                REQUIRE(newNode.sibling->type == AtomSet::Type::Atom);
                REQUIRE(newNode.sibling->sibling == nullptr);
                REQUIRE(newNode.sibling->asAtom().quoted == false);
                REQUIRE(newNode.sibling->asAtom().name == "sibling");
                REQUIRE(newNode.sibling->asAtom() == static_cast<const AtomSet&>(*newNode.sibling).asAtom());
            }
        }
    }

    SECTION("Set (without childs)")
    {
        for (auto &&quoted : {true, false}) {
            AtomSet node = AtomSet::SetDescription {
                quoted, nullptr
            };

            REQUIRE(node.type == AtomSet::Type::Set);
            REQUIRE(node.sibling == nullptr);
            REQUIRE(node.asSet().quoted == quoted);
            REQUIRE(node.asSet().child == nullptr);
            REQUIRE(node.asSet() == static_cast<const AtomSet&>(node).asSet());

            SECTION("Move construction")
            {
                node.sibling = std::make_unique<AtomSet>(AtomSet::AtomDescription{
                                                             false, "sibling"
                                                         });

                AtomSet newNode = std::move(node);

                REQUIRE(node.type == AtomSet::Type::Undefined);

                REQUIRE(newNode.type == AtomSet::Type::Set);
                REQUIRE(newNode.asSet().quoted == quoted);
                REQUIRE(newNode.asSet().child == nullptr);
                REQUIRE(newNode.asSet() == static_cast<const AtomSet&>(newNode).asSet());

                REQUIRE(newNode.sibling != nullptr);
                REQUIRE(newNode.sibling->type == AtomSet::Type::Atom);
                REQUIRE(newNode.sibling->sibling == nullptr);
                REQUIRE(newNode.sibling->asAtom().quoted == false);
                REQUIRE(newNode.sibling->asAtom().name == "sibling");
                REQUIRE(newNode.sibling->asAtom() == static_cast<const AtomSet&>(*newNode.sibling).asAtom());
            }
        }
    }

    SECTION("Set (with childs)")
    {
        for (auto &&quoted : {true, false}) {
            AtomSet node = AtomSet::SetDescription {
                quoted, std::make_unique<AtomSet>(AtomSet::AtomDescription{
                                                      true, "child"
                                                  })
            };

            REQUIRE(node.type == AtomSet::Type::Set);
            REQUIRE(node.sibling == nullptr);
            REQUIRE(node.asSet().quoted == quoted);
            REQUIRE(node.asSet().child != nullptr);
            REQUIRE(node.asSet() == static_cast<const AtomSet&>(node).asSet());

            auto &&child = node.asSet().child;
            REQUIRE(child->type == AtomSet::Type::Atom);
            REQUIRE(child->sibling == nullptr);
            REQUIRE(child->asAtom().quoted == true);
            REQUIRE(child->asAtom().name == "child");
            REQUIRE(child->asAtom() == static_cast<const AtomSet&>(*child).asAtom());

            SECTION("Move construction")
            {
                node.sibling = std::make_unique<AtomSet>(AtomSet::AtomDescription{
                                                             false, "sibling"
                                                         });

                AtomSet newNode = std::move(node);

                REQUIRE(node.type == AtomSet::Type::Undefined);

                REQUIRE(newNode.type == AtomSet::Type::Set);
                REQUIRE(newNode.asSet().quoted == quoted);
                REQUIRE(newNode.asSet().child != nullptr);
                REQUIRE(newNode.asSet() == static_cast<const AtomSet&>(newNode).asSet());

                auto &&child = newNode.asSet().child;
                REQUIRE(child->type == AtomSet::Type::Atom);
                REQUIRE(child->sibling == nullptr);
                REQUIRE(child->asAtom().quoted == true);
                REQUIRE(child->asAtom().name == "child");
                REQUIRE(child->asAtom() == static_cast<const AtomSet&>(*child).asAtom());

                REQUIRE(newNode.sibling != nullptr);
                REQUIRE(newNode.sibling->type == AtomSet::Type::Atom);
                REQUIRE(newNode.sibling->sibling == nullptr);
                REQUIRE(newNode.sibling->asAtom().quoted == false);
                REQUIRE(newNode.sibling->asAtom().name == "sibling");
                REQUIRE(newNode.sibling->asAtom() == static_cast<const AtomSet&>(*newNode.sibling).asAtom());
            }
        }
    }
}

TEST_CASE("Assignment")
{
    constexpr bool quoted = true;

    SECTION("Atom")
    {
        AtomSet src = AtomSet::AtomDescription {
            not quoted, "src"
        };

        src.sibling = std::make_unique<AtomSet>(AtomSet::AtomDescription{
                                                    not quoted, "sibling"
                                                });

        SECTION("To Atom")
        {
            AtomSet node = AtomSet::AtomDescription {
                quoted, "node"
            };

            node = std::move(src);

            REQUIRE(src.type == AtomSet::Type::Undefined);

            REQUIRE(node.type == AtomSet::Type::Atom);
            REQUIRE(node.asAtom().quoted == not quoted);
            REQUIRE(node.asAtom().name == "src");
            REQUIRE(node.asAtom() == static_cast<const AtomSet&>(node).asAtom());

            REQUIRE(node.sibling != nullptr);
            REQUIRE(node.sibling->type == AtomSet::Type::Atom);
            REQUIRE(node.sibling->sibling == nullptr);
            REQUIRE(node.sibling->asAtom().quoted == not quoted);
            REQUIRE(node.sibling->asAtom().name == "sibling");
            REQUIRE(node.sibling->asAtom() == static_cast<const AtomSet&>(*node.sibling).asAtom());
        }

        SECTION("To Set")
        {
            AtomSet node = AtomSet::SetDescription {
                quoted, nullptr
            };

            node = std::move(src);

            REQUIRE(src.type == AtomSet::Type::Undefined);

            REQUIRE(node.type == AtomSet::Type::Atom);
            REQUIRE(node.asAtom().quoted == not quoted);
            REQUIRE(node.asAtom().name == "src");
            REQUIRE(node.asAtom() == static_cast<const AtomSet&>(node).asAtom());

            REQUIRE(node.sibling != nullptr);
            REQUIRE(node.sibling->type == AtomSet::Type::Atom);
            REQUIRE(node.sibling->sibling == nullptr);
            REQUIRE(node.sibling->asAtom().quoted == not quoted);
            REQUIRE(node.sibling->asAtom().name == "sibling");
            REQUIRE(node.sibling->asAtom() == static_cast<const AtomSet&>(*node.sibling).asAtom());
        }
    }

    SECTION("Set")
    {
        AtomSet src = AtomSet::SetDescription {
                not quoted, std::make_unique<AtomSet>(AtomSet::AtomDescription{
                                                          quoted, "child"
                                                      })
        };

        src.sibling = std::make_unique<AtomSet>(AtomSet::AtomDescription{
                                                    not quoted, "sibling"
                                                });

        SECTION("To Atom")
        {
            AtomSet node = AtomSet::AtomDescription {
                quoted, "node"
            };

            node = std::move(src);

            REQUIRE(src.type == AtomSet::Type::Undefined);

            REQUIRE(node.type == AtomSet::Type::Set);
            REQUIRE(node.asSet().quoted == not quoted);
            REQUIRE(node.asSet().child != nullptr);
            REQUIRE(node.asSet() == static_cast<const AtomSet&>(node).asSet());

            auto &&child = node.asSet().child;
            REQUIRE(child->type == AtomSet::Type::Atom);
            REQUIRE(child->sibling == nullptr);
            REQUIRE(child->asAtom().quoted == quoted);
            REQUIRE(child->asAtom().name == "child");
            REQUIRE(child->asAtom() == static_cast<const AtomSet&>(*child).asAtom());

            REQUIRE(node.sibling != nullptr);
            REQUIRE(node.sibling->type == AtomSet::Type::Atom);
            REQUIRE(node.sibling->sibling == nullptr);
            REQUIRE(node.sibling->asAtom().quoted == not quoted);
            REQUIRE(node.sibling->asAtom().name == "sibling");
            REQUIRE(node.sibling->asAtom() == static_cast<const AtomSet&>(*node.sibling).asAtom());
        }

        SECTION("To Set")
        {
            AtomSet node = AtomSet::SetDescription {
                quoted, nullptr
            };

            node = std::move(src);

            REQUIRE(src.type == AtomSet::Type::Undefined);

            REQUIRE(node.type == AtomSet::Type::Set);
            REQUIRE(node.asSet().quoted == not quoted);
            REQUIRE(node.asSet().child != nullptr);
            REQUIRE(node.asSet() == static_cast<const AtomSet&>(node).asSet());

            auto &&child = node.asSet().child;
            REQUIRE(child->type == AtomSet::Type::Atom);
            REQUIRE(child->sibling == nullptr);
            REQUIRE(child->asAtom().quoted == quoted);
            REQUIRE(child->asAtom().name == "child");
            REQUIRE(child->asAtom() == static_cast<const AtomSet&>(*child).asAtom());

            REQUIRE(node.sibling != nullptr);
            REQUIRE(node.sibling->type == AtomSet::Type::Atom);
            REQUIRE(node.sibling->sibling == nullptr);
            REQUIRE(node.sibling->asAtom().quoted == not quoted);
            REQUIRE(node.sibling->asAtom().name == "sibling");
            REQUIRE(node.sibling->asAtom() == static_cast<const AtomSet&>(*node.sibling).asAtom());
        }
    }
}

