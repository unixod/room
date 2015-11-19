#include <catch.hpp>
#include "detail/atom-set.h"

using room::parser::detail::AtomSet;

TEST_CASE("Initialization")
{
    SECTION("Set")
    {
        constexpr auto quoted = true;

        AtomSet node1 = AtomSet::SetDescription {
            quoted, nullptr
        };

        REQUIRE(node1.type == AtomSet::Type::Set);
        REQUIRE(node1.sibling == nullptr);
        REQUIRE(node1.asSet().quoted == quoted);
        REQUIRE(node1.asSet().child == nullptr);


        AtomSet node2 = AtomSet::SetDescription {
            not quoted, &node1
        };

        REQUIRE(node2.type == AtomSet::Type::Set);
        REQUIRE(node2.sibling == nullptr);
        REQUIRE(node2.asSet().quoted == not quoted);
        REQUIRE(node2.asSet().child == &node1);
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
