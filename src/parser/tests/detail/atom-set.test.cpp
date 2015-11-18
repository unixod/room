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
        REQUIRE(node1.set.quoted == quoted);
        REQUIRE(node1.set.child == nullptr);


        AtomSet node2 = AtomSet::SetDescription {
            not quoted, &node1
        };

        REQUIRE(node2.type == AtomSet::Type::Set);
        REQUIRE(node2.set.quoted == not quoted);
        REQUIRE(node2.set.child == &node1);
    }

    SECTION("Atom")
    {
        for (auto &&quoted : {true, false}) {
            constexpr auto atomName = "some";

            AtomSet node = AtomSet::AtomDescription {
                quoted, atomName
            };

            REQUIRE(node.type == AtomSet::Type::Atom);
            REQUIRE(node.atom.quoted == quoted);
            REQUIRE(node.atom.name == atomName);
        }
    }
}

TEST_CASE("Copying")
{
}

TEST_CASE("Moving")
{

}

TEST_CASE("Access to description")
{

}
