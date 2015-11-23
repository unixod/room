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
    }
};

} // namespace Catch


TEST_CASE("Initialization")
{
    SECTION("Set")
    {
        for (auto &&quoted : {true, false}) {
            AtomSet node = AtomSet::SetDescription {
                quoted, nullptr
            };

            REQUIRE(node.type == AtomSet::Type::Set);
            REQUIRE(node.sibling == nullptr);
            REQUIRE(node.asSet().quoted == quoted);
            REQUIRE(node.asSet().child == nullptr);
        }
    }

    SECTION("Atom")
    {
        for (auto &&quoted : {true, false}) {
            constexpr auto atomName = "some";

            AtomSet node = AtomSet::AtomDescription {
                quoted, atomName
            };

            REQUIRE(node.type == AtomSet::Type::Atom);
            REQUIRE(node.sibling == nullptr);
            REQUIRE(node.asAtom().quoted == quoted);
            REQUIRE(node.asAtom().name == atomName);
        }
    }


    constexpr auto quoted = true;
    constexpr auto atomName = "some";
    AtomSet root = AtomSet::SetDescription {
        quoted, std::make_unique<AtomSet>(
                    AtomSet::AtomDescription {
                        quoted, atomName
                    }
                )
    };

    REQUIRE(root.type == AtomSet::Type::Set);
    REQUIRE(root.sibling == nullptr);
    REQUIRE(root.asSet().quoted == quoted);
    REQUIRE(root.asSet().child != nullptr);

    auto &&child = root.asSet().child;
    REQUIRE(child->type == AtomSet::Type::Atom);
    REQUIRE(child->sibling == nullptr);
    REQUIRE(child->asAtom().quoted == quoted);
    REQUIRE(child->asAtom().name == atomName);
}

TEST_CASE("Movement")
{
//    constexpr auto quoted = true;

//    SECTION("Set") {
//        AtomSet initial = AtomSet::SetDescription {
//            quoted, nullptr
//        };

//        AtomSet sibling = AtomSet::SetDescription {
//            quoted, nullptr
//        };

//        AtomSet child = AtomSet::SetDescription {
//            quoted, nullptr
//        };

//        initial.sibling = &sibling;
//        initial.asSet().child = &child;

//        SECTION("Initialization")
//        {
//            AtomSet node = std::move(initial);

////            REQUIRE(node.type == initial.type);
////            REQUIRE(node.sibling == initial.sibling);
////            REQUIRE(node.asSet().quoted == node.asSet().quoted);
////            REQUIRE(node.asSet().child == node.asSet().child);
//        }

//        SECTION("Assignment")
//        {

//        }
//    }
}

TEST_CASE("Access to description")
{

}
