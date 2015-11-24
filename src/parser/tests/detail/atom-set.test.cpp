#include <catch.hpp>
#include "testing/facilities.h"
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

            SECTION("Move construction")
            {
                node.sibling = std::make_unique<AtomSet>(AtomSet::AtomDescription{
                                                             false, "sibling"
                                                         });

                AtomSet newNode = std::move(node);

                REQUIRE(newNode.type == AtomSet::Type::Atom);
                REQUIRE(newNode.asAtom().quoted == quoted);
                REQUIRE(newNode.asAtom().name == "node");

                REQUIRE(newNode.sibling != nullptr);
                REQUIRE(newNode.sibling->type == AtomSet::Type::Atom);
                REQUIRE(newNode.sibling->sibling == nullptr);
                REQUIRE(newNode.sibling->asAtom().quoted == false);
                REQUIRE(newNode.sibling->asAtom().name == "sibling");
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

            SECTION("Move construction")
            {
                node.sibling = std::make_unique<AtomSet>(AtomSet::AtomDescription{
                                                             false, "sibling"
                                                         });

                AtomSet newNode = std::move(node);

                REQUIRE(newNode.type == AtomSet::Type::Set);
                REQUIRE(newNode.asSet().quoted == quoted);
                REQUIRE(newNode.asSet().child == nullptr);

                REQUIRE(newNode.sibling != nullptr);
                REQUIRE(newNode.sibling->type == AtomSet::Type::Atom);
                REQUIRE(newNode.sibling->sibling == nullptr);
                REQUIRE(newNode.sibling->asAtom().quoted == false);
                REQUIRE(newNode.sibling->asAtom().name == "sibling");
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

            auto &&child = node.asSet().child;
            REQUIRE(child->type == AtomSet::Type::Atom);
            REQUIRE(child->sibling == nullptr);
            REQUIRE(child->asAtom().quoted == true);
            REQUIRE(child->asAtom().name == "child");

            SECTION("Move construction")
            {
                node.sibling = std::make_unique<AtomSet>(AtomSet::AtomDescription{
                                                             false, "sibling"
                                                         });

                AtomSet newNode = std::move(node);

                REQUIRE(newNode.type == AtomSet::Type::Set);
                REQUIRE(newNode.asSet().quoted == quoted);
                REQUIRE(newNode.asSet().child != nullptr);

                auto &&child = newNode.asSet().child;
                REQUIRE(child->type == AtomSet::Type::Atom);
                REQUIRE(child->sibling == nullptr);
                REQUIRE(child->asAtom().quoted == true);
                REQUIRE(child->asAtom().name == "child");

                REQUIRE(newNode.sibling != nullptr);
                REQUIRE(newNode.sibling->type == AtomSet::Type::Atom);
                REQUIRE(newNode.sibling->sibling == nullptr);
                REQUIRE(newNode.sibling->asAtom().quoted == false);
                REQUIRE(newNode.sibling->asAtom().name == "sibling");
            }
        }
    }
}

TEST_CASE("Assignment")
{

}

TEST_CASE("Access to description")
{

}
